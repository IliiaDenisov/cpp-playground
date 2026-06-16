#pragma once
#include "../../../src/geometryHelper/include/PointHelper.h"
#include "../../../src/geometryHelper/include/MathHelper.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;

TEST_CASE("AreEqual basic cases", "[MathHelper]")
{
    CHECK(MathHelper::AreEqual(1.0, 1.0));
    CHECK(MathHelper::AreEqual(1.0, 1.0000001));
    CHECK_FALSE(MathHelper::AreEqual(1.0, 1.1));

    CHECK(MathHelper::AreEqual(0.0, 0.0));
}

TEST_CASE("DotProduct", "[MathHelper]")
{
    PointDouble a{2.0, 3.0};
    PointDouble b{4.0, 5.0};
    // 2*4 + 3*5 = 8 + 15 = 23
    CHECK_THAT(MathHelper::DotProduct(a, b), WithinAbs(23.0, 1e-9));
}

TEST_CASE("CrossProduct ", "[MathHelper]")
{
    PointDouble a{2.0, 3.0};
    PointDouble b{4.0, 5.0};
    CHECK_THAT(MathHelper::CrossProduct(a, b), WithinAbs(2.0, 1e-9));
}

TEST_CASE("NormalizeVector", "[MathHelper]")
{
    PointDouble v{3.0, 4.0};
    MathHelper::NormalizeVector(v);
    CHECK_THAT(v.x, WithinAbs(0.6, 1e-9));
    CHECK_THAT(v.y, WithinAbs(0.8, 1e-9));
    CHECK_THAT(MathHelper::GetVectorLength(v), WithinAbs(1.0, 1e-9));
}

TEST_CASE("NormalizeVector with near-zero vector does nothing", "[MathHelper]")
{
    PointDouble v{0.0, 0.0};
    MathHelper::NormalizeVector(v);
    CHECK_THAT(v.x, WithinAbs(0.0, 1e-12));
    CHECK_THAT(v.y, WithinAbs(0.0, 1e-12));
}

TEST_CASE("GetNormalizedVector returns normalized copy without mutating original", "[MathHelper]")
{
    PointDouble v{3.0, 4.0};
    PointDouble n = MathHelper::GetNormalizedVector(v);

    CHECK_THAT(v.x, WithinAbs(3.0, 1e-9));
    CHECK_THAT(v.y, WithinAbs(4.0, 1e-9));
    CHECK_THAT(n.x, WithinAbs(0.6, 1e-9));
    CHECK_THAT(n.y, WithinAbs(0.8, 1e-9));
}

TEST_CASE("GetNormalVectorToSegment is unit length and perpendicular", "[MathHelper]")
{
    PointDouble start{0.0, 0.0};
    PointDouble end{1.0, 0.0};

    PointDouble normal = MathHelper::GetNormalVectorToSegment(start, end);

    CHECK_THAT(MathHelper::GetVectorLength(normal), WithinAbs(1.0, 1e-9));

    PointDouble segmentVector = end - start;
    CHECK_THAT(MathHelper::DotProduct(normal, segmentVector), WithinAbs(0.0, 1e-9));
}

TEST_CASE("GetNormalVectorFromPointToSegment points toward the base point", "[MathHelper]")
{
    PointDouble start{0.0, 0.0};
    PointDouble end{1.0, 0.0};
    PointDouble base{0.5, 1.0}; // above the segment

    PointDouble normal = MathHelper::GetNormalVectorFromPointToSegment(start, end, base);

    CHECK_THAT(MathHelper::GetVectorLength(normal), WithinAbs(1.0, 1e-9));
    // Normal should point "up" toward base (positive y)
    CHECK(normal.y > 0.0);
}

TEST_CASE("GetNormalVectorFromPointToSegment points toward base on other side", "[MathHelper]")
{
    PointDouble start{0.0, 0.0};
    PointDouble end{1.0, 0.0};
    PointDouble base{0.5, -1.0}; // below the segment

    PointDouble normal = MathHelper::GetNormalVectorFromPointToSegment(start, end, base);

    CHECK_THAT(MathHelper::GetVectorLength(normal), WithinAbs(1.0, 1e-9));
    CHECK(normal.y < 0.0);
}

