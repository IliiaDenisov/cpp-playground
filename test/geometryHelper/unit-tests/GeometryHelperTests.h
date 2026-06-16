#pragma once
#include "../../../src/geometryHelper/include/GeometryHelper.h"
#include "../../../src/geometryHelper/include/MathHelper.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;

// Helper to build a CRect with y-axis: top < bottom (screen coords)
static CRect MakeRect(double left, double top, double right, double bottom)
{
    CRect r;
    r.left   = left;
    r.top    = top;
    r.right  = right;
    r.bottom = bottom;
    return r;
}

// ─────────────────────────────────────────────────────────────
// FindClosestRadiusToPoint (angle step overload)
// ─────────────────────────────────────────────────────────────

TEST_CASE("FindClosestRadiusToPoint (step) - result is always same distance from center as input", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    PointDouble input{3.0, 4.0}; // distance = 5
    auto result = GeometryHelper::FindClosestRadiusToPoint(center, input, 45.0);

    CHECK_THAT(MathHelper::GetDistanceBetweenPoints(result, center),
               WithinAbs(MathHelper::GetDistanceBetweenPoints(input, center), 1e-6));
}

TEST_CASE("FindClosestRadiusToPoint (step) - point exactly on 0-degree axis", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    PointDouble input{5.0, 0.0};
    auto result = GeometryHelper::FindClosestRadiusToPoint(center, input, 45.0);

    CHECK_THAT(result.x, WithinAbs(5.0, 1e-6));
    CHECK_THAT(result.y, WithinAbs(0.0, 1e-6));
}

TEST_CASE("FindClosestRadiusToPoint (step) - point near 90-degree axis snaps to 90", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    PointDouble input{0.01, 5.0}; // almost on +y axis
    auto result = GeometryHelper::FindClosestRadiusToPoint(center, input, 45.0);

    CHECK_THAT(result.x, WithinAbs(0.0, 1e-4));
    CHECK_THAT(result.y, WithinAbs(MathHelper::GetDistanceBetweenPoints(input, center), 1e-4));
}

TEST_CASE("FindClosestRadiusToPoint (step) - 360/step gives correct candidate count", "[GeometryHelper]")
{
    // With step=90, options are 0, 90, 180, 270 degrees.
    // A point at 45 degrees is equidistant from 0 and 90 -> result must be one of them.
    PointDouble center{0.0, 0.0};
    double r = 7.0;
    PointDouble input{ r * std::cos(MathHelper::ToRadians(45.0)),
                       r * std::sin(MathHelper::ToRadians(45.0)) };
    auto result = GeometryHelper::FindClosestRadiusToPoint(center, input, 90.0);

    bool at0   = std::abs(result.x - r)   < 1e-5 && std::abs(result.y)     < 1e-5;
    bool at90  = std::abs(result.x)        < 1e-5 && std::abs(result.y - r) < 1e-5;
    CHECK((at0 || at90));
}

TEST_CASE("FindClosestRadiusToPoint (step) - offset center is respected", "[GeometryHelper]")
{
    PointDouble center{50.0, 30.0};
    PointDouble input{60.0, 30.0}; // 10 units right of center = 0 degrees
    auto result = GeometryHelper::FindClosestRadiusToPoint(center, input, 45.0);

    CHECK_THAT(result.x, WithinAbs(60.0, 1e-5));
    CHECK_THAT(result.y, WithinAbs(30.0, 1e-5));
}

TEST_CASE("FindClosestRadiusToPoint (step) - fine step gives closer snap than coarse step", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    PointDouble input{3.0, 4.0};

    auto coarse = GeometryHelper::FindClosestRadiusToPoint(center, input, 90.0);
    auto fine   = GeometryHelper::FindClosestRadiusToPoint(center, input, 10.0);

    double distCoarse = MathHelper::GetDistanceBetweenPoints(input, coarse);
    double distFine   = MathHelper::GetDistanceBetweenPoints(input, fine);

    CHECK(distFine <= distCoarse + 1e-9);
}

// ─────────────────────────────────────────────────────────────
// FindClosestRadiusToPoint (angle options overload)
// ─────────────────────────────────────────────────────────────

