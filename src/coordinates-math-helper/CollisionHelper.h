#pragma once

#include <vector>
#include "Points.h"
#include "Rect.h"

class CollisionHelper
{
public:
	static constexpr double defaultTolerance = 1e-5; 

	static bool HitTestLineToLine(const PointDouble& lineStart1, const PointDouble& lineEnd1, const PointDouble& lineStart2, const PointDouble& lineEnd2, PointDouble& collisionPoint);
	static bool HitTestLineToRect(const PointDouble& lineStart1, const PointDouble& lineEnd1, const CRect& rect, std::vector<PointDouble>& collisionPoints);

	static bool HitTestSegmentToSegment(const PointDouble& segmentStart1, const PointDouble& segmentEnd1, const PointDouble& segmentStart2, const PointDouble& segmentEnd2, PointDouble& collisionPoint);
	static bool HitTestSegmentToRect(const PointDouble& segmentStart1, const PointDouble& segmentEnd1, const CRect& rect, std::vector<PointDouble>& collisionPoints);

	static bool HitTestRayToSegment(const PointDouble& rayOrigin, const PointDouble& rayDir, const PointDouble& lineStart1, const PointDouble& lineEnd1, PointDouble& collisionPoint);
	static bool HitTestRayToRect(const PointDouble& rayOrigin, const PointDouble& rayDir, const CRect& rect, std::vector<PointDouble>& collisionPoints);

	static bool HitTestQuadraticBezierCurve(PointDouble point, PointDouble point1, PointDouble point2, PointDouble controlPoint, const double dTolerance);
	static bool HitTestCubicBezierCurve(PointDouble targetPoint, PointDouble pointStart, PointDouble cp1, PointDouble cp2, PointDouble pointEnd, const double dTolerance);

	static bool IsPointInXAxisRange(const PointDouble& point, const double xStart, const double xEnd);
	static bool IsPointInYAxisRange(const PointDouble& point, const double yStart, const double yEnd);
	static bool IsPointInRectangle(const PointDouble& point, const CRect& rect);
	static bool IsRectangleInRectangle(const CRect& lhsRect, const CRect& rhsRect);
	static bool IsPointInTriangle(const PointDouble& point, const PointDouble& p1, const PointDouble& p2, const PointDouble& p3);
	static bool IsPointInEllipse(const PointDouble& point, const PointDouble& ellipseOrigin, const double semiMajorAxis, const double semiMinorAxis);
	static bool IsPointInPolygon(const PointDouble& point, const std::vector<PointDouble>& polygonPoints);
	static bool IsLineInRectangle(const PointDouble& lineStart1, const PointDouble& lineEnd1, const CRect& rect);
	static bool IsPointInAreaEnclosedByCurve(const PointDouble& point, const PointDouble& vertex1, const PointDouble& vertex2, const PointDouble& coVertex);
	static bool IsPointInSector(const PointDouble& currPoint, const PointDouble& sectorCenter, double radius, double sectorStartAngle, double sectorSweepAngle);
	static bool IsPointOnSegment(const PointDouble& point, const PointDouble& segmentStart, const PointDouble& segmentEnd, const double dTolerance = defaultTolerance);


	static double GetDistanceToFullCircleArc(const PointDouble& point, const PointDouble& arcCenter, double radius);

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
	static double GetDistanceToArc(const PointDouble& currPoint, const PointDouble& arcCenter, double arcRadius, double arcStartAngle, double arcSweepAngle);

	static bool ArePointsOnSameSideOfLineSegment(const PointDouble& p1, const PointDouble& p2, const PointDouble& lineStart, const PointDouble& lineEnd);
};