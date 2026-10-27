#pragma once
#include <_types.h>

/*
 * Terrain generator registry for the "Terrain Settings" -> "Terrain Generation"
 * screen. Each entry is a terrain generator ported from another Minecraft
 * version; new generators can be appended to TerrainGen::GENERATORS and they
 * will automatically show up in the selector (the green arrow buttons then
 * start cycling through them).
 *
 * Generator IDs (stored in LevelSettings::generatorVersion / LevelData NBT
 * tag "GeneratorVersion" so every saved world keeps the generator it was
 * created with):
 *   0 = vanilla MCPE 0.8.1 generation (not listed in the selector)
 *   1 = rd-132211 (Minecraft Pre-Classic) generation
 */
#define TERRAINGEN_ID_NORMAL    0
#define TERRAINGEN_ID_RD132211  1

struct TerrainGenEntry {
	int32_t id;
	const char* name;
};

struct TerrainGen {
	// Generators offered by the "Terrain Generation" selector, in cycle order.
	static const TerrainGenEntry GENERATORS[];
	static const int32_t GENERATOR_COUNT;

	// Currently selected entry (index into GENERATORS) used for new worlds.
	static int32_t selected;

	static int32_t getSelectedIndex();
	static void setSelectedIndex(int32_t index);
	static int32_t getSelectedId();
	static const char* getSelectedName();
	static const char* getName(int32_t index);

	// Left green arrow: dir = -1 (go back). Right green arrow: dir = +1 (advance).
	// Fully wired for future generators; while the list only has one entry
	// (RD-132211) pressing the arrows intentionally does nothing at all.
	static void cycle(int32_t dir);
};
