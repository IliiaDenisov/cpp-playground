#pragma once
#include "../../../src/geometryHelper/include/CurveApproximationHelper.h"
#include "../../../src/geometryHelper/include/MathHelper.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;

// ─────────────────────────────────────────────────────────────
// GetBezierCurvesDrawingStructure
// ─────────────────────────────────────────────────────────────

TEST_CASE("GetBezierCurvesDrawingStructure - output size", "[CurveApproximationHelper]")
{
    // n endPoints, n-1 control pairs → total = n + 2*(n-1) = 3n-2
    std::vector<PointDouble> endPoints   = {{0,0}, {5,5}, {10,0}};
    std::vector<PointDouble> cp1         = {{1,3}, {6,3}};
    std::vector<PointDouble> cp2         = {{4,3}, {9,3}};

    auto result = CurveAppoximationHelper::GetBezierCurvesDrawingStructure(endPoints, cp1, cp2);

    // 3 endPoints + 2 cp1 + 2 cp2 = 7
    CHECK(result.size() == 7);
}

TEST_CASE("GetBezierCurvesDrawingStructure - layout: endPoints at indices 0,3,6,...", "[CurveApproximationHelper]")
{
    std::vector<PointDouble> endPoints = {{0,0}, {5,5}, {10,0}};
    std::vector<PointDouble> cp1       = {{1,3}, {6,3}};
    std::vector<PointDouble> cp2       = {{4,3}, {9,3}};

    auto result = CurveAppoximationHelper::GetBezierCurvesDrawingStructure(endPoints, cp1, cp2);

    // endPoints sit at positions 0, 3, 6
    CHECK_THAT(result[0].x, WithinAbs(0.0, 1e-9));
    CHECK_THAT(result[0].y, WithinAbs(0.0, 1e-9));

    CHECK_THAT(result[3].x, WithinAbs(5.0, 1e-9));
    CHECK_THAT(result[3].y, WithinAbs(5.0, 1e-9));

    CHECK_THAT(result[6].x, WithinAbs(10.0, 1e-9));
    CHECK_THAT(result[6].y, WithinAbs(0.0, 1e-9));
}

TEST_CASE("GetBezierCurvesDrawingStructure - layout: control points at indices 1,2,4,5,...", "[CurveApproximationHelper]")
{
    std::vector<PointDouble> endPoints = {{0,0}, {5,5}, {10,0}};
    std::vector<PointDouble> cp1       = {{1,3}, {6,3}};
    std::vector<PointDouble> cp2       = {{4,3}, {9,3}};

    auto result = CurveAppoximationHelper::GetBezierCurvesDrawingStructure(endPoints, cp1, cp2);

    // first control pair sits at 1, 2
    CHECK_THAT(result[1].x, WithinAbs(1.0, 1e-9));
    CHECK_THAT(result[2].x, WithinAbs(4.0, 1e-9));

    // second control pair sits at 4, 5
    CHECK_THAT(result[4].x, WithinAbs(6.0, 1e-9));
    CHECK_THAT(result[5].x, WithinAbs(9.0, 1e-9));
}

TEST_CASE("GetBezierCurvesDrawingStructure - single segment (2 endpoints, 1 control pair)", "[CurveApproximationHelper]")
{
    std::vector<PointDouble> endPoints = {{0,0}, {10,0}};
    std::vector<PointDouble> cp1       = {{3,5}};
    std::vector<PointDouble> cp2       = {{7,5}};

    auto result = CurveAppoximationHelper::GetBezierCurvesDrawingStructure(endPoints, cp1, cp2);

    // 2 + 1 + 1 = 4 elements
    CHECK(result.size() == 4);
    CHECK_THAT(result[0].x, WithinAbs(0.0,  1e-9));
    CHECK_THAT(result[1].x, WithinAbs(3.0,  1e-9));
    CHECK_THAT(result[2].x, WithinAbs(7.0,  1e-9));
    CHECK_THAT(result[3].x, WithinAbs(10.0, 1e-9));
}

// ─────────────────────────────────────────────────────────────
// GetCubicBezierCurveDrawingPoints
// ─────────────────────────────────────────────────────────────

