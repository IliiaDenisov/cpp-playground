#include "../include/CurveApproximationHelper.h"
#include "../include/CollisionHelper.h"
#include "../include/MathHelper.h"
#include <limits>

bool CollisionHelper::HitTestLineToLine(const PointDouble& lineStart1, const PointDouble& lineEnd1, const PointDouble& lineStart2, const PointDouble& lineEnd2, PointDouble& collisionPoint)
{
	// https://www.jeffreythompson.org/collision-detection/

	const double x1 = lineStart1.x;
	const double x2 = lineEnd1.x;
	const double x3 = lineStart2.x;
	const double x4 = lineEnd2.x;

	const double y1 = lineStart1.y;
	const double y2 = lineEnd1.y;
	const double y3 = lineStart2.y;
	const double y4 = lineEnd2.y;

	// calculate the direction of the lines
	const double denominator = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	const double numerator_A = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3));
	const double numerator_B = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3));

	if (MathHelper::AreEqual(denominator, 0.0))
	{
		return false;
	}

	const double uA = numerator_A / denominator;
	const double uB = numerator_B / denominator;

	const double intersectionX = x1 + (uA * (x2 - x1));
	const double intersectionY = y1 + (uA * (y2 - y1));
	collisionPoint.x = intersectionX;
	collisionPoint.y = intersectionY;
	return true;
}

bool CollisionHelper::HitTestLineToRect(const PointDouble& lineStart1, const PointDouble& lineEnd1, const CRect& rect, std::vector<PointDouble>& collisionPoints)
{
	collisionPoints.clear();
	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below

	PointDouble collisionPoint;
	std::vector<PointDouble> rectPoints{ PointDouble{ rect.left, rect.top }, PointDouble{ rect.left, rect.bottom}, PointDouble{ rect.right, rect.bottom}, PointDouble{ rect.right, rect.top} };

	bool bHit = false;
	for (int i = 0; i < 4; ++i)
	{
		auto lineStart2 = rectPoints[i % 4];
		auto lineEnd2 = rectPoints[(i + 1) % 4];

		if (HitTestLineToLine(lineStart1, lineEnd1, lineStart2, lineEnd2, collisionPoint))
		{
			bHit = true;
			collisionPoints.push_back(collisionPoint);
		}
	}

	return bHit;
}

bool CollisionHelper::HitTestSegmentToSegment(const PointDouble& segmentStart1, const PointDouble& segmentEnd1, const PointDouble& segmentStart2, const PointDouble& segmentEnd2, PointDouble& collisionPoint)
{
	// https://www.jeffreythompson.org/collision-detection/

	const double x1 = segmentStart1.x;
	const double x2 = segmentEnd1.x;
	const double x3 = segmentStart2.x;
	const double x4 = segmentEnd2.x;

	const double y1 = segmentStart1.y;
	const double y2 = segmentEnd1.y;
	const double y3 = segmentStart2.y;
	const double y4 = segmentEnd2.y;

	// calculate the direction of the lines
	const double uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	const double uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, segments are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{

		double intersectionX = x1 + (uA * (x2 - x1));
		double intersectionY = y1 + (uA * (y2 - y1));
		collisionPoint.x = intersectionX;
		collisionPoint.y = intersectionY;
		return true;
	}
	return false;
}

bool CollisionHelper::HitTestSegmentToRect(const PointDouble& segmentStart1, const PointDouble& segmentEnd1, const CRect& rect, std::vector<PointDouble>& collisionPoints)
{
	collisionPoints.clear();
	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below

	PointDouble collisionPoint;
	std::vector<PointDouble> rectPoints{ PointDouble{ rect.left, rect.top }, PointDouble{ rect.left, rect.bottom}, PointDouble{ rect.right, rect.bottom}, PointDouble{ rect.right, rect.top} };

	bool bHit = false;
	for (int i = 0; i < 4; ++i)
	{
		auto lineStart2 = rectPoints[i % 4];
		auto lineEnd2 = rectPoints[(i + 1) % 4];

		if (HitTestSegmentToSegment(segmentStart1, segmentEnd1, lineStart2, lineEnd2, collisionPoint))
		{
			bHit = true;
			collisionPoints.push_back(collisionPoint);
		}
	}

	return bHit;
}


