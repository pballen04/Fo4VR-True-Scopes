#pragma once
#include "CalibrationAPI.h"
#include <string>
namespace TrueScopes::Calibration
{
    // Renderer consumes a copied snapshot, only for the same completed identity probe.
    bool ForRender(std::uint32_t weapon, const char* optic, std::uint32_t probe, Values& out);
    // Internal extension only; public menu ABI v2 and saved profiles unchanged.
    bool FallbackContext(std::uint32_t weapon,const char* optic,std::uint32_t probe,
                         Values& out,std::string& stableKey);
}