TEST_CASE("GetCubicBezierCurveDrawingPoints - result contains 3 endpoints", "[CurveApproximationHelper]")
{
    PointDouble p1{0,0}, p2{5,5}, p3{10,0};
    auto info = CurveAppoximationHelper::GetCubicBezierCurveDrawingPoints(p1, p2, p3);

    REQUIRE(info.endPoints.size() == 3);
    REQUIRE(info.controlPoints1.size() == 2);
    REQUIRE(info.controlPoints2.size() == 2);
}

TEST_CASE("GetCubicBezierCurveDrawingPoints - endpoints are exactly p1, p2, p3", "[CurveApproximationHelper]")
{
    PointDouble p1{0,0}, p2{5,5}, p3{10,0};
    auto info = CurveAppoximationHelper::GetCubicBezierCurveDrawingPoints(p1, p2, p3);

    CHECK_THAT(info.endPoints[0].x, WithinAbs(0.0, 1e-9));
    CHECK_THAT(info.endPoints[0].y, WithinAbs(0.0, 1e-9));
    CHECK_THAT(info.endPoints[1].x, WithinAbs(5.0, 1e-9));
    CHECK_THAT(info.endPoints[1].y, WithinAbs(5.0, 1e-9));
    CHECK_THAT(info.endPoints[2].x, WithinAbs(10.0, 1e-9));
    CHECK_THAT(info.endPoints[2].y, WithinAbs(0.0, 1e-9));
}

TEST_CASE("GetCubicBezierCurveDrawingPoints - control points derived from formula", "[CurveApproximationHelper]")
{
    // Verify against manually computed values
    // dir = p1 - p3 = (0,0) - (10,0) = (-10, 0)
    // cp1 = p2 + dir * 0.25 = (5,5) + (-2.5, 0) = (2.5, 5)
    // cp2 = p2 - dir * 0.25 = (5,5) + (2.5, 0)  = (7.5, 5)
    // controlPoint0_0 = p1 + (cp1-p1)*2/3 = (0,0) + (2.5,5)*2/3 = (5/3, 10/3)
    // controlPoint1_0 = p2 + (cp1-p2)*2/3 = (5,5) + (-2.5,0)*2/3 = (5 - 5/3, 5) = (10/3, 5)
    // controlPoint0_1 = p2 + (cp2-p2)*2/3 = (5,5) + (2.5,0)*2/3  = (5 + 5/3, 5) = (20/3, 5)
    // controlPoint1_1 = p3 + (cp2-p3)*2/3 = (10,0) + (-2.5,5)*2/3 = (10 - 5/3, 10/3)

    PointDouble p1{0,0}, p2{5,5}, p3{10,0};
    auto info = CurveAppoximationHelper::GetCubicBezierCurveDrawingPoints(p1, p2, p3);

    CHECK_THAT(info.controlPoints1[0].x, WithinAbs(5.0/3.0,  1e-9));
    CHECK_THAT(info.controlPoints1[0].y, WithinAbs(10.0/3.0, 1e-9));

    CHECK_THAT(info.controlPoints2[0].x, WithinAbs(10.0/3.0, 1e-9));
    CHECK_THAT(info.controlPoints2[0].y, WithinAbs(5.0,      1e-9));

    CHECK_THAT(info.controlPoints1[1].x, WithinAbs(20.0/3.0, 1e-9));
    CHECK_THAT(info.controlPoints1[1].y, WithinAbs(5.0,      1e-9));

    CHECK_THAT(info.controlPoints2[1].x, WithinAbs(10.0 - 5.0/3.0, 1e-9));
    CHECK_THAT(info.controlPoints2[1].y, WithinAbs(10.0/3.0,       1e-9));
}

TEST_CASE("GetCubicBezierCurveDrawingPoints - collinear points", "[CurveApproximationHelper]")
{
    // All three points on the x-axis; curve should be flat
    PointDouble p1{0,0}, p2{5,0}, p3{10,0};
    auto info = CurveAppoximationHelper::GetCubicBezierCurveDrawingPoints(p1, p2, p3);

    for (const auto& cp : info.controlPoints1)
        CHECK_THAT(cp.y, WithinAbs(0.0, 1e-9));
    for (const auto& cp : info.controlPoints2)
        CHECK_THAT(cp.y, WithinAbs(0.0, 1e-9));
}

