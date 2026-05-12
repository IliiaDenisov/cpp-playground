#pragma once

template <typename PointType>
std::vector<PointType> CurveAppoximationHelper::GetBezierCurvesDrawingStructure(const std::vector<PointType> &endPoints, const std::vector<PointType> &firstControlPoints, const std::vector<PointType> &secondControlPoints)
{
	std::vector<PointType> bezierPointsStructure(endPoints.size() + firstControlPoints.size() + secondControlPoints.size());
	/// ------------------------------------------------------------------
	/// bezierPointsStructure array structure:
	///
	/// [endPoint_0, control_point_0_0, control_point_1_0, endPoint_1,
	/// 			 control_point_0_1, control_point_1_1, endPoint_2,
	/// 			 ....
	/// 			 control_point_0_i, control_point_1_i, endPoint_i,
	/// 			 ....
	/// 			 control_point_0_n, control_point_1_n, endPoint_n]
	/// ------------------------------------------------------------------

	int i = 0;
	for (auto &point : endPoints)
	{
		bezierPointsStructure[i] = point;
		i += 3;
	}

	i = 1;
	for (int j = 0; j < firstControlPoints.size(); ++j)
	{
		bezierPointsStructure[i] = firstControlPoints[j];
		bezierPointsStructure[i + 1] = secondControlPoints[j];
		i += 3;
	}

	return bezierPointsStructure;
}
