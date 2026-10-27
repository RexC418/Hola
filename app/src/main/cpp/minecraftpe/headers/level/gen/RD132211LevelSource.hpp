#pragma once
#include <level/gen/ChunkSource.hpp>
#include <map>
#include <util/Random.hpp>

/*
 * 1:1 port of the Minecraft Pre-Classic rd-132211 terrain generator
 * (com.mojang.rubydung.level.Level from the RubyDung source) to MCPE 0.8.1.
 *
 * Original generation (Level.java, constructor):
 *     Level(256, 256, 64)
 *     blocks[i] = (byte)(y <= depth * 2 / 3 ? 1 : 0)
 * where the vertical axis "y" goes from 0 to depth-1 (depth = 64),
 * so every column is solid rock for y <= 64 * 2 / 3 (= 42) and air above.
 *
 * Port notes (per the mod specification):
 *  - rd-132211 "rock" (Tile.rock, tex 0) maps to MCPE 0.8.1 Cobblestone
 *    (Tile::stoneBrick, block ID 4, "cobblestone") for the 42 bottom layers.
 *  - The topmost solid layer (y = 42) uses MCPE 0.8.1 Grass Block
 *    (Tile::grass, block ID 2).
 *  - Everything above the surface is air; the world edge is open so the
 *    player can walk off the 256x256 map and fall into the void, exactly
 *    like in rd-132211.
 *  - No decoration of any kind is performed (rd-132211 had no caves, ores,
 *    trees, water or mobs).
 */
struct RD132211LevelSource : ChunkSource{
	// rd-132211 world height (the vertical "depth" of RubyDung's Level)
	static const int32_t RD_WORLD_DEPTH = 64;
	// Last solid layer: y <= RD_WORLD_DEPTH * 2 / 3  (= 42, like rd-132211)
	static const int32_t RD_SURFACE_Y = RD_WORLD_DEPTH * 2 / 3;

	struct Level* level;
	bool spawnMobs;
	std::map<int32_t, struct LevelChunk*> chunkMap;
	Random random;

	RD132211LevelSource(struct Level*, long, int32_t, bool_t);
	virtual ~RD132211LevelSource();
	virtual bool_t hasChunk(int32_t, int32_t);
	virtual struct LevelChunk* getChunk(int32_t, int32_t);
	virtual struct LevelChunk* create(int32_t, int32_t);
	virtual void postProcess(ChunkSource*, int32_t, int32_t);
	virtual bool_t tick();
	virtual bool_t shouldSave();
	virtual std::vector<Biome::MobSpawnerData> getMobsAt(const struct MobCategory&, int32_t, int32_t, int32_t);
	virtual std::string gatherStats();
};