// ─────────────────────────────────────────────────────────────
// GetCubicBezierSineWaveApproximation
// ─────────────────────────────────────────────────────────────

TEST_CASE("GetCubicBezierSineWaveApproximation - structure sizes", "[CurveApproximationHelper]")
{
    auto info = CurveAppoximationHelper::GetCubicBezierSineWaveApproximation({0,0}, 100.0, 10.0);

    CHECK(info.endPoints.size()     == 2);
    CHECK(info.controlPoints1.size() == 1);
    CHECK(info.controlPoints2.size() == 1);
}

TEST_CASE("GetCubicBezierSineWaveApproximation - endpoints at waveStart and waveStart+waveLength", "[CurveApproximationHelper]")
{
    PointDouble start{3.0, 7.0};
    double waveLength = 200.0;
    auto info = CurveAppoximationHelper::GetCubicBezierSineWaveApproximation(start, waveLength, 10.0);

    CHECK_THAT(info.endPoints[0].x, WithinAbs(3.0,   1e-9));
    CHECK_THAT(info.endPoints[0].y, WithinAbs(7.0,   1e-9));
    CHECK_THAT(info.endPoints[1].x, WithinAbs(203.0, 1e-9));
    CHECK_THAT(info.endPoints[1].y, WithinAbs(7.0,   1e-9));
}

TEST_CASE("GetCubicBezierSineWaveApproximation - control points are symmetric about wave midpoint", "[CurveApproximationHelper]")
{
    PointDouble start{0,0};
    double waveLength = 100.0, amplitude = 15.0;
    auto info = CurveAppoximationHelper::GetCubicBezierSineWaveApproximation(start, waveLength, amplitude);

    const auto& cp1 = info.controlPoints1[0];
    const auto& cp2 = info.controlPoints2[0];

    // cp1.x + cp2.x should equal waveLength (symmetric offsets from each end)
    CHECK_THAT(cp1.x + cp2.x, WithinAbs(waveLength, 1e-6));

    // cp1 and cp2 y-offsets from the wave baseline should be equal in magnitude, opposite in sign
    CHECK_THAT(cp1.y + cp2.y, WithinAbs(0.0, 1e-6));
}

TEST_CASE("GetCubicBezierSineWaveApproximation - control point offsets scale with amplitude", "[CurveApproximationHelper]")
{
    PointDouble start{0,0};
    double waveLength = 100.0;

    auto info1 = CurveAppoximationHelper::GetCubicBezierSineWaveApproximation(start, waveLength, 10.0);
    auto info2 = CurveAppoximationHelper::GetCubicBezierSineWaveApproximation(start, waveLength, 20.0);

    // doubling the amplitude should double the y-offset of control points
    double yOffset1 = std::abs(info1.controlPoints1[0].y - start.y);
    double yOffset2 = std::abs(info2.controlPoints1[0].y - start.y);
    CHECK_THAT(yOffset2 / yOffset1, WithinAbs(2.0, 1e-6));
}

TEST_CASE("GetCubicBezierSineWaveApproximation - zero amplitude gives flat wave", "[CurveApproximationHelper]")
{
    auto info = CurveAppoximationHelper::GetCubicBezierSineWaveApproximation({0,0}, 100.0, 0.0);

    CHECK_THAT(info.controlPoints1[0].y, WithinAbs(0.0, 1e-9));
    CHECK_THAT(info.controlPoints2[0].y, WithinAbs(0.0, 1e-9));
}

// ─────────────────────────────────────────────────────────────
// ApproximateQuadraticBezierCurveWithPolyline
// ─────────────────────────────────────────────────────────────

TEST_CASE("ApproximateQuadraticBezierCurveWithPolyline - output clamped to [25, 100] steps", "[CurveApproximationHelper]")
{
    // Very short curve -> minimum 25 steps
    auto pointsShort = CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(
        {0,0}, {1,0}, {0.5, 0.1});
    CHECK(pointsShort.size() >= 25);

    // Very long curve -> capped at 100 steps
    auto pointsLong = CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(
        {0,0}, {10000,0}, {5000, 5000});
    CHECK(pointsLong.size() <= 100);
}

