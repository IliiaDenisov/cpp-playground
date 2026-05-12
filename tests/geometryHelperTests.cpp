#include "../src/geometryHelper/include/PointHelper.h"
#include "../src/geometryHelper/include/CollisionHelper.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "Point in rectangle hit testing", "[point_in_rect]" ) {
    CRect rect{0, 0, 1, 1};
    PointDouble point{0.5, 0.5};
    REQUIRE(CollisionHelper::IsPointInRectangle(point, rect));
    
    rect = CRect{0, 0, 1, 1};
    point = PointDouble{-0.5, 0.5};
    REQUIRE(!CollisionHelper::IsPointInRectangle(point, rect));
    
    rect = CRect{0, 0, 1, 1};
    point = PointDouble{-0.5, -0.5};
    REQUIRE(!CollisionHelper::IsPointInRectangle(point, rect));

    rect = CRect{0, 0, 1, 1};
    point = PointDouble{1.0, 1.0};
    REQUIRE(CollisionHelper::IsPointInRectangle(point, rect));
}