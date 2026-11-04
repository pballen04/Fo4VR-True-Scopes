Add an optional live scope-widget calibration API

Unrecognized modded scopes can have a displaced or angled widget. This adds a
versioned game-thread API to preview weapon-relative translation, size, pitch
and yaw, and an optional FRIK companion to save per-weapon/attachment profiles.
Default calibration is a no-op. Fallback optics keep a rigid weapon-local
baseline so bracing/reparenting does not detach the picture from the gun.

The port is based on v0.3.82 and retains its reflex renderer, same-frame pose
sampling and STS ownership/camera paths. No personal weapon-fitting tables or
profiles are included. The ABI and storage/threading contract are documented
in docs/LIVE_CALIBRATION.md.

The original engine widget orientation is now keyed to weapon/optic/OMOD
identity, and automatic eye selection requires a centered dwell and can recover
from an eye chosen during the raise. These avoid carrying the first gun's local
frame into another gun and leaving a scope blacked out for the wrong eye.

Validation: isolated Release backend and optional companion builds; eight
offline tests for lifecycle, rotation, optic classification, reticle fitting,
calibration frames/epochs, sticks, fallback anchoring and eye/identity recovery. Uses an existing
matching dependency cache; a clean upstream submodule build is still pending.
The original older-backend integration was tested in VR; this v0.3.82 port
still requires its own headset test. Optional UI runtime assets and dependency
review remain release gates. This is a local draft; no PR has been submitted.
