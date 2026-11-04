#pragma once

#include <cstddef>
#include <cstring>
#include <cstdint>
#include <array>
#include <span>
#include <algorithm>
#include <cmath>

namespace TrueScopes::WidgetRotation
{
	// The engine's local widget orientation belongs to this weapon/optic/mod
	// configuration. Keeping one session-global L0 lends the first gun's frame
	// to a later gun, even though its tracking calibration was correctly reset.
	class AuthoredBaseline
	{
	public:
		bool Observe(std::uint32_t weapon, const char* optic, std::span<const std::uint32_t> mods) noexcept
		{
			if (!weapon || !optic || !optic[0] || mods.size() > mods_.size()) return false;
			std::size_t length = 0;
			while (length < sizeof(optic_) && optic[length]) ++length;
			if (length == sizeof(optic_)) return false;
			std::array<std::uint32_t, 24> sorted{};
			std::copy(mods.begin(), mods.end(), sorted.begin());
			std::sort(sorted.begin(), sorted.begin() + mods.size());
			if (weapon != weapon_ || std::strcmp(optic_, optic) != 0 || count_ != mods.size() || sorted != mods_) {
				weapon_ = weapon; std::memcpy(optic_, optic, length + 1);
				mods_ = sorted; count_ = mods.size(); valid_ = false; ++revision_;
			}
			return true;
		}
		bool Valid() const noexcept { return valid_; }
		std::uint64_t Revision() const noexcept { return revision_; }
		bool Capture(const float* raw) noexcept {
			for (int c=0;c<3;++c) for(int r=0;r<3;++r) if(!std::isfinite(raw[c*4+r])) return false;
			std::memcpy(raw_, raw, sizeof(raw_)); valid_ = true; return true;
		}
		const float* Raw() const noexcept { return raw_; }
	private:
		std::uint32_t weapon_ = 0;
		char optic_[128]{};
		std::array<std::uint32_t, 24> mods_{};
		std::size_t count_ = 0;
		std::uint64_t revision_ = 0;
		float raw_[12]{};
		bool valid_ = false;
	};

	// Weapon-to-widget rotation calibration. Identity probes refresh which optic
	// geometry supplies Rw, but they do not replace ScopeParent or its engine
	// baseline; only a real widget lifecycle reset may discard this calibration.
	class Calibration
	{
	public:
		[[nodiscard]] bool Captured() const noexcept
		{
			return captured_;
		}

		// An aimed capture was taken with the scope armed and the eye on the tube;
		// a provisional one only with the weapon still. Provisional K is re-taken at
		// every adoption (the pre-cache behaviour, which is fine at load); aimed K
		// is cached for the scope and never re-learned. One provisional capture
		// made permanent by the cache tilted a whole session (2026-09-17).
		[[nodiscard]] bool Aimed() const noexcept
		{
			return aimed_;
		}

		void Reset() noexcept
		{
			captured_ = false;
			aimed_ = false;
		}

		// Deliberately preserves K. Scope-in requests an identity probe, including
		// when FRIK has already re-aimed the weapon from a two-hand grip. Rebuilding
		// K against the engine's one-hand L0 in that pose snaps the disc off-axis.
		void PreserveAcrossIdentityProbe() noexcept {}

		// K is the mesh-to-disc rotation: a constant per weapon and grip. Restoring a
		// cached K across an adoption is what keeps a carry edge, a FRIK rewrite or a
		// Pip-Boy transit from re-learning it against a local that is our own previous
		// write and a relation that has not settled yet.
		void Restore(const float (&a_alignment)[9]) noexcept
		{
			std::memcpy(alignment_, a_alignment, sizeof(alignment_));
			captured_ = true;
			aimed_ = true;  // only aimed captures are ever cached
		}

		[[nodiscard]] bool Export(float (&a_out)[9]) const noexcept
		{
			if (!captured_) {
				return false;
			}
			std::memcpy(a_out, alignment_, sizeof(alignment_));
			return true;
		}

		void Capture(const float (&a_weaponWorld)[9], const float (&a_parentWorld)[9],
			const float (&a_engineLocal)[9], bool a_aimed) noexcept
		{
			float parentTimesLocal[9];
			Multiply(a_parentWorld, a_engineLocal, parentTimesLocal);
			MultiplyTransposeLeft(a_weaponWorld, parentTimesLocal, alignment_);
			captured_ = true;
			aimed_ = a_aimed;
		}

		[[nodiscard]] bool Compute(const float (&a_weaponWorld)[9],
			const float (&a_parentWorld)[9], float (&a_localOut)[9]) const noexcept
		{
			if (!captured_) {
				return false;
			}
			float weaponTimesAlignment[9];
			Multiply(a_weaponWorld, alignment_, weaponTimesAlignment);
			MultiplyTransposeLeft(a_parentWorld, weaponTimesAlignment, a_localOut);
			return true;
		}

	private:
		static void Multiply(const float (&a_a)[9], const float (&a_b)[9],
			float (&a_out)[9]) noexcept
		{
			for (std::size_t r = 0; r < 3; ++r) {
				for (std::size_t c = 0; c < 3; ++c) {
					a_out[r * 3 + c] = a_a[r * 3 + 0] * a_b[0 * 3 + c] +
					                       a_a[r * 3 + 1] * a_b[1 * 3 + c] +
					                       a_a[r * 3 + 2] * a_b[2 * 3 + c];
				}
			}
		}

		// a_a^T * a_b
		static void MultiplyTransposeLeft(const float (&a_a)[9], const float (&a_b)[9],
			float (&a_out)[9]) noexcept
		{
			for (std::size_t r = 0; r < 3; ++r) {
				for (std::size_t c = 0; c < 3; ++c) {
					a_out[r * 3 + c] = a_a[0 * 3 + r] * a_b[0 * 3 + c] +
					                       a_a[1 * 3 + r] * a_b[1 * 3 + c] +
					                       a_a[2 * 3 + r] * a_b[2 * 3 + c];
				}
			}
		}

		bool  captured_ = false;
		bool  aimed_ = false;
		float alignment_[9] = {};
	};
}
