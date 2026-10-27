#include <util/TerrainGen.hpp>

/*
 * The generator list of the "Terrain Generation" selector.
 * For now only RD-132211 is available; future ports simply add more entries
 * here and the green arrow buttons start cycling through them.
 */
const TerrainGenEntry TerrainGen::GENERATORS[] = {
		{ TERRAINGEN_ID_RD132211, "RD-132211" },
};

const int32_t TerrainGen::GENERATOR_COUNT = (int32_t)(sizeof(TerrainGen::GENERATORS) / sizeof(TerrainGen::GENERATORS[0]));

int32_t TerrainGen::selected = 0;

int32_t TerrainGen::getSelectedIndex() {
	if(TerrainGen::selected < 0) return 0;
	if(TerrainGen::selected >= TerrainGen::GENERATOR_COUNT) return TerrainGen::GENERATOR_COUNT - 1;
	return TerrainGen::selected;
}

void TerrainGen::setSelectedIndex(int32_t index) {
	if(index < 0) index = 0;
	if(index >= TerrainGen::GENERATOR_COUNT) index = TerrainGen::GENERATOR_COUNT - 1;
	TerrainGen::selected = index;
}

int32_t TerrainGen::getSelectedId() {
	return TerrainGen::GENERATORS[TerrainGen::getSelectedIndex()].id;
}

const char* TerrainGen::getSelectedName() {
	return TerrainGen::getName(TerrainGen::getSelectedIndex());
}

const char* TerrainGen::getName(int32_t index) {
	if(index < 0 || index >= TerrainGen::GENERATOR_COUNT) return "";
	return TerrainGen::GENERATORS[index].name;
}

void TerrainGen::cycle(int32_t dir) {
	int32_t count = TerrainGen::GENERATOR_COUNT;
	if(count <= 1) {
		// Only one generator available (RD-132211 for now): nothing to change
		// to, so the arrows do nothing at all - but the cycling logic itself
		// is already implemented for future terrain generators.
		return;
	}
	int32_t v = TerrainGen::selected + dir;
	while(v < 0) v += count;
	while(v >= count) v -= count;
	TerrainGen::selected = v;
}