TEST_CASE("ApproximateQuadraticBezierCurveWithPolyline - starts at point1", "[CurveApproximationHelper]")
{
    PointDouble p1{2.0, 3.0}, p2{10.0, 3.0}, cp{6.0, 8.0};
    auto points = CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(p1, p2, cp);

    // t=0: (1-0)^2*p1 + 0 + 0 = p1
    CHECK_THAT(points.front().x, WithinAbs(2.0, 1e-9));
    CHECK_THAT(points.front().y, WithinAbs(3.0, 1e-9));
}

TEST_CASE("ApproximateQuadraticBezierCurveWithPolyline - does not include t=1 endpoint", "[CurveApproximationHelper]")
{
    // Loop runs t in [0, 1) so p2 is never exactly reached
    PointDouble p1{0,0}, p2{10,0}, cp{5,5};
    auto points = CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(p1, p2, cp);

    bool endpointFound = std::any_of(points.begin(), points.end(), [&](const PointDouble& p)
    {
        return std::abs(p.x - p2.x) < 1e-6 && std::abs(p.y - p2.y) < 1e-6;
    });
    CHECK_FALSE(endpointFound);
}

TEST_CASE("ApproximateQuadraticBezierCurveWithPolyline - collinear control point gives straight line", "[CurveApproximationHelper]")
{
    // Control point on the line between p1 and p2 -> curve is a straight line
    PointDouble p1{0,0}, p2{10,0}, cp{5,0};
    auto points = CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(p1, p2, cp);

    for (const auto& p : points)
        CHECK_THAT(p.y, WithinAbs(0.0, 1e-9));
}

TEST_CASE("ApproximateQuadraticBezierCurveWithPolyline - midpoint near control point", "[CurveApproximationHelper]")
{
    // At t=0.5, quadratic Bezier midpoint = 0.25*p1 + 0.5*cp + 0.25*p2
    PointDouble p1{0,0}, p2{10,0}, cp{5,10};
    auto points = CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(p1, p2, cp);

    PointDouble expectedMid{ 0.25*0 + 0.5*5 + 0.25*10, 0.25*0 + 0.5*10 + 0.25*0 };  // {5, 5}

    // Find the point closest to t=0.5 midpoint
    auto closest = std::min_element(points.begin(), points.end(), [&](const PointDouble& a, const PointDouble& b)
    {
        double da = (a.x - expectedMid.x)*(a.x - expectedMid.x) + (a.y - expectedMid.y)*(a.y - expectedMid.y);
        double db = (b.x - expectedMid.x)*(b.x - expectedMid.x) + (b.y - expectedMid.y)*(b.y - expectedMid.y);
        return da < db;
    });

    CHECK_THAT(closest->x, WithinAbs(5.0, 0.5));
    CHECK_THAT(closest->y, WithinAbs(5.0, 0.5));
}

// ─────────────────────────────────────────────────────────────
// ApplyChaikinAlgorithm (PointInteger)
// ─────────────────────────────────────────────────────────────

TEST_CASE("ApplyChaikinAlgorithm (int) - degree 0 returns input unchanged", "[CurveApproximationHelper]")
{
    std::vector<PointInteger> input = {{0,0}, {10,10}, {20,0}};
    auto result = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 0);

    REQUIRE(result.size() == input.size());
    for (size_t i = 0; i < input.size(); i++)
    {
        CHECK(result[i].x == input[i].x);
        CHECK(result[i].y == input[i].y);
    }
}

TEST_CASE("ApplyChaikinAlgorithm (int) - single point returns input unchanged", "[CurveApproximationHelper]")
{
    std::vector<PointInteger> input = {{5, 5}};
    auto result = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 3);

    REQUIRE(result.size() == 1);
    CHECK(result[0].x == 5);
    CHECK(result[0].y == 5);
}

TEST_CASE("ApplyChaikinAlgorithm (int) - degree 1 output size for n points is 2*(n-1)+2 = 2n", "[CurveApproximationHelper]")
{
    // n=3 -> 2*3 = 6
    std::vector<PointInteger> input = {{0,0}, {10,0}, {20,0}};
    auto result = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 1);

    CHECK(result.size() == 2 * input.size());
}

TEST_CASE("ApplyChaikinAlgorithm (int) - degree 1 preserves first and last points", "[CurveApproximationHelper]")
{
    std::vector<PointInteger> input = {{0,0}, {10,10}, {20,0}};
    auto result = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 1);

    CHECK(result.front().x == 0);
    CHECK(result.front().y == 0);
    CHECK(result.back().x  == 20);
    CHECK(result.back().y  == 0);
}