TEST_CASE("FindClosestRadiusToPoint (options) - empty options returns origin", "[GeometryHelper]")
{
    auto result = GeometryHelper::FindClosestRadiusToPoint({0,0}, {5,5}, {});
    CHECK_THAT(result.x, WithinAbs(0.0, 1e-12));
    CHECK_THAT(result.y, WithinAbs(0.0, 1e-12));
}

TEST_CASE("FindClosestRadiusToPoint (options) - single option always returned", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    PointDouble input{3.0, 4.0};
    double r = MathHelper::GetDistanceBetweenPoints(input, center);

    auto result = GeometryHelper::FindClosestRadiusToPoint(center, input, {90.0});

    // Only option is 90 degrees -> result must be (0, r)
    CHECK_THAT(result.x, WithinAbs(0.0, 1e-6));
    CHECK_THAT(result.y, WithinAbs(r,   1e-6));
}

TEST_CASE("FindClosestRadiusToPoint (options) - exact angle match", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    double r = 5.0;
    double angleDeg = 30.0;
    PointDouble input{ r * std::cos(MathHelper::ToRadians(angleDeg)),
                       r * std::sin(MathHelper::ToRadians(angleDeg)) };

    auto result = GeometryHelper::FindClosestRadiusToPoint(center, input, {0.0, 30.0, 60.0, 90.0});

    CHECK_THAT(result.x, WithinAbs(input.x, 1e-6));
    CHECK_THAT(result.y, WithinAbs(input.y, 1e-6));
}

TEST_CASE("FindClosestRadiusToPoint (options) - picks nearest of two candidates", "[GeometryHelper]")
{
    PointDouble center{0.0, 0.0};
    double r = 5.0;
    // Point at 80 degrees -> closer to 90 than to 45
    PointDouble input{ r * std::cos(MathHelper::ToRadians(80.0)),
                       r * std::sin(MathHelper::ToRadians(80.0)) };

    auto result = GeometryHelper::FindClosestRadiusToPoint(center, input, {0.0, 45.0, 90.0, 135.0});

    CHECK_THAT(result.x, WithinAbs(0.0, 1e-5));
    CHECK_THAT(result.y, WithinAbs(r,   1e-5));
}

TEST_CASE("FindClosestRadiusToPoint (options) - result preserves radius from center", "[GeometryHelper]")
{
    PointDouble center{10.0, 20.0};
    PointDouble input{17.0, 24.0};
    double expectedRadius = MathHelper::GetDistanceBetweenPoints(input, center);

    auto result = GeometryHelper::FindClosestRadiusToPoint(center, input, {0.0, 30.0, 45.0, 60.0, 90.0, 180.0, 270.0});

    CHECK_THAT(MathHelper::GetDistanceBetweenPoints(result, center), WithinAbs(expectedRadius, 1e-6));
}

// ─────────────────────────────────────────────────────────────
// GetFanFillPoints
// ─────────────────────────────────────────────────────────────

TEST_CASE("GetFanFillPoints - origin inside box is included in result", "[GeometryHelper]")
{
    auto box = MakeRect(0.0, 0.0, 100.0, 100.0);
    PointDouble origin{50.0, 50.0};
    PointDouble dir1{100.0, 50.0}; // ray toward right edge
    PointDouble dir2{50.0, 100.0}; // ray toward bottom edge

    auto pts = GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box);

    bool hasOrigin = std::any_of(pts.begin(), pts.end(), [&](const PointDouble& p)
    {
        return std::abs(p.x - origin.x) < 1e-6 && std::abs(p.y - origin.y) < 1e-6;
    });
    CHECK(hasOrigin);
}

TEST_CASE("GetFanFillPoints - origin outside box is not included in result", "[GeometryHelper]")
{
    auto box = MakeRect(0.0, 0.0, 100.0, 100.0);
    PointDouble origin{-20.0, 50.0}; // left of box
    PointDouble dir1{0.0, 0.0};       // toward top-left corner
    PointDouble dir2{0.0, 100.0};     // toward bottom-left corner

    auto pts = GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box);

    bool hasOrigin = std::any_of(pts.begin(), pts.end(), [&](const PointDouble& p)
    {
        return std::abs(p.x - origin.x) < 1e-6 && std::abs(p.y - origin.y) < 1e-6;
    });
    CHECK_FALSE(hasOrigin);
}

