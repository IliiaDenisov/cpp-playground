#include <iostream>
#include "src/geometryHelper/include/PointHelper.h"
#include "src/geometryHelper/include/CollisionHelper.h"

int main()
{
    auto isInRect = CollisionHelper::IsPointInRectangle(PointDouble{0.0, 0.0}, CRect{-1, -1, 1, 1});
    int a[10];
    std::cout << a[15] << "\n";
    return 0;
}