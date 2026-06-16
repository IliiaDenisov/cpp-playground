#include "../../../src/geometryHelper/include/MathHelper.h"
#include "../../../src/geometryHelper/include/GeometryHelper.h"
#include "../../../src/geometryHelper/include/CurveApproximationHelper.h"
#include "../../../src/geometryHelper/include/CollisionHelper.h"
#include <benchmark/benchmark.h>

static void BM_GetDistanceBetweenPoints(benchmark::State& state)
{
    PointDouble p1{1.0, 2.0}, p2{4.0, 6.0};
    for (auto _ : state)
        benchmark::DoNotOptimize(MathHelper::GetDistanceBetweenPoints(p1, p2));
}
BENCHMARK(BM_GetDistanceBetweenPoints);

static void BM_NormalizeVector(benchmark::State& state)
{
    for (auto _ : state)
    {
        PointDouble v{3.0, 4.0};
        MathHelper::NormalizeVector(v);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(BM_NormalizeVector);

static void BM_ApproximateQuadraticBezier_Short(benchmark::State& state)
{
    PointDouble p1{0,0}, p2{1,0}, cp{0.5, 0.1};
    for (auto _ : state)
        benchmark::DoNotOptimize(
            CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(p1, p2, cp));
}
BENCHMARK(BM_ApproximateQuadraticBezier_Short);

static void BM_ApproximateQuadraticBezier_Long(benchmark::State& state)
{
    PointDouble p1{0,0}, p2{10000,0}, cp{5000, 5000};
    for (auto _ : state)
        benchmark::DoNotOptimize(
            CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(p1, p2, cp));
}
BENCHMARK(BM_ApproximateQuadraticBezier_Long);

static void BM_ChaikinInteger(benchmark::State& state)
{
    const int degree = state.range(0);
    std::vector<PointInteger> input = {{0,0},{10,5},{20,0},{30,10},{40,0}};
    for (auto _ : state)
        benchmark::DoNotOptimize(
            CurveAppoximationHelper::ApplyChaikinAlgorithm(input, degree));
}
BENCHMARK(BM_ChaikinInteger)->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5);

static void BM_ChaikinFloat(benchmark::State& state)
{
    const int degree = state.range(0);
    std::vector<PointFloat> input = {{0,0},{10,5},{20,0},{30,10},{40,0}};
    for (auto _ : state)
        benchmark::DoNotOptimize(
            CurveAppoximationHelper::ApplyChaikinAlgorithm(input, degree));
}
BENCHMARK(BM_ChaikinFloat)->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5);

static void BM_GetBezierCurvesDrawingStructure(benchmark::State& state)
{
    const int n = state.range(0);
    std::vector<PointDouble> endPoints(n), cp1(n-1), cp2(n-1);
    for (int i = 0; i < n; i++)   endPoints[i] = {(double)i, (double)i};
    for (int i = 0; i < n-1; i++) cp1[i] = cp2[i] = {(double)i + 0.5, (double)i + 0.5};

    for (auto _ : state)
        benchmark::DoNotOptimize(
            CurveAppoximationHelper::GetBezierCurvesDrawingStructure(endPoints, cp1, cp2));
}
BENCHMARK(BM_GetBezierCurvesDrawingStructure)->Arg(4)->Arg(16)->Arg(64)->Arg(256);

static void BM_HitTestRayToRect(benchmark::State& state)
{
    CRect box; box.left=0; box.top=0; box.right=100; box.bottom=100;
    PointDouble origin{50, 50}, dir{100, 50};
    std::vector<PointDouble> hits;
    for (auto _ : state)
    {
        hits.clear();
        benchmark::DoNotOptimize(
            CollisionHelper::HitTestRayToRect(origin, dir, box, hits));
    }
}
BENCHMARK(BM_HitTestRayToRect);

static void BM_IsPointInPolygon(benchmark::State& state)
{
    const int n = state.range(0);
    std::vector<PointDouble> polygon(n);
    for (int i = 0; i < n; i++)
    {
        double angle = 2.0 * MathHelper::PI * i / n;
        polygon[i] = {50.0 + 40.0 * std::cos(angle), 50.0 + 40.0 * std::sin(angle)};
    }
    PointDouble point{50.0, 50.0};
    for (auto _ : state)
        benchmark::DoNotOptimize(CollisionHelper::IsPointInPolygon(point, polygon));
}
BENCHMARK(BM_IsPointInPolygon)->Arg(4)->Arg(16)->Arg(64)->Arg(256);

static void BM_HitTestQuadraticBezierCurve_Hit(benchmark::State& state)
{
    PointDouble target{5, 5}, p1{0,0}, p2{10,0}, cp{5,10};
    for (auto _ : state)
        benchmark::DoNotOptimize(
            CollisionHelper::HitTestQuadraticBezierCurve(target, p1, p2, cp, 1.0));
}
BENCHMARK(BM_HitTestQuadraticBezierCurve_Hit);

static void BM_HitTestQuadraticBezierCurve_Miss(benchmark::State& state)
{
    PointDouble target{50, 50}, p1{0,0}, p2{10,0}, cp{5,10};
    for (auto _ : state)
        benchmark::DoNotOptimize(
            CollisionHelper::HitTestQuadraticBezierCurve(target, p1, p2, cp, 1.0));
}
BENCHMARK(BM_HitTestQuadraticBezierCurve_Miss);

static void BM_GetFanFillPoints_OriginInside(benchmark::State& state)
{
    CRect box; box.left=0; box.top=0; box.right=100; box.bottom=100;
    PointDouble origin{50,50}, dir1{100,50}, dir2{50,100};
    for (auto _ : state)
        benchmark::DoNotOptimize(
            GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box));
}
BENCHMARK(BM_GetFanFillPoints_OriginInside);

static void BM_GetFanFillPoints_OriginOutside(benchmark::State& state)
{
    CRect box; box.left=0; box.top=0; box.right=100; box.bottom=100;
    PointDouble origin{-20,50}, dir1{0,0}, dir2{0,100};
    for (auto _ : state)
        benchmark::DoNotOptimize(
            GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box));
}
BENCHMARK(BM_GetFanFillPoints_OriginOutside);

static void BM_FindClosestRadius_AngleStep(benchmark::State& state)
{
    const double step = state.range(0);
    PointDouble center{0,0}, input{3,4};
    for (auto _ : state)
        benchmark::DoNotOptimize(
            GeometryHelper::FindClosestRadiusToPoint(center, input, step));
}
BENCHMARK(BM_FindClosestRadius_AngleStep)->Arg(45)->Arg(10)->Arg(5)->Arg(1);

static void BM_FindClosestRadius_AngleOptions(benchmark::State& state)
{
    const int n = state.range(0);
    std::vector<double> options(n);
    for (int i = 0; i < n; i++) options[i] = 360.0 * i / n;
    PointDouble center{0,0}, input{3,4};
    for (auto _ : state)
        benchmark::DoNotOptimize(
            GeometryHelper::FindClosestRadiusToPoint(center, input, options));
}
BENCHMARK(BM_FindClosestRadius_AngleOptions)->Arg(8)->Arg(36)->Arg(72)->Arg(360);

BENCHMARK_MAIN();