TEST_CASE("GetDistanceBetweenPoints", "[MathHelper]")
{
    PointDouble p1{0.0, 0.0};
    PointDouble p2{3.0, 4.0};
    CHECK_THAT(MathHelper::GetDistanceBetweenPoints(p1, p2), WithinAbs(5.0, 1e-9));
}

TEST_CASE("GetVectorLength", "[MathHelper]")
{
    PointDouble v{3.0, 4.0};
    CHECK_THAT(MathHelper::GetVectorLength(v), WithinAbs(5.0, 1e-9));

    PointDouble zero{0.0, 0.0};
    CHECK_THAT(MathHelper::GetVectorLength(zero), WithinAbs(0.0, 1e-12));
}

TEST_CASE("GetDistanceToLineFromPoint", "[MathHelper]")
{
    // Horizontal line y = 0, from (0,0) to (1,0)
    PointDouble l1{0.0, 0.0};
    PointDouble l2{1.0, 0.0};
    PointDouble point{5.0, 3.0};

    CHECK_THAT(MathHelper::GetDistanceToLineFromPoint(l1, l2, point), WithinAbs(3.0, 1e-9));
}

TEST_CASE("GetDistanceToLineFromPoint with degenerate line (zero length)", "[MathHelper]")
{
    PointDouble l1{1.0, 1.0};
    PointDouble l2{1.0, 1.0};
    PointDouble point{4.0, 5.0};

    // denominator is 0 -> AreEqual(denominator, 0.0) true -> returns 0.0
    CHECK_THAT(MathHelper::GetDistanceToLineFromPoint(l1, l2, point), WithinAbs(0.0, 1e-12));
}

TEST_CASE("GetDistanceToSegmentFromPoint - projection within segment", "[MathHelper]")
{
    PointDouble s0{0.0, 0.0};
    PointDouble s1{10.0, 0.0};
    PointDouble p{5.0, 3.0};

    CHECK_THAT(MathHelper::GetDistanceToSegmentFromPoint(s0, s1, p), WithinAbs(3.0, 1e-9));
}

TEST_CASE("GetDistanceToSegmentFromPoint - projection clamped to endpoint", "[MathHelper]")
{
    PointDouble s0{0.0, 0.0};
    PointDouble s1{10.0, 0.0};

    // Point beyond s1
    PointDouble pBeyondEnd{15.0, 0.0};
    CHECK_THAT(MathHelper::GetDistanceToSegmentFromPoint(s0, s1, pBeyondEnd), WithinAbs(5.0, 1e-9));

    // Point before s0
    PointDouble pBeforeStart{-5.0, 0.0};
    CHECK_THAT(MathHelper::GetDistanceToSegmentFromPoint(s0, s1, pBeforeStart), WithinAbs(5.0, 1e-9));
}

TEST_CASE("GetDistanceToSegmentFromPoint - degenerate segment (s0 == s1)", "[MathHelper]")
{
    PointDouble s0{2.0, 2.0};
    PointDouble s1{2.0, 2.0};
    PointDouble p{5.0, 6.0};

    CHECK_THAT(MathHelper::GetDistanceToSegmentFromPoint(s0, s1, p), WithinAbs(5.0, 1e-9));
}

TEST_CASE("AngleBetweenVectors", "[MathHelper]")
{
    PointDouble x_axis{1.0, 0.0};
    PointDouble y_axis{0.0, 1.0};

    // Counterclockwise angle from x-axis to y-axis = +PI/2
    CHECK_THAT(MathHelper::AngleBetweenVectors(x_axis, y_axis), WithinAbs(MathHelper::PI / 2.0, 1e-9));

    // Counterclockwise angle from y-axis to x-axis = -PI/2
    CHECK_THAT(MathHelper::AngleBetweenVectors(y_axis, x_axis), WithinAbs(-MathHelper::PI / 2.0, 1e-9));

    // Angle from a vector to itself = 0
    CHECK_THAT(MathHelper::AngleBetweenVectors(x_axis, x_axis), WithinAbs(0.0, 1e-9));
}

