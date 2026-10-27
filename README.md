# MCPE 0.8.1 — rd-132211 Terrain Generator Port

Minecraft Pocket Edition 0.8.1 (decompiled) with a 1:1 port of the
**Minecraft Pre-Classic rd-132211** terrain generator, plus a new
"Terrain Settings" UI to pick the terrain generator used for new worlds.

## Changelog

* **v1.2 — CI build fix**: `app/src/main/cpp/.gitignore` (inherited from the
  decompiled project) was ignoring `minecraftpe/impl/pcm_data.c`, so the
  repository pushed to GitHub was missing the 31 MB embedded-sound source and
  `libminecraftpe.so` failed to link (106 undefined `PCM_*` symbols). The
  ignore entries (`pcm_data.c`, `assets/*`, `sounds/*`) were removed; the
  workflow now accepts SDK licenses before installing the NDK.
* **v1.1 — Startup crash fix (SIGSEGV in `AAssetManager_open`)**: on some
  devices the game crashed ~1 s after launch
  (`pthread_mutex_lock` <- `AAssetManager_open` <- `TextureAtlas::load`).
  Cause: the line in `android_main` that hands the `ANativeActivity`'s asset
  manager to `AppPlatform_android23` was left commented out in the decompiled
  source, so `assetManager` stayed NULL while `initialized` was already 1.
  Fix: call `appPlatform.initWithActivity(activity)` in `android_main`
  (`jni.cpp`) plus a defensive NULL check in
  `AppPlatform_android23::readAssetFile` so a missing asset manager can never
  segfault the engine again.

## What the mod does

* **rd-132211 terrain generation** (`RD132211LevelSource`): each world is a
  256x256 map where every column is solid for `y <= 64 * 2 / 3` (= 42), exactly
  like RubyDung's `Level(256, 256, 64)`. The 42 bottom layers are
  **Cobblestone** (the MCPE stand-in for rd-132211's rock) and the top layer is
  **Grass Block**. No caves, ores, trees, water or mobs — just like pre-classic.
* **No invisible bedrock barrier**: `EmptyLevelChunk` now returns air instead of
  invisible bedrock, so the world edge is open. You can walk off the 256x256
  map and **fall into the void** (and die), exactly like rd-132211. The vanilla
  MCPE 0.8.1 void-kill mechanic is kept.
* **Terrain Settings button** on the Create World screen opens the
  **Terrain Generation** selector:
  `[ < ]  [ RD-132211 ]  [ > ]`
  The center shows the version the selected generator belongs to; the arrow
  buttons are fully wired to cycle the generator list (they do nothing while
  RD-132211 is the only entry). New generators can be added in
  `minecraftpe/headers/util/TerrainGen.hpp` / `impl/util/TerrainGen.cpp`.
* The selected generator is stored per world in the level.dat NBT tag
  `GeneratorVersion` (0 = vanilla MCPE, 1 = rd-132211), so worlds keep
  regenerating their chunks with the right generator after save/load. The
  vanilla MCPE 0.8.1 save/load system is untouched otherwise.

## Project layout

The Android project lives at the repository root (Gradle 8.6 / AGP 8.4.0):

* `app/src/main/cpp/` — decompiled MCPE 0.8.1 C++ source (with the mod changes)
* `app/src/main/cpp/minecraftpe/impl/pcm_data.c` — embedded sound data
* `app/src/main/assets/`, `app/src/main/res/` — original MCPE 0.8.1 assets/resources
* `app/src/main/java/` — MCPE 0.8.1 Java glue code

Build target: **arm64-v8a**, **minSdk 24**, **targetSdk 34**.

## Building

### Locally

```bash
# requires JDK 17, Android SDK + NDK 26.3.11579264 + CMake 3.22.1
./gradlew assembleRelease
# output: app/build/outputs/apk/release/app-release.apk (signed with the debug key)
```

### GitHub Actions

Push this repository to GitHub; the included workflow
`.github/workflows/build.yml` builds the arm64-v8a release APK automatically and
uploads it as the `MCPE-0.8.1-rd132211-arm64-v8a` artifact.

## Mod source map

| Change | File |
| --- | --- |
| New rd-132211 chunk source | `cpp/minecraftpe/headers/level/gen/RD132211LevelSource.hpp`, `cpp/minecraftpe/impl/level/gen/RD132211LevelSource.cpp` |
| Generator registry / selector data | `cpp/minecraftpe/headers/util/TerrainGen.hpp`, `cpp/minecraftpe/impl/util/TerrainGen.cpp` |
| Terrain Settings screen + arrow buttons | `cpp/minecraftpe/headers/gui/screens/TerrainSettingsScreen.hpp`, `cpp/minecraftpe/impl/gui/screens/TerrainSettingsScreen.cpp` |
| "Terrain Settings" button on Create World | `cpp/minecraftpe/headers/gui/screens/CreateWorldScreen.hpp`, `cpp/minecraftpe/impl/gui/screens/CreateWorldScreen.cpp` |
| Generator plumbing (settings → level data) | `cpp/minecraftpe/headers/level/LevelSettings.hpp`, `cpp/minecraftpe/impl/level/storage/LevelData.cpp`, `cpp/minecraftpe/impl/level/dimension/Dimension.cpp`, `cpp/minecraftpe/impl/Minecraft.cpp` |
| Invisible bedrock barrier removed (void) | `cpp/minecraftpe/headers/level/chunk/EmptyLevelChunk.hpp` |
| Arrow texture for the selector | `app/src/main/assets/images/gui/terrain_arrows.png` |
