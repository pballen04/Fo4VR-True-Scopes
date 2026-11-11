# Optional FRIK UI companion

Separate FO4VRScopeCalibration.dll; TrueScopes does not depend on FRIK.
The live ABI is in ../../src/TrueScopes/CalibrationAPI.h. Save/reset/cancel
profiles are handled by the companion and keyed using plugin/local form IDs.
No personal profile JSON is included.

Build this directory independently with the framework's normal vcpkg setup.
Set `F4VR_COMMON_FRAMEWORK_PATH` to the framework checkout. The original build
used F4VR-CommonFramework at `3f0bbc8c45c1193b16ca1c09b9f8ba2a97b95890` and
CommonLibF4VR at `90570520f646e32bfe07bd0cbf9f51bf2aee76b3`.
The tested FRIK external-menu API header is included under sdk with its parent
project's GPL-3.0 attribution retained by the repository license.

This source builds a DLL. It is **not a complete installable UI package** yet:
the green launcher, controller-relative row/button meshes and textures used
by the existing private installation need a final provenance/packaging pass.
Do not distribute a DLL alone as an installable menu. Position/tilt are explicit
modes; grip/trigger modifiers are not required. Speed, Save, Reset and Cancel
are distinct controls with neutral-stick/debounce handling.