TEST_CASE("ToRadians and ToDegrees", "[MathHelper]")
{
    CHECK_THAT(MathHelper::ToRadians(180.0), WithinAbs(MathHelper::PI, 1e-9));
    CHECK_THAT(MathHelper::ToRadians(90.0), WithinAbs(MathHelper::PI / 2.0, 1e-9));
    CHECK_THAT(MathHelper::ToRadians(0.0), WithinAbs(0.0, 1e-9));

    CHECK_THAT(MathHelper::ToDegrees(MathHelper::PI), WithinAbs(180.0, 1e-9));
    CHECK_THAT(MathHelper::ToDegrees(MathHelper::PI / 2.0), WithinAbs(90.0, 1e-9));
    CHECK_THAT(MathHelper::ToDegrees(0.0), WithinAbs(0.0, 1e-9));

    // Round trip
    double deg = 37.5;
    CHECK_THAT(MathHelper::ToDegrees(MathHelper::ToRadians(deg)), WithinAbs(deg, 1e-9));
}

TEST_CASE("NormalizeAngle", "[MathHelper]")
{
    CHECK_THAT(MathHelper::NormalizeAngle(0.0), WithinAbs(0.0, 1e-9));
    CHECK_THAT(MathHelper::NormalizeAngle(MathHelper::PI), WithinAbs(MathHelper::PI, 1e-6));
    CHECK_THAT(MathHelper::NormalizeAngle(-MathHelper::PI), WithinAbs(-MathHelper::PI, 1e-6)); // atan2 wraps to +PI

    // 3*PI should normalize to PI (or -PI, due to atan2 sign at boundary)
    double normalized = MathHelper::NormalizeAngle(3.0 * MathHelper::PI);
    CHECK_THAT(std::abs(normalized), WithinAbs(MathHelper::PI, 1e-6));

    // 2*PI should normalize to ~0
    CHECK_THAT(MathHelper::NormalizeAngle(2.0 * MathHelper::PI), WithinAbs(0.0, 1e-6));

    // 5*PI/2 should normalize to PI/2
    CHECK_THAT(MathHelper::NormalizeAngle(5.0 * MathHelper::PI / 2.0), WithinAbs(MathHelper::PI / 2.0, 1e-6));
}

TEST_CASE("RotateVector by 90 degrees counterclockwise", "[MathHelper]")
{
    PointDouble v{1.0, 0.0};
    MathHelper::RotateVector(v, MathHelper::PI / 2.0);

    CHECK_THAT(v.x, WithinAbs(0.0, 1e-9));
    CHECK_THAT(v.y, WithinAbs(1.0, 1e-9));
}

TEST_CASE("RotateVector by 180 degrees", "[MathHelper]")
{
    PointDouble v{1.0, 0.0};
    MathHelper::RotateVector(v, MathHelper::PI);

    CHECK_THAT(v.x, WithinAbs(-1.0, 1e-9));
    CHECK_THAT(v.y, WithinAbs(0.0, 1e-9));
}

TEST_CASE("RotateVector by 0 degrees does nothing", "[MathHelper]")
{
    PointDouble v{3.0, 4.0};
    MathHelper::RotateVector(v, 0.0);

    CHECK_THAT(v.x, WithinAbs(3.0, 1e-9));
    CHECK_THAT(v.y, WithinAbs(4.0, 1e-9));
}

