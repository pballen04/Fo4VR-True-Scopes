#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace TrueScopes::AimingEye
{
    // Render-thread state. A crossing during a raise must not lock the wrong
    // eye for the rest of an episode. Latch only after a continuous on-axis
    // dwell; a settled, clearly better eye may recover a stale latch later.
    class Selector
    {
    public:
        static constexpr std::uint64_t dwellMs = 180;
        void Reset() noexcept { initialized_ = false; side_ = pending_ = 0; }
        int Side() const noexcept { return side_; }
        int Update(std::uint64_t episode, std::uint64_t now, int dominant,
                   float left, float right, bool live, float radius, float margin) noexcept
        {
            dominant = dominant < 0 ? -1 : 1;
            if (!initialized_ || episode != episode_ || now < last_) {
                episode_ = episode; initialized_ = true; side_ = pending_ = 0;
            }
            last_ = now;
            const float lat[2]{left, right};
            const auto valid = [&](int side) { const float x = lat[side > 0 ? 1 : 0]; return std::isfinite(x) && x >= 0; };
            const auto at = [&](int side) { return lat[side > 0 ? 1 : 0]; };
            const float tolerance = std::isfinite(radius) ? (std::max)(.05f, radius) : .05f;
            margin = std::isfinite(margin) ? (std::max)(0.f, margin) : 0.f;
            const int current = side_ ? side_ : dominant;
            const int other = -current;
            const bool better = valid(other) && (!valid(current) || at(other) + margin < at(current));
            const int candidate = better ? other : current;
            const bool centered = live && valid(candidate) && at(candidate) <= tolerance;
            if (!centered || candidate == side_) {
                pending_ = 0;
            } else {
                // A missed render interval is not evidence of continuous aim.
                if (pending_ != candidate || now - pendingLast_ > 100) {
                    pending_ = candidate; since_ = now;
                }
                pendingLast_ = now;
                if (now - since_ >= dwellMs) { side_ = candidate; pending_ = 0; }
            }
            // Before a latch, use the current best guess without publishing it
            // to the gate as a committed eye. A bad locked eye gets the same
            // dwell before recovery, avoiding flips on each frame of a sweep.
            return side_ ? side_ : candidate;
        }
    private:
        bool initialized_ = false;
        std::uint64_t episode_ = 0, last_ = 0, since_ = 0, pendingLast_ = 0;
        int side_ = 0, pending_ = 0;
    };
}