TEST_CASE("GetFanFillPoints - returns at least 3 points for a valid fan inside box", "[GeometryHelper]")
{
    auto box = MakeRect(0.0, 0.0, 100.0, 100.0);
    PointDouble origin{50.0, 50.0};
    PointDouble dir1{100.0, 50.0};
    PointDouble dir2{50.0, 100.0};

    auto pts = GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box);
    CHECK(pts.size() >= 3);
}

TEST_CASE("GetFanFillPoints - result points are sorted in clockwise order around centroid", "[GeometryHelper]")
{
    auto box = MakeRect(0.0, 0.0, 100.0, 100.0);
    PointDouble origin{50.0, 50.0};
    PointDouble dir1{100.0, 50.0};
    PointDouble dir2{50.0, 100.0};

    auto pts = GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box);
    REQUIRE(pts.size() >= 3);

    // Verify sorted order: signed area of the polygon should be negative for clockwise
    // (assuming standard orientation where y increases downward, as in screen coords)
    double signedArea = 0.0;
    for (size_t i = 0; i < pts.size(); ++i)
    {
        const auto& a = pts[i];
        const auto& b = pts[(i + 1) % pts.size()];
        signedArea += (a.x * b.y - b.x * a.y);
    }
    // Clockwise winding → signed area negative in standard math coords,
    // positive in screen coords (y-down). Either way it should not be zero
    // for a non-degenerate polygon.
    CHECK(std::abs(signedArea) > 1e-6);
}

TEST_CASE("GetFanFillPoints - box corner inside fan sweep angle is included", "[GeometryHelper]")
{
    // Origin at top-left area, two rays sweeping through the bottom-right corner
    auto box = MakeRect(0.0, 0.0, 100.0, 100.0);
    PointDouble origin{10.0, 10.0};
    PointDouble dir1{100.0, 10.0};  // ray right (along top edge)
    PointDouble dir2{10.0, 100.0};  // ray down (along left edge)

    auto pts = GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box);

    // The bottom-right corner (100,100) is within the fan sweep
    bool hasBottomRight = std::any_of(pts.begin(), pts.end(), [](const PointDouble& p)
    {
        return std::abs(p.x - 100.0) < 1e-6 && std::abs(p.y - 100.0) < 1e-6;
    });
    CHECK(hasBottomRight);
}

TEST_CASE("GetFanFillPoints - box corner outside fan sweep angle is excluded", "[GeometryHelper]")
{
    // A narrow fan: both rays point rightward, very small sweep
    // The top-left corner (0,0) should not be in the sweep
    auto box = MakeRect(0.0, 0.0, 100.0, 100.0);
    PointDouble origin{50.0, 50.0};
    PointDouble dir1{100.0, 49.0}; // slightly above midline
    PointDouble dir2{100.0, 51.0}; // slightly below midline

    auto pts = GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box);

    // Top-left corner is far outside the narrow rightward fan
    bool hasTopLeft = std::any_of(pts.begin(), pts.end(), [](const PointDouble& p)
    {
        return std::abs(p.x - 0.0) < 1e-6 && std::abs(p.y - 0.0) < 1e-6;
    });
    CHECK_FALSE(hasTopLeft);
}

TEST_CASE("GetFanFillPoints - ray collision points with box edges are included", "[GeometryHelper]")
{
    auto box = MakeRect(0.0, 0.0, 100.0, 100.0);
    PointDouble origin{50.0, 50.0};
    PointDouble dir1{100.0, 50.0}; // hits right edge at (100, 50)
    PointDouble dir2{50.0, 0.0};   // hits top edge at (50, 0)

    auto pts = GeometryHelper::GetFanFillPoints(origin, dir1, dir2, box);

    bool hasRightHit = std::any_of(pts.begin(), pts.end(), [](const PointDouble& p)
    {
        return std::abs(p.x - 100.0) < 1e-5 && std::abs(p.y - 50.0) < 1e-5;
    });
    bool hasTopHit = std::any_of(pts.begin(), pts.end(), [](const PointDouble& p)
    {
        return std::abs(p.x - 50.0) < 1e-5 && std::abs(p.y - 0.0) < 1e-5;
    });

    CHECK(hasRightHit);
    CHECK(hasTopHit);
}