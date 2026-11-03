# Live calibration integration candidate

Based on upstream v0.3.82, commit
`2e06b23b3c0ddaecea2ae3f7250a59e8eb57bd0c`. The feature adds a versioned
POD-only API; TrueScopes does not require FRIK to load or render.

`TrueScopes_GetCalibrationAPI(2, sizeof(API))` returns read/set callbacks.
Call these on the game thread. Callers supply State storage with its byte size.
Epoch/probe and equipped-weapon/OMOD identity prevent a delayed preview from
editing a replacement weapon. No engine pointers, allocation ownership, STL or
CommonLib types cross the DLL boundary. See src/TrueScopes/CalibrationAPI.h.

Values cover weapon-relative right/forward/up, size, pitch and yaw. The default
values are a no-op. Translation is transformed into the widget parent's frame;
tilt composes with the unadjusted tracking rotation each frame, avoiding
accumulated tilt. Calibration changes the widget, not projectile direction,
scope camera/FOV, or activation logic. Existing upstream STS ownership/camera
paths are retained and decline normal widget fitting while STS owns it.

For unknown/fallback optics, the whole baseline is captured in weapon space.
Bracing/moving/reparenting the gun must not leave the widget behind in a hand
or UI frame. The bounded anchor store uses the existing personal path
Mods_Config/FO4VRScopeCalibration/fallback-anchors-v1.json. Invalid storage is
preserved, writes are disabled and in-session anchoring remains available.
The optional UI companion owns user profile save/reset/cancel; API calls alone
do not save a profile. Personal CW/FAL tables and saved profiles are excluded
from this upstream branch.

Regression tests cover bounds/NaN rejection, epoch changes, translation frames,
nonaccumulating tilt, rigid fallback motion, reparenting and anchor JSON.
Existing upstream lifecycle/rotation/optic/reticle tests remain enabled.
An isolated local build can reuse matching cached CommonLib headers/libraries;
that is separate from a clean dependency/submodule build and a headset test.

## Optional FRIK companion

The existing companion uses this ABI, provides the True Scopes launcher, near
controller rows, position/tilt modes, speed, Save, Reset and Cancel, and persists
per-weapon/OMOD profiles. It is an optional separate DLL, not a FRIK patch.
Its source lives under integrations/frik. UI runtime asset provenance/packaging
and upstream maintainer review remain required before release.

## Standalone red dots alongside TrueScopes

Upstream v0.3.82 already exposes `reflexReticleEnabled`. Set it false **before
game startup** when the standalone renderer owns reflex sights. Hooks.cpp then
does not install TrueScopes' reflex draw hooks; magnified scopes remain enabled.
This branch does not replace upstream Reticle/Optic code or add competing draw
hooks. The red-dot repository provides a generator for a separate reversible
MO2 config override that retains the user's other TOML settings.

Headset acceptance is still required for vanilla, modded and piggyback optics,
save/load, cancel/reset, two-handed/braced motion and coexistence on v0.3.82.
No live installation was changed by this port.
