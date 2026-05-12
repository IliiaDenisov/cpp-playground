#include "../include/CurveApproximationHelper.h"
#include "../include/MathHelper.h"
#include "../include/PointHelper.h"
#include <vector>

// @details Fill cubic Bezier curve points for a curve connecting point1, point2, point3
CurveAppoximationHelper::BezierCurvesPointsInfo CurveAppoximationHelper::GetCubicBezierCurveDrawingPoints(const PointDouble &point1, const PointDouble &point2, const PointDouble &point3)
{
	auto dir = point1 - point3;

	constexpr double controlPointOffset = 0.25;
	auto cp1 = point2 + dir * controlPointOffset;
	auto cp2 = point2 - dir * controlPointOffset;

	PointDouble controlPoint0_0 = point1 + (cp1 - point1) * (2.0 / 3.0);
	PointDouble controlPoint1_0 = point2 + (cp1 - point2) * (2.0 / 3.0);

	PointDouble controlPoint0_1 = point2 + (cp2 - point2) * (2.0 / 3.0);
	PointDouble controlPoint1_1 = point3 + (cp2 - point3) * (2.0 / 3.0);

	std::vector<PointDouble> endPoints{point1, point2, point3};
	std::vector<PointDouble> controlPoints1{controlPoint0_0, controlPoint0_1};
	std::vector<PointDouble> controlPoints2{controlPoint1_0, controlPoint1_1};

	return BezierCurvesPointsInfo{endPoints, controlPoints1, controlPoints2};
}

// @details Approximate sin function in range [0, 2PI] with cubic bezier curve
CurveAppoximationHelper::BezierCurvesPointsInfo CurveAppoximationHelper::GetCubicBezierSineWaveApproximation(const PointDouble &waveStart, const double waveLength, const double amplitude)
{
	PointDouble point1 = waveStart;
	PointDouble point2 = PointDouble{point1.x + waveLength, point1.y};

	auto u = (8.0 / 3.0 - sqrt(3.0)) * waveLength / 2.0;
	auto v = 2.0 * sqrt(3.0) * amplitude;

	PointDouble cp1 = PointDouble{point1.x + u, point1.y - v};
	PointDouble cp2 = PointDouble{point2.x - u, point2.y + v};

	std::vector<PointDouble> endPoints{point1, point2};
	std::vector<PointDouble> controlPoints1{cp1};
	std::vector<PointDouble> controlPoints2{cp2};

	return BezierCurvesPointsInfo{endPoints, controlPoints1, controlPoints2};
}

std::vector<PointDouble> CurveAppoximationHelper::ApproximateQuadraticBezierCurveWithPolyline(PointDouble point1, PointDouble point2, PointDouble controlPoint)
{
	const auto len = static_cast<int>(round(MathHelper::GetVectorLength(controlPoint - point1) + MathHelper::GetVectorLength(controlPoint - point2)));

	constexpr int minNumSteps = 25;
	constexpr int maxNumSteps = 100;
	const int numSteps = std::clamp(len, minNumSteps, maxNumSteps);
	const double step = 1.0 / numSteps;

	std::vector<PointDouble> points;
	points.reserve(numSteps);

	// Iterate t in range [0.0, 1.0] with step, calculating current point on the curve
	for (double t = 0; t < 1.0; t += step)
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

		points.push_back(currPointOnCurve);
	}
	return points;
}

std::vector<PointInteger> CurveAppoximationHelper::ApplyChaikinAlgorithm(const std::vector<PointInteger> &arr, int simplificationDegree)
{
	if (simplificationDegree == 0 || arr.size() < 2)
		return arr;

	size_t l = arr.size();
	std::vector<PointInteger> smooth;

	smooth.push_back(arr.front());

	for (size_t i = 0; i < l; ++i)
	{
		size_t next = i + 1;
		if (next >= l)
			continue;

		int point_0X = (int)(0.75f * (float)arr[i].x + 0.25f * (float)arr[next].x);
		int point_0Y = (int)(0.75f * (float)arr[i].y + 0.25f * (float)arr[next].y);

		int point_1X = (int)(0.25f * (float)arr[i].x + 0.75f * (float)arr[next].x);
		int point_1Y = (int)(0.25f * (float)arr[i].y + 0.75f * (float)arr[next].y);

		PointInteger additionalPoint0 = PointInteger{point_0X, point_0Y};
		PointInteger additionalPoint1 = PointInteger{point_1X, point_1Y};

		smooth.push_back(additionalPoint0);
		smooth.push_back(additionalPoint1);
	}

	smooth.push_back(arr.back());

	if (simplificationDegree == 1)
		return smooth;

	return ApplyChaikinAlgorithm(smooth, simplificationDegree - 1);
}

std::vector<PointFloat> CurveAppoximationHelper::ApplyChaikinAlgorithm(const std::vector<PointFloat> &arr, int simplificationDegree)
{
	if (simplificationDegree == 0 || arr.size() < 2)
		return arr;

	size_t l = arr.size();
	std::vector<PointFloat> smooth;

	smooth.push_back(arr.front());

	for (size_t i = 0; i < l; ++i)
	{
		size_t next = i + 1;
		if (next >= l)
			continue;

		smooth.push_back({0.75f * arr[i].X + 0.25f * arr[next].X,
						  0.75f * arr[i].Y + 0.25f * arr[next].Y});
		smooth.push_back({0.25f * arr[i].X + 0.75f * arr[next].X,
						  0.25f * arr[i].Y + 0.75f * arr[next].Y});
	}

	smooth.push_back(arr.back());

	if (simplificationDegree == 1)
		return smooth;

	return ApplyChaikinAlgorithm(smooth, simplificationDegree - 1);
}
