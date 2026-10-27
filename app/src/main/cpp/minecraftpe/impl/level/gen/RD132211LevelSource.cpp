#include <level/gen/RD132211LevelSource.hpp>
#include <level/Level.hpp>
#include <level/chunk/LevelChunk.hpp>
#include <tile/Tile.hpp>
#include <string.h>

RD132211LevelSource::RD132211LevelSource(struct Level* a2, long a3, int32_t a4, bool_t a5)
		: random(a3) {
	this->level = a2;
	this->spawnMobs = a5;
}

RD132211LevelSource::~RD132211LevelSource() {
	// Chunks handed out to the ChunkCache stay alive for the lifetime of the
	// level (same ownership model as RandomLevelSource) - nothing to free here.
}

bool_t RD132211LevelSource::hasChunk(int32_t, int32_t) {
	return 1;
}

static int32_t rd_chunkKey(int32_t chunkX, int32_t chunkZ) {
	uint32_t key = (uint32_t)(chunkZ & 0x7FFF) | ((uint32_t)(chunkX & 0x7FFF) << 16) | ((uint32_t)chunkX & 0x80000000u);
	if(chunkZ < 0) key |= 0x8000u;
	return (int32_t)key;
}

struct LevelChunk* RD132211LevelSource::getChunk(int32_t chunkX, int32_t chunkZ) {
	int32_t key = rd_chunkKey(chunkX, chunkZ);
	std::map<int32_t, struct LevelChunk*>::iterator it = this->chunkMap.find(key);
	if(it != this->chunkMap.end()) {
		return it->second;
	}

	/*
	 * Port of rd-132211 Level.java:
	 *     for(int x = 0; x < w; ++x)
	 *         for(int y = 0; y < d; ++y)          // d = 64 (vertical)
	 *             for(int z = 0; z < h; ++z)
	 *                 blocks[i] = (byte)(y <= d * 2 / 3 ? 1 : 0);
	 *
	 * MCPE 0.8.1 chunks are 16x16x128 with block index (y | x << 11 | z << 7),
	 * matching the original (x, y, z) loop order below. Only the bottom 64
	 * layers of the rd-132211 world exist, so the terrain is generated with
	 * the exact same shape: solid for y <= 42, air above.
	 */
	uint8_t* blocks = new uint8_t[0x8000u];
	memset(blocks, 0, 0x8000u);

	for(int32_t x = 0; x < 16; ++x) {
		for(int32_t y = 0; y <= RD132211LevelSource::RD_SURFACE_Y; ++y) {
			for(int32_t z = 0; z < 16; ++z) {
				int32_t index = y | (x << 11) | (z << 7);
				if(y == RD132211LevelSource::RD_SURFACE_Y) {
					// Top layer: Grass Block (the old "grass" surface)
					blocks[index] = Tile::grass->blockID;
				} else {
					// The 42 layers below the surface: Cobblestone.
					// Tile::stoneBrick is the decompiled symbol for MCPE 0.8.1
					// Cobblestone (block ID 4, "cobblestone"), the stand-in for
					// rd-132211's rock (Tile.rock).
					blocks[index] = Tile::stoneBrick->blockID;
				}
			}
		}
	}

	struct LevelChunk* chunk = new LevelChunk(this->level, blocks, chunkX, chunkZ);
	chunk->recalcHeightmap();
	this->chunkMap.insert(std::pair<int32_t, struct LevelChunk*>(key, chunk));
	return chunk;
}

struct LevelChunk* RD132211LevelSource::create(int32_t x, int32_t z) {
	return this->getChunk(x, z);
}

void RD132211LevelSource::postProcess(struct ChunkSource*, int32_t, int32_t) {
	// rd-132211 has no decoration pass: no ores, no trees, no flowers,
	// no water springs, no snow and no mob spawning. Terrain is exactly
	// the flat cobblestone slab with its grass cover.
}

bool_t RD132211LevelSource::tick() {
	return 0;
}

bool_t RD132211LevelSource::shouldSave() {
	return 1;
}

std::vector<Biome::MobSpawnerData> RD132211LevelSource::getMobsAt(const struct MobCategory&, int32_t, int32_t, int32_t) {
	// rd-132211 had no mobs at all.
	return std::vector<Biome::MobSpawnerData>();
}

std::string RD132211LevelSource::gatherStats() {
	return "RD132211LevelSource";
}
