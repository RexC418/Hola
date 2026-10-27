#include <_types.h>

struct LevelSettings
{
        int32_t seed, gameType;
        // Terrain generator to use for a new world:
        //   0 = vanilla MCPE 0.8.1 generation
        //   1 = rd-132211 (Minecraft Pre-Classic) generation (see RD132211LevelSource)
        // The value is carried into LevelData (and persisted in the level.dat
        // NBT tag "GeneratorVersion") so every world keeps its own generator.
        int32_t generatorVersion;

        LevelSettings(int seed, int gt, int genVer = 0) : seed(seed), gameType(gt), generatorVersion(genVer){

        }
};
