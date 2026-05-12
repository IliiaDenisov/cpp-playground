#pragma once
#include "MathHelper.h"
#include "PointHelper.h"
#include <vector>

struct CurveAppoximationHelper
{
	struct BezierCurvesPointsInfo
	{
		std::vector<PointDouble> endPoints;
		std::vector<PointDouble> controlPoints1;
		std::vector<PointDouble> controlPoints2;
	};

	template<typename PointType>
	static std::vector<PointType> GetBezierCurvesDrawingStructure(const std::vector<PointType>& endPoints, const std::vector<PointType>& firstControlPoints, const std::vector<PointType>& secondControlPoints);

	// @details Fill cubic Bezier curve points for a curve connecting point1, point2, point3
	static BezierCurvesPointsInfo GetCubicBezierCurveDrawingPoints(const PointDouble& point1, const PointDouble& point2, const PointDouble& point3);

	// @details Approximate sin function in range [0, 2PI] with cubic bezier curve
	static BezierCurvesPointsInfo GetCubicBezierSineWaveApproximation(const PointDouble& waveStart, const double waveLength, const double amplitude);
	static std::vector<PointDouble> ApproximateQuadraticBezierCurveWithPolyline(PointDouble point1, PointDouble point2, PointDouble controlPoint);
	static std::vector<PointInteger> ApplyChaikinAlgorithm(const std::vector<PointInteger>& arr, int simplificationDegree);
	static std::vector<PointFloat> ApplyChaikinAlgorithm(const std::vector<PointFloat>& arr, int simplificationDegree);
};

#include "CurveApproximationHelper.hpp"
