#pragma once
#include "../../../src/geometryHelper/include/CollisionHelper.h"
#include "../../../src/geometryHelper/include/PointHelper.h"
#include "../../../src/geometryHelper/include/MathHelper.h"
#include "../../../src/geometryHelper/include/GeometryHelper.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;

TEST_CASE("FindPointsCentroid - basic average", "[GeometryHelper]")
{
    std::vector<PointDouble> points = {
        {0.0, 0.0},
        {10.0, 0.0},
        {0.0, 10.0},
        {10.0, 10.0}
    };

    PointDouble centroid = GeometryHelper::FindPointsCentroid(points);

    CHECK_THAT(centroid.x, WithinAbs(5.0, 1e-9));
    CHECK_THAT(centroid.y, WithinAbs(5.0, 1e-9));
}

TEST_CASE("FindPointsCentroid - single point", "[GeometryHelper]")
{
    std::vector<PointDouble> points = { {3.0, 7.0} };

    PointDouble centroid = GeometryHelper::FindPointsCentroid(points);

    CHECK_THAT(centroid.x, WithinAbs(3.0, 1e-9));
    CHECK_THAT(centroid.y, WithinAbs(7.0, 1e-9));
}

TEST_CASE("FindPointsCentroid - empty vector returns origin", "[GeometryHelper]")
{
    std::vector<PointDouble> points;

    PointDouble centroid = GeometryHelper::FindPointsCentroid(points);

    CHECK_THAT(centroid.x, WithinAbs(0.0, 1e-12));
    CHECK_THAT(centroid.y, WithinAbs(0.0, 1e-12));
}

TEST_CASE("FindClosestRadiusToPoint (angle step) - point on axis", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    PointDouble inputPoint{5.0, 0.001}; // close to +x axis, slightly off

    PointDouble result = GeometryHelper::FindClosestRadiusToPoint(center, inputPoint, 45.0);

    // distance to center should equal distance from inputPoint to center
    double expectedRadius = MathHelper::GetDistanceBetweenPoints(inputPoint, center);
    CHECK_THAT(MathHelper::GetDistanceBetweenPoints(result, center), WithinAbs(expectedRadius, 1e-9));

    // Closest discretized direction (0, 45, 90...) to a point near +x axis should be (radius, 0)
    CHECK_THAT(result.x, WithinAbs(expectedRadius, 1e-6));
    CHECK_THAT(result.y, WithinAbs(0.0, 1e-6));
}

TEST_CASE("FindClosestRadiusToPoint (angle step) - point near 90 degrees", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    PointDouble inputPoint{0.001, 5.0}; // close to +y axis

    PointDouble result = GeometryHelper::FindClosestRadiusToPoint(center, inputPoint, 45.0);

    double expectedRadius = MathHelper::GetDistanceBetweenPoints(inputPoint, center);

    CHECK_THAT(result.x, WithinAbs(0.0, 1e-6));
    CHECK_THAT(result.y, WithinAbs(expectedRadius, 1e-6));
}

TEST_CASE("FindClosestRadiusToPoint (angle step) - point at 45 degrees with 90-degree step", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    double radius = 10.0;
    // place input point exactly at 45 degrees
    PointDouble inputPoint{ radius * std::cos(MathHelper::ToRadians(45.0)), radius * std::sin(MathHelper::ToRadians(45.0)) };

    // angle step of 90 -> options are 0, 90, 180, 270
    // both 0 and 90 are equidistant from a point at 45 degrees, so result should be one of them
    PointDouble result = GeometryHelper::FindClosestRadiusToPoint(center, inputPoint, 90.0);

    bool isZeroDegrees = (std::abs(result.x - radius) < 1e-6) && (std::abs(result.y - 0.0) < 1e-6);
    bool isNinetyDegrees = (std::abs(result.x - 0.0) < 1e-6) && (std::abs(result.y - radius) < 1e-6);

    CHECK((isZeroDegrees || isNinetyDegrees));
}

TEST_CASE("FindClosestRadiusToPoint (angle step) - offset rotation center", "[GeometryHelper]")
{
    PointDouble center{100.0, 50.0};
    PointDouble inputPoint{110.0, 50.0}; // 10 units to the right of center, along 0 degrees

    PointDouble result = GeometryHelper::FindClosestRadiusToPoint(center, inputPoint, 45.0);

    CHECK_THAT(result.x, WithinAbs(110.0, 1e-6));
    CHECK_THAT(result.y, WithinAbs(50.0, 1e-6));
}

