#include "../../../src/geometryHelper/include/CollisionHelper.h"
#include "../../../src/geometryHelper/include/PointHelper.h"
#include <benchmark/benchmark.h>

static void BM_IsPointInRectangle(benchmark::State &state)
{
    CRect rect{0, 0, 1, 1};
    PointDouble point{0.5, 0.5};
    for (auto _ : state)
    {
        auto res = CollisionHelper::IsPointInRectangle(point, rect);
    }
}
BENCHMARK(BM_IsPointInRectangle);

BENCHMARK_MAIN();