TEST_CASE("ApplyChaikinAlgorithm (int) - degree 1 intermediate points follow 3/4 and 1/4 rule", "[CurveApproximationHelper]")
{
    std::vector<PointInteger> input = {{0,0}, {8,8}};
    auto result = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 1);

    // Between input[0] and input[1]:
    // point_0 = 0.75*(0,0) + 0.25*(8,8) = (2, 2)
    // point_1 = 0.25*(0,0) + 0.75*(8,8) = (6, 6)
    REQUIRE(result.size() == 4);
    CHECK(result[0].x == 0);  CHECK(result[0].y == 0);
    CHECK(result[1].x == 2);  CHECK(result[1].y == 2);
    CHECK(result[2].x == 6);  CHECK(result[2].y == 6);
    CHECK(result[3].x == 8);  CHECK(result[3].y == 8);
}

TEST_CASE("ApplyChaikinAlgorithm (int) - higher degrees produce more points", "[CurveApproximationHelper]")
{
    std::vector<PointInteger> input = {{0,0}, {10,10}, {20,0}};
    auto result1 = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 1);
    auto result2 = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 2);
    auto result3 = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 3);

    CHECK(result2.size() > result1.size());
    CHECK(result3.size() > result2.size());
}

// ─────────────────────────────────────────────────────────────
// ApplyChaikinAlgorithm (PointFloat)
// ─────────────────────────────────────────────────────────────

TEST_CASE("ApplyChaikinAlgorithm (float) - degree 0 returns input unchanged", "[CurveApproximationHelper]")
{
    std::vector<PointFloat> input = {{0.f,0.f}, {10.f,10.f}, {20.f,0.f}};
    auto result = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 0);

    REQUIRE(result.size() == input.size());
    for (size_t i = 0; i < input.size(); i++)
    {
        CHECK(result[i].X == input[i].X);
        CHECK(result[i].Y == input[i].Y);
    }
}

TEST_CASE("ApplyChaikinAlgorithm (float) - degree 1 preserves first and last points", "[CurveApproximationHelper]")
{
    std::vector<PointFloat> input = {{0.f,0.f}, {10.f,10.f}, {20.f,0.f}};
    auto result = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 1);

    CHECK(result.front().X == 0.f);
    CHECK(result.front().Y == 0.f);
    CHECK(result.back().X  == 20.f);
    CHECK(result.back().Y  == 0.f);
}

TEST_CASE("ApplyChaikinAlgorithm (float) - degree 1 intermediate points follow 3/4 and 1/4 rule (float)", "[CurveApproximationHelper]")
{
    std::vector<PointFloat> input = {{0.f,0.f}, {8.f,8.f}};
    auto result = CurveAppoximationHelper::ApplyChaikinAlgorithm(input, 1);

    REQUIRE(result.size() == 4);
    CHECK_THAT(result[1].X, WithinAbs(2.f, 1e-5f));
    CHECK_THAT(result[1].Y, WithinAbs(2.f, 1e-5f));
    CHECK_THAT(result[2].X, WithinAbs(6.f, 1e-5f));
    CHECK_THAT(result[2].Y, WithinAbs(6.f, 1e-5f));
}

TEST_CASE("ApplyChaikinAlgorithm (float) - float version preserves more precision than int version", "[CurveApproximationHelper]")
{
    // Use odd coordinates where integer truncation loses info
    std::vector<PointInteger> inputInt   = {{0,0}, {3,3}};
    std::vector<PointFloat>   inputFloat = {{0.f,0.f}, {3.f,3.f}};

    auto resultInt   = CurveAppoximationHelper::ApplyChaikinAlgorithm(inputInt, 1);
    auto resultFloat = CurveAppoximationHelper::ApplyChaikinAlgorithm(inputFloat, 1);

    // 0.75*0 + 0.25*3 = 0.75, truncated to 0 in int, preserved in float
    CHECK(resultInt[1].x == 0);          // truncation
    CHECK_THAT(resultFloat[1].X, WithinAbs(0.75f, 1e-5f));  // no truncation
}