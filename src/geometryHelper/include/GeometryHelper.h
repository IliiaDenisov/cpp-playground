#pragma once

#include <vector>
#include "Rect.h"
#include "PointHelper.h"

class GeometryHelper
{
public:
	static std::vector<PointDouble> GetFanFillPoints(const PointDouble& fanOrigin, const PointDouble& fanDirection_1, const PointDouble& fanDirection_2, const CRect& drawingAreaBoundingBox);
	static PointDouble FindPointsCentroid(const std::vector<PointDouble>& points);
	static PointDouble FindClosestRadiusToPoint(const PointDouble& rotationCenterPoint, const PointDouble& inputPoint, const double degreesAngleStep = 45.0);
	static PointDouble FindClosestRadiusToPoint(const PointDouble& rotationCenterPoint, const PointDouble& inputPoint, const std::vector<double>& angleOptions);
};