bool CollisionHelper::HitTestRayToSegment(const PointDouble& rayOrigin, const PointDouble& rayDir, const PointDouble& segmentStart1, const PointDouble& segmentEnd1, PointDouble& collisionPoint)
{
	if (HitTestLineToLine(rayOrigin, rayDir, segmentStart1, segmentEnd1, collisionPoint))
	{
		bool onSegment = IsPointOnSegment(collisionPoint, segmentStart1, segmentEnd1);
		auto n_1 = MathHelper::GetNormalizedVector(rayDir - rayOrigin);
		auto n_2 = MathHelper::GetNormalizedVector(collisionPoint - rayOrigin);
		return onSegment && MathHelper::AreEqual(n_1.x, n_2.x) && MathHelper::AreEqual(n_1.y, n_2.y);
	}
	return false;
}


bool CollisionHelper::HitTestRayToRect(const PointDouble& rayOrigin, const PointDouble& rayDir, const CRect& rect, std::vector<PointDouble>& collisionPoints)
{
	collisionPoints.clear();
	PointDouble collisionPointTmp;

	std::vector<PointDouble> edges = { PointDouble{rect.left, rect.top}, PointDouble{rect.right, rect.top}, PointDouble{rect.right, rect.bottom}, PointDouble{rect.left, rect.bottom} };
	for (int i = 0; i < edges.size(); i++)
	{
		if (HitTestRayToSegment(rayOrigin, rayDir, edges[i % edges.size()], edges[(i + 1) % edges.size()], collisionPointTmp))
		{
			collisionPoints.push_back(collisionPointTmp);
		}
	}

	return !collisionPoints.empty();
}


bool CollisionHelper::HitTestQuadraticBezierCurve(PointDouble targetPoint, PointDouble point1, PointDouble point2, PointDouble controlPoint, const double dTolerance)
{
	// quadro line from point1 to point2 with control point controlPoint
	const auto len = static_cast<int>(round(MathHelper::GetVectorLength(controlPoint - point1) + MathHelper::GetVectorLength(controlPoint - point2)));
	constexpr int minNumSteps = 25;
	constexpr int maxNumSteps = 100;
	const int numSteps = std::clamp(len, minNumSteps, maxNumSteps);
	const double step = 1.0 / numSteps;
	bool bHit = false;

	PointDouble prevPointOnCurve = { 0.0, 0.0 };
	// Iterate t in range [0.0, 1.0] with step, calculating current point on the curve
	// If for at least one point on the curve the distance between point and point on the curve is less than tolerance, 
	// then the hit test is successful
	for (double t = 0; t < 1.0 && !bHit; t += step)
	{
		t = std::clamp(t, 0.0, 1.0);

		// (1-t)^2 * P0 + 2t * (1-t) * P1 + t * t * P2
		// P0 = point1
		// P1 = controlPoint
		// P2 = point2
		const PointDouble s1 = point1 * ((1 - t) * (1 - t));
		const PointDouble s2 = controlPoint * (2 * t * (1 - t));
		const PointDouble s3 = point2 * (t * t);
		const PointDouble currPointOnCurve = s1 + s2 + s3;

		if (t > 0.0)
		{
			bHit = MathHelper::GetDistanceToSegmentFromPoint(prevPointOnCurve, currPointOnCurve, targetPoint) < dTolerance;
		}
		else
		{
			bHit = MathHelper::GetVectorLength(targetPoint - currPointOnCurve) < dTolerance;
		}
		prevPointOnCurve = currPointOnCurve;
	}

	return bHit;
}

bool CollisionHelper::HitTestCubicBezierCurve(PointDouble targetPoint, PointDouble p0, PointDouble p1, PointDouble p2, PointDouble p3, const double dTolerance)
{
	// quadro line from point1 to point2 with control point controlPoint
	const auto len = static_cast<int>(round(MathHelper::GetVectorLength(p0 - p1) + MathHelper::GetVectorLength(p1 - p2) + MathHelper::GetVectorLength(p2 - p3)));
	constexpr int minNumSteps = 25;
	constexpr int maxNumSteps = 100;
	const int numSteps = std::clamp(len, minNumSteps, maxNumSteps);
	const double step = 1.0 / numSteps;
	bool bHit = false;

	// Iterate t in range [0.0, 1.0] with step, calculating current point on the curve
	// If for at least one point on the curve the distance between point and point on the curve is less than tolerance, 
	// then the hit test is successful

	PointDouble prevPointOnCurve = { 0.0, 0.0 };

	for (double t = 0.0; t < 1.0 && !bHit; t += step)
	{
		t = std::clamp(t, 0.0, 1.0);

		// (1-t)^3 * P0 + 3(1 - t)^2 * P1 * t + 3 * (1 - t) * (t^2) * P2 + t^3 * P3
		// P0 = point1
		// P1 = controlPoint
		// P2 = point2
		const PointDouble s1 = p0 * ((1 - t) * (1 - t) * (1 - t));
		const PointDouble s2 = p1 * 3.0 * t * ((1 - t) * (1 - t));
		const PointDouble s3 = p2 * 3.0 * (t * t * (1 - t));
		const PointDouble s4 = p3 * (t * t * t);
		const PointDouble currPointOnCurve = s1 + s2 + s3 + s4;

		if (t > 0.0)
		{
			bHit = MathHelper::GetDistanceToSegmentFromPoint(prevPointOnCurve, currPointOnCurve, targetPoint) < dTolerance;
		}
		else
		{
			bHit = MathHelper::GetVectorLength(targetPoint - currPointOnCurve) < dTolerance;
		}
		prevPointOnCurve = currPointOnCurve;
	}

	return bHit;
}