TEST_CASE("Lerp basic interpolation", "[MathHelper]")
{
    CHECK_THAT(MathHelper::Lerp(0.0, 0.0, 10.0), WithinAbs(0.0, 1e-9));
    CHECK_THAT(MathHelper::Lerp(1.0, 0.0, 10.0), WithinAbs(10.0, 1e-9));
    CHECK_THAT(MathHelper::Lerp(0.5, 0.0, 10.0), WithinAbs(5.0, 1e-9));
    CHECK_THAT(MathHelper::Lerp(0.25, 0.0, 10.0), WithinAbs(2.5, 1e-9));
}

TEST_CASE("Lerp clamps x outside [0,1]", "[MathHelper]")
{
    CHECK_THAT(MathHelper::Lerp(-1.0, 0.0, 10.0), WithinAbs(0.0, 1e-9));
    CHECK_THAT(MathHelper::Lerp(2.0, 0.0, 10.0), WithinAbs(10.0, 1e-9));
}

TEST_CASE("Lerp swaps a and b if a > b", "[MathHelper]")
{
    // a=10, b=0 -> swapped to a=0, b=10
    CHECK_THAT(MathHelper::Lerp(0.5, 10.0, 0.0), WithinAbs(5.0, 1e-9));
}

TEST_CASE("Unlerp basic", "[MathHelper]")
{
    CHECK_THAT(MathHelper::Unlerp(5.0, 0.0, 10.0), WithinAbs(0.5, 1e-9));
    CHECK_THAT(MathHelper::Unlerp(0.0, 0.0, 10.0), WithinAbs(0.0, 1e-9));
    CHECK_THAT(MathHelper::Unlerp(10.0, 0.0, 10.0), WithinAbs(1.0, 1e-9));
}

TEST_CASE("Unlerp clamps x to [a,b]", "[MathHelper]")
{
    CHECK_THAT(MathHelper::Unlerp(-5.0, 0.0, 10.0), WithinAbs(0.0, 1e-9));
    CHECK_THAT(MathHelper::Unlerp(15.0, 0.0, 10.0), WithinAbs(1.0, 1e-9));
}

TEST_CASE("Unlerp swaps a and b if a > b", "[MathHelper]")
{
    // a=10, b=0 -> swapped to a=0,b=10
    CHECK_THAT(MathHelper::Unlerp(5.0, 10.0, 0.0), WithinAbs(0.5, 1e-9));
}

TEST_CASE("Unlerp returns -1 when a == b", "[MathHelper]")
{
    CHECK(MathHelper::Unlerp(5.0, 5.0, 5.0) == -1.0);
}

TEST_CASE("LerpRemap maps value between ranges", "[MathHelper]")
{
    // x=5 in [0,10] -> 0.5 -> mapped to [100,200] -> 150
    CHECK_THAT(MathHelper::LerpRemap(5.0, 0.0, 10.0, 100.0, 200.0), WithinAbs(150.0, 1e-9));

    // edges
    CHECK_THAT(MathHelper::LerpRemap(0.0, 0.0, 10.0, 100.0, 200.0), WithinAbs(100.0, 1e-9));
    CHECK_THAT(MathHelper::LerpRemap(10.0, 0.0, 10.0, 100.0, 200.0), WithinAbs(200.0, 1e-9));
}

TEST_CASE("LerpRemap handles reversed source/target ranges", "[MathHelper]")
{
    // source range reversed: a1=10, a2=0
    CHECK_THAT(MathHelper::LerpRemap(5.0, 10.0, 0.0, 100.0, 200.0), WithinAbs(150.0, 1e-9));

    // target range reversed: b1=200, b2=100
    CHECK_THAT(MathHelper::LerpRemap(5.0, 0.0, 10.0, 200.0, 100.0), WithinAbs(150.0, 1e-9));
}

TEST_CASE("LerpRemap clamps out-of-range x", "[MathHelper]")
{
    CHECK_THAT(MathHelper::LerpRemap(-5.0, 0.0, 10.0, 100.0, 200.0), WithinAbs(100.0, 1e-9));
    CHECK_THAT(MathHelper::LerpRemap(15.0, 0.0, 10.0, 100.0, 200.0), WithinAbs(200.0, 1e-9));
}