TEST_CASE("FindClosestRadiusToPoint (angle options) - exact match", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    double radius = 5.0;
    PointDouble inputPoint{ radius * std::cos(MathHelper::ToRadians(30.0)), radius * std::sin(MathHelper::ToRadians(30.0)) };

    std::vector<double> angleOptions = { 0.0, 30.0, 60.0, 90.0 };

    PointDouble result = GeometryHelper::FindClosestRadiusToPoint(center, inputPoint, angleOptions);

    CHECK_THAT(result.x, WithinAbs(inputPoint.x, 1e-6));
    CHECK_THAT(result.y, WithinAbs(inputPoint.y, 1e-6));
}

TEST_CASE("FindClosestRadiusToPoint (angle options) - picks nearest of non-uniform options", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    double radius = 5.0;
    // point near 80 degrees
    PointDouble inputPoint{ radius * std::cos(MathHelper::ToRadians(80.0)), radius * std::sin(MathHelper::ToRadians(80.0)) };

    std::vector<double> angleOptions = { 0.0, 45.0, 90.0, 135.0 };

    PointDouble result = GeometryHelper::FindClosestRadiusToPoint(center, inputPoint, angleOptions);

    // expect the 90-degree option (radius, along +y) to be closest
    CHECK_THAT(result.x, WithinAbs(0.0, 1e-6));
    CHECK_THAT(result.y, WithinAbs(radius, 1e-6));
}

TEST_CASE("FindClosestRadiusToPoint (angle options) - empty options returns origin", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    PointDouble inputPoint{5.0, 5.0};
    std::vector<double> angleOptions; // empty

    PointDouble result = GeometryHelper::FindClosestRadiusToPoint(center, inputPoint, angleOptions);

    CHECK_THAT(result.x, WithinAbs(0.0, 1e-12));
    CHECK_THAT(result.y, WithinAbs(0.0, 1e-12));
}

TEST_CASE("FindClosestRadiusToPoint (angle options) - offset center", "[GeometryHelper]")
{
    PointDouble center{20.0, 20.0};
    double radius = 5.0;
    PointDouble inputPoint{ center.x + radius, center.y }; // along 0 degrees relative to center

    std::vector<double> angleOptions = { 0.0, 90.0, 180.0, 270.0 };

    PointDouble result = GeometryHelper::FindClosestRadiusToPoint(center, inputPoint, angleOptions);

    CHECK_THAT(result.x, WithinAbs(inputPoint.x, 1e-6));
    CHECK_THAT(result.y, WithinAbs(inputPoint.y, 1e-6));
}

TEST_CASE("GetFanFillPoints - origin inside box, both directions hit box edges", "[GeometryHelper]")
{
    CRect box;
    box.left = 0.0;
    box.top = 0.0;
    box.right = 100.0;
    box.bottom = 100.0;

    PointDouble origin{50.0, 50.0};
    // direction 1 points along +x toward right edge
    PointDouble dir1{100.0, 50.0};
    // direction 2 points along +y toward bottom edge (assuming y grows downward like screen coords)
    PointDouble dir2{50.0, 100.0};

    auto points = GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box);

    // origin is inside the box, so it should be among the fill points
    bool originFound = std::any_of(points.begin(), points.end(), [&](const PointDouble& p)
        {
            return MathHelper::AreEqual(p.x, origin.x, 1e-6) && MathHelper::AreEqual(p.y, origin.y, 1e-6);
        });
    CHECK(originFound);

    // there should be at least 3 points to form a fan-shaped polygon
    CHECK(points.size() >= 3);
}

TEST_CASE("GetFanFillPoints - directions hit opposite edges of box, origin outside", "[GeometryHelper]")
{
    CRect box;
    box.left = 0.0;
    box.top = 0.0;
    box.right = 10.0;
    box.bottom = 10.0;

    // origin far to the left of the box, casting rays into the box
    PointDouble origin{-10.0, 5.0};
    PointDouble dir1{0.0, 0.0};   // toward top-left corner direction
    PointDouble dir2{0.0, 10.0};  // toward bottom-left corner direction

    auto points = GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box);

    // origin outside the box -> should not appear in fill points
    bool originFound = std::any_of(points.begin(), points.end(), [&](const PointDouble& p)
        {
            return MathHelper::AreEqual(p.x, origin.x, 1e-6) && MathHelper::AreEqual(p.y, origin.y, 1e-6);
        });
    CHECK_FALSE(originFound);

    CHECK(points.size() >= 2);
}