bool CollisionHelper::IsPointInXAxisRange(const PointDouble& point, const double xStart, const double xEnd)
{
	return point.x >= std::min(xStart, xEnd) && point.x <= std::max(xStart, xEnd);
}


bool CollisionHelper::IsPointInYAxisRange(const PointDouble& point, const double yStart, const double yEnd)
{
	return point.y >= std::min(yStart, yEnd) && point.y <= std::max(yStart, yEnd);
}


bool CollisionHelper::IsPointInRectangle(const PointDouble& point, const CRect& rect)
{
	return point.x <= rect.right && point.x >= rect.left && point.y >= rect.top && point.y <= rect.bottom;
}


bool CollisionHelper::IsRectangleInRectangle(const CRect& lhsRect, const CRect& rhsRect)
{
	return lhsRect.left >= rhsRect.left && lhsRect.right <= rhsRect.right &&
		lhsRect.top >= rhsRect.top && lhsRect.bottom <= rhsRect.bottom;
}


bool CollisionHelper::IsPointInTriangle(const PointDouble& point, const PointDouble& p1, const PointDouble& p2, const PointDouble& p3)
{
	const auto sign = [](const PointDouble& p1, const PointDouble& p2, const PointDouble& p3)
		{
			return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
		};

	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(point, p1, p2);
	d2 = sign(point, p2, p3);
	d3 = sign(point, p3, p1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

bool CollisionHelper::IsPointInEllipse(const PointDouble& point, const PointDouble& ellipseOrigin, const double semiMajorAxis, const double semiMinorAxis)
{
	if (MathHelper::AreEqual(semiMinorAxis, 0.0) || MathHelper::AreEqual(semiMajorAxis, 0.0))
		return false;

	return (((point.x - ellipseOrigin.x) * (point.x - ellipseOrigin.x)) / (semiMajorAxis * semiMajorAxis) +
		((point.y - ellipseOrigin.y) * (point.y - ellipseOrigin.y)) / (semiMinorAxis * semiMinorAxis)) <= 1.0;

}

bool CollisionHelper::IsPointInPolygon(const PointDouble& point, const std::vector<PointDouble>& polygonPoints)
{
	double x = point.x;
	double y = point.y;

	int i;
	int j = polygonPoints.size() - 1;
	bool oddNodes = false;

	for (i = 0; i < polygonPoints.size(); i++)
	{
		if ((polygonPoints[i].y < y && polygonPoints[j].y >= y
			|| polygonPoints[j].y < y && polygonPoints[i].y >= y)
			&& (polygonPoints[i].x <= x || polygonPoints[j].x <= x))
		{
			oddNodes ^= (polygonPoints[i].x + (y - polygonPoints[i].y) / (polygonPoints[j].y - polygonPoints[i].y) * (polygonPoints[j].x - polygonPoints[i].x) < x);
		}
		j = i;
	}

	return oddNodes;
}

bool CollisionHelper::IsLineInRectangle(const PointDouble& lineStart1, const PointDouble& lineEnd1, const CRect& rect)
{
	return IsPointInRectangle(lineStart1, rect) && IsPointInRectangle(lineEnd1, rect);
}

bool CollisionHelper::IsPointInAreaEnclosedByCurve(const PointDouble& point, const PointDouble& vertex1, const PointDouble& vertex2, const PointDouble& coVertex)
{
	auto dir = vertex1 - vertex2;
	auto controlPoint1 = coVertex + (dir * 0.25);
	auto controlPoint2 = coVertex - (dir * 0.25);

	auto firstHalfPoints = CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(vertex1, coVertex, controlPoint1);
	auto secondHalfPoints = CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(coVertex, vertex2, controlPoint2);

	std::vector<PointDouble> polygonPoints;
	polygonPoints.reserve(firstHalfPoints.size() + secondHalfPoints.size());

	polygonPoints.insert(polygonPoints.end(), firstHalfPoints.begin(), firstHalfPoints.end());
	polygonPoints.insert(polygonPoints.end(), secondHalfPoints.begin(), secondHalfPoints.end());

	return IsPointInPolygon(point, polygonPoints);
}

bool CollisionHelper::IsPointInSector(const PointDouble& currPoint, const PointDouble& sectorCenter, double radius, double sectorStartAngle, double sectorSweepAngle)
{
	PointDouble arcStartAngleDir = PointDouble(1.0, 0.0);
	MathHelper::RotateVector(arcStartAngleDir, MathHelper::ToRadians(sectorStartAngle));

	PointDouble currVecDir = currPoint - sectorCenter;
	MathHelper::NormalizeVector(currVecDir);

	auto angleDeviationFromStartAngle = MathHelper::ToDegrees(MathHelper::AngleBetweenVectors(arcStartAngleDir, currVecDir));

	bool bHits = false;
	if (sectorSweepAngle < 0.0 && angleDeviationFromStartAngle < 0.0)
	{
		bHits = angleDeviationFromStartAngle > sectorSweepAngle;
	}
	else if (sectorSweepAngle >= 0 && angleDeviationFromStartAngle >= 0.0)
	{
		bHits = angleDeviationFromStartAngle < sectorSweepAngle;
	}
	return bHits ? MathHelper::GetDistanceBetweenPoints(sectorCenter, currPoint) <= radius : false;
}

bool CollisionHelper::IsPointOnSegment(const PointDouble& point, const PointDouble& s1, const PointDouble& s2, const double dTolerance)
{
	double sum = MathHelper::GetDistanceBetweenPoints(s1, point) + MathHelper::GetDistanceBetweenPoints(point, s2) - MathHelper::GetDistanceBetweenPoints(s1, s2);
	return MathHelper::AreEqual(sum, 0.0, dTolerance);
}

double CollisionHelper::GetDistanceToFullCircleArc(const PointDouble& point, const PointDouble& arcCenter, double radius)
{
	return abs(radius - MathHelper::GetDistanceBetweenPoints(point, arcCenter));
}

/// @details Calculates the distance to the arc if the currPoint can hit the arc with a center point of arcCenter and with a
///	start angle of arcStartAngle (in degrees, counterclockwise) and a sweep angle (in degrees, counterclockwise),
/// otherwise returns DBL_MAX
///
/// @param currPoint	 Point used to calculate distance from
/// @param arcCenter	 Center of the arc
///	@param arcRadius	 Radius of the arc
///	@param arcStartAngle A start arc angle in degrees, counterclockwise
///	@param arcSweepAngle A sweep arc angle in degrees, counterclockwise
///	@returns The distance to the arc
double CollisionHelper::GetDistanceToArc(const PointDouble& currPoint, const PointDouble& arcCenter, double arcRadius, double arcStartAngle, double arcSweepAngle)
{
	PointDouble arcStartAngleDir = PointDouble(1.0, 0.0);
	MathHelper::RotateVector(arcStartAngleDir, MathHelper::ToRadians(arcStartAngle));

	PointDouble currVecDir = currPoint - arcCenter;
	MathHelper::NormalizeVector(currVecDir);

	auto angleDeviationFromStartAngle = MathHelper::ToDegrees(MathHelper::AngleBetweenVectors(arcStartAngleDir, currVecDir));

	bool bHits = false;
	if (arcSweepAngle < 0.0 && angleDeviationFromStartAngle < 0.0)
	{
		bHits = angleDeviationFromStartAngle > arcSweepAngle;
	}
	else if (arcSweepAngle >= 0 && angleDeviationFromStartAngle >= 0.0)
	{
		bHits = angleDeviationFromStartAngle < arcSweepAngle;
	}
	return bHits ? GetDistanceToFullCircleArc(currPoint, arcCenter, arcRadius) : std::numeric_limits<double>::max();
}

bool CollisionHelper::ArePointsOnSameSideOfLineSegment(const PointDouble& p1, const PointDouble& p2, const PointDouble& lineStart, const PointDouble& lineEnd)
{
	PointDouble lineDirection = lineEnd - lineStart;
	PointDouble v1 = p1 - lineStart;
	PointDouble v2 = p2 - lineStart;

	// Compute the cross products
	double cross1 = MathHelper::CrossProduct(lineDirection, v1);
	double cross2 = MathHelper::CrossProduct(lineDirection, v2);

	// The points are on the same side if the cross products have the same sign
	return cross1 * cross2 >= 0.0;
};