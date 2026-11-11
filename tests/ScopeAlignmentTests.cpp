#include "TrueScopes/WidgetRotation.h"
#include <cstdio>
#include <cstdlib>

static void require(bool pass, const char* message) { if (!pass) { std::puts(message); std::exit(1); } }
int main()
{
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
    std::puts("Scope alignment regression tests passed: per-configuration authored rotation.");
}
