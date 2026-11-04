#include "TrueScopes/AimingEye.h"
#include "TrueScopes/WidgetRotation.h"
#include <cstdio>
#include <cstdlib>
#include <limits>

static void require(bool pass, const char* message) { if (!pass) { std::puts(message); std::exit(1); } }
int main()
{
    using TrueScopes::AimingEye::Selector;
    Selector eye;
    // The reported CW raise chose left at 2.39/3.87. It must not lock on one
    // transient sample, then stay left after the right eye reaches the tube.
    eye.Update(1, 100, -1, 2.39f, 3.87f, true, 2.6f, 2.5f);
    require(eye.Side() == 0, "Transient raise latched the eye");
    for (std::uint64_t t=120; t<=320; t+=20) eye.Update(1,t,-1,4.5f,.1f,true,2.6f,2.5f);
    require(eye.Side() == 1, "Settled right aiming eye was not selected");
    eye.Update(1,340,-1,.1f,4.5f,true,2.6f,2.5f);
    eye.Update(1,360,-1,4.5f,.1f,true,2.6f,2.5f);
    require(eye.Side() == 1, "One-frame crossover flipped the eye");
    for (std::uint64_t t=380;t<=600;t+=20) eye.Update(1,t,-1,.1f,4.5f,true,2.6f,2.5f);
    require(eye.Side() == -1, "A genuine sustained eye change could not recover");
    eye.Update(2,620,1,7.f,8.f,true,2.6f,2.5f);
    require(eye.Side() == 0, "New scope retained a previous latch");
    for(std::uint64_t t=640;t<=900;t+=20) eye.Update(2,t,1,3.37f,7.63f,true,2.6f,2.5f);
    require(eye.Side() == 0, "Off-axis transit became a permanent latch");
    eye.Update(2,920,1,.1f,4.5f,true,2.6f,2.5f);
    eye.Update(2,2000,1,.1f,4.5f,true,2.6f,2.5f);
    require(eye.Side() == 0, "A long missing-render interval counted as dwell");
    eye.Reset();
    for(std::uint64_t t=2100;t<=2400;t+=20) eye.Update(2,t,1,std::numeric_limits<float>::quiet_NaN(),-1.f,true,2.6f,2.5f);
    require(eye.Side() == 0, "Invalid eye geometry latched");

    // Reproduce session-global L0 contamination across guns and attachments.
    TrueScopes::WidgetRotation::AuthoredBaseline baseline;
    const std::uint32_t modsA[]{1,2}, modsReordered[]{2,1}, modsB[]{1,3};
    float first[12]{1,0,0,0,0,1,0,0,0,0,1,0};
    float second[12]{0,1,0,0,-1,0,0,0,0,0,1,0};
    require(baseline.Observe(10,"scope-a",modsA) && !baseline.Valid(),"First identity invalid");
    baseline.Capture(first);
    require(baseline.Observe(10,"scope-a",modsReordered) && baseline.Valid(),"Probe/mod ordering reset the same baseline");
    require(baseline.Observe(11,"scope-a",modsA) && !baseline.Valid(),"Second gun inherited the first gun's L0");
    baseline.Capture(second);
    require(baseline.Raw()[0]==0 && baseline.Raw()[1]==1,"Second authored frame not captured");
    require(baseline.Observe(11,"scope-b",modsA) && !baseline.Valid(),"Different optic inherited L0");
    baseline.Capture(first);
    require(baseline.Observe(11,"scope-b",modsB) && !baseline.Valid(),"Different attachment configuration inherited L0");
    std::puts("Scope alignment regression tests passed: eye dwell/recovery and per-configuration authored rotation.");
}
