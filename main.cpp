#include <iostream>
#include "src/geometryHelper/include/PointHelper.h"
#include "src/geometryHelper/include/CollisionHelper.h"

int main()
{
    auto isInRect = CollisionHelper::IsPointInRectangle(PointDouble{0.0, 0.0}, CRect{-1, -1, 1, 1});
    std::cout << "Is in rect: " << isInRect << "\n";
    return 0;
}