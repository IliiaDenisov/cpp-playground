#include "../include/GeometryHelper.h"
#include "../include/CollisionHelper.h"
#include "../include/MathHelper.h"
#include <cmath>
#include <iterator>

std::vector<PointDouble> GeometryHelper::GetFanFillPoints(const PointDouble& fanOrigin, const PointDouble& firstFanDirection, const PointDouble& secondFanDirection, const CRect& chartBoundingBox)
{
	std::vector<PointDouble> fillPoints;
	std::vector<PointDouble> collisionPointsTmp;

	if (CollisionHelper::HitTestRayToRect(fanOrigin, firstFanDirection, chartBoundingBox, collisionPointsTmp))
	{
		std::copy(collisionPointsTmp.begin(), collisionPointsTmp.end(), std::back_inserter(fillPoints));
		collisionPointsTmp.clear();
	}

	if (CollisionHelper::HitTestRayToRect(fanOrigin, secondFanDirection, chartBoundingBox, collisionPointsTmp))
	{
		std::copy(collisionPointsTmp.begin(), collisionPointsTmp.end(), std::back_inserter(fillPoints));
		collisionPointsTmp.clear();
	}

	if (CollisionHelper::IsPointInRectangle(fanOrigin, chartBoundingBox))
	{
		fillPoints.push_back(fanOrigin);
	}

	std::vector<PointDouble> chartEdges(4);
	chartEdges[0] = PointDouble{ chartBoundingBox.left, chartBoundingBox.top };
	chartEdges[1] = PointDouble{ chartBoundingBox.left, chartBoundingBox.bottom };
	chartEdges[2] = PointDouble{ chartBoundingBox.right, chartBoundingBox.top };
	chartEdges[3] = PointDouble{ chartBoundingBox.right, chartBoundingBox.bottom };

	const double fanSweepAngle = MathHelper::ToDegrees(MathHelper::AngleBetweenVectors(firstFanDirection - fanOrigin, secondFanDirection - fanOrigin));
	std::pair<double, double> angleRange = { std::min(fanSweepAngle, 0.0), std::max(fanSweepAngle, 0.0) };

	for (const auto edge : chartEdges)
	{
		double currAngle = MathHelper::ToDegrees(MathHelper::AngleBetweenVectors(firstFanDirection - fanOrigin, edge - fanOrigin));
		if (currAngle >= angleRange.first && currAngle <= angleRange.second)
		{
			fillPoints.push_back(edge);
		}
	}

	PointDouble centroid = FindPointsCentroid(fillPoints);

	// sorting points in a clockwise order, so they are correctly rendered by FillPolygon function
	auto less = [center = centroid](const PointDouble& a, const PointDouble& b)
		{
			if (a.x - center.x >= 0 && b.x - center.x < 0)
			{
				return true;
			}
			if (a.x - center.x < 0 && b.x - center.x >= 0)
			{
				return false;
			}
			if (a.x - center.x == 0 && b.x - center.x == 0)
			{
				if (a.y - center.y >= 0 || b.y - center.y >= 0)
				{
					return a.y > b.y;
				}
				return b.y > a.y;
			}

			// compute the cross product of vectors (center -> a) x (center -> b)
			double det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);

			if (det < 0.0)
			{
				return true;
			}

			if (det > 0.0)
			{
				return false;
			}

			// points a and b are on the same line from the center
			// check which point is closer to the center
			double d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
			double d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
			return d1 > d2;
		};

	std::sort(fillPoints.begin(), fillPoints.end(), less);

	return fillPoints;
}

PointDouble GeometryHelper::FindPointsCentroid(const std::vector<PointDouble>& points)
{
	PointDouble centroid{ 0.0, 0.0 };
	for (const auto& point : points)
	{
		centroid.x += point.x / static_cast<double>(points.size());
		centroid.y += point.y / static_cast<double>(points.size());
	}
	return centroid;
}

PointDouble GeometryHelper::FindClosestRadiusToPoint(const PointDouble& rotationCenterPoint, const PointDouble& inputPoint, const double degreesAngleStep)
{
	double radiusLen = MathHelper::GetDistanceBetweenPoints(inputPoint, rotationCenterPoint);
	auto baseRadius = PointDouble{ 1.0, 0.0 } * radiusLen;
	const double numOptions = std::round(360.0 / degreesAngleStep);

	std::vector<PointDouble> radiuses(numOptions);
	for (int i = 0; i < radiuses.size(); i++)
	{
		PointDouble currRadius = baseRadius;
		MathHelper::RotateVector(currRadius, MathHelper::ToRadians(i * degreesAngleStep));
		radiuses[i] = rotationCenterPoint + currRadius;
	}

	auto it = std::min_element(radiuses.begin(), radiuses.end(), [inputPoint](const PointDouble& lhs, const PointDouble& rhs)
		{
			return MathHelper::GetDistanceBetweenPoints(inputPoint, lhs) < MathHelper::GetDistanceBetweenPoints(inputPoint, rhs);
		});

	return it != radiuses.end() ? *it : PointDouble{ 0.0, 0.0 };
}

PointDouble GeometryHelper::FindClosestRadiusToPoint(const PointDouble& center, const PointDouble& point, const std::vector<double>& angleOptions)
{
	const double radiusLen = MathHelper::GetDistanceBetweenPoints(point, center);
	const auto baseRadius = PointDouble{ 1.0, 0.0 } * radiusLen;

	std::vector<PointDouble> radiuses(angleOptions.size());
	std::transform(angleOptions.begin(), angleOptions.end(), radiuses.begin(), [baseRadius, center](const double angle)
		{
			PointDouble currRadius = baseRadius;
			MathHelper::RotateVector(currRadius, MathHelper::ToRadians(angle));
			return center + currRadius;
		});

	auto it = std::min_element(radiuses.begin(), radiuses.end(), [point](const PointDouble& lhs, const PointDouble& rhs)
		{
			return MathHelper::GetDistanceBetweenPoints(point, lhs) < MathHelper::GetDistanceBetweenPoints(point, rhs);
		});

	return it != radiuses.end() ? *it : PointDouble{ 0.0, 0.0 };
}