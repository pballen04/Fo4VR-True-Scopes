# Optional FRIK UI companion

Separate FO4VRScopeCalibration.dll; TrueScopes does not depend on FRIK.
The live ABI is in ../../src/TrueScopes/CalibrationAPI.h. Save/reset/cancel
profiles are handled by the companion and keyed using plugin/local form IDs.
No personal profile JSON is included.

Build this directory independently with the compatible framework source path
and its normal vcpkg setup. F4VR-CommonFramework revision and CommonLib headers
used for the isolated local build are recorded in ../../docs/LOCAL_DEPENDENCIES.json.
The tested FRIK external-menu API header is included under sdk with its parent
project's GPL-3.0 attribution retained by the repository license.

This source builds a DLL. It is **not a complete installable UI package** yet:
the green launcher, controller-relative row/button meshes and textures used
by the existing private installation need a final provenance/packaging pass.
Do not distribute a DLL alone as an installable menu. Position/tilt are explicit
modes; grip/trigger modifiers are not required. Speed, Save, Reset and Cancel
are distinct controls with neutral-stick/debounce handling.
