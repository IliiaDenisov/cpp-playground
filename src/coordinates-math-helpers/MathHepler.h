#pragma once

#include <type_traits>
#include "PointHelper.h"

class MathHelper
{
public:
	static constexpr double PI = 3.14159265358979323846;

	template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	static bool AreEqual(T x, T y, const T epsilon = 1e-5)
	{
		return std::abs(x - y) <= epsilon * std::abs(x);
	}

	template <typename PointType>
	static auto DotProduct(const PointType& lhs, const PointType& rhs)
	{
		using PointTraits = PointHelper::PointTraits<PointType>;
		return PointTraits::x(lhs) * PointTraits::x(rhs) + PointTraits::y(lhs) * PointTraits::y(rhs);
	}

	template <typename PointType>
	static auto CrossProduct(const PointType& lhs, const PointType& rhs)
	{
		using PointTraits = PointHelper::PointTraits<PointType>;
		return PointTraits::x(lhs) * PointTraits::y(rhs) - PointTraits::x(lhs) * PointTraits::x(rhs);
	}

	template <typename PointType, typename std::enable_if_t<std::is_floating_point_v<typename PointHelper::PointTraits<PointType>::CoordinateType>, bool> = true>
	static void NormalizeVector(PointType& v)
	{
		using PointTraits = PointHelper::PointTraits<PointType>;

		const PointType& constV = v;
		auto len = std::sqrt(PointTraits::x(constV) * PointTraits::x(constV) + PointTraits::y(constV) * PointTraits::y(constV));
		double DoubleEpsilon = 1e-5;
		if (abs(len) < static_cast<decltype(std::declval<typename PointTraits::CoordinateType>())>(DoubleEpsilon))
			return;

		PointTraits::x(v) /= len;
		PointTraits::y(v) /= len;
	}

	template <typename PointType, typename std::enable_if_t<std::is_floating_point_v<typename PointHelper::PointTraits<PointType>::CoordinateType>, bool> = true>
	static PointType GetNormalizedVector(const PointType& vec)
	{
		PointType vecNormalized = vec;
		NormalizeVector(vecNormalized);
		return vecNormalized;
	}

	template <typename PointType, typename std::enable_if_t<std::is_floating_point_v<typename PointHelper::PointTraits<PointType>::CoordinateType>, bool> = true>
	static PointType GetNormalVectorToSegment(const PointType& segmentStartPoint, const PointType& segmentEndPoint)
	{
		using PointTraits = PointHelper::PointTraits<PointType>;
		using CoordType = PointTraits::CoordinateType;

		PointType segmentVector = segmentEndPoint - segmentStartPoint;
		PointType normal = PointType{ PointTraits::y(segmentVector), -PointTraits::x(segmentVector) };
		NormalizeVector(normal);
		return normal;
	}

	template <typename PointType, typename std::enable_if_t<std::is_floating_point_v<typename PointHelper::PointTraits<PointType>::CoordinateType>, bool> = true>
	static PointType GetNormalVectorFromPointToSegment(const PointType& segmentStartPoint, const PointType& segmentEndPoint, const PointType& basePoint)
	{
		using PointTraits = PointHelper::PointTraits<PointType>;
		using CoordType = PointTraits::CoordinateType;

		PointType segmentVector = segmentEndPoint - segmentStartPoint;
		PointType normal_1 = PointType{ PointTraits::y(segmentVector), -PointTraits::x(segmentVector) };
		PointType normal_2 = PointType{ -PointTraits::y(segmentVector), PointTraits::x(segmentVector) };

		NormalizeVector(normal_1);
		NormalizeVector(normal_2);

		// choose the vector for which the dot product is positive
		return DotProduct(basePoint - segmentEndPoint, normal_1) > static_cast<CoordType>(0.0) ? normal_1 : normal_2;
	}

	template <typename PointType, typename std::enable_if_t<std::is_floating_point_v<typename PointHelper::PointTraits<PointType>::CoordinateType>, bool> = true>
	static auto GetDistanceBetweenPoints(const PointType& p1, const PointType& p2)
	{
		using PointTraits = PointHelper::PointTraits<PointType>;
		return std::sqrt((PointTraits::x(p2) - PointTraits::x(p1)) * (PointTraits::x(p2) - PointTraits::x(p1))
			+ (PointTraits::y(p2) - PointTraits::y(p1)) * (PointTraits::y(p2) - PointTraits::y(p1)));
	}

	template <typename PointType, typename std::enable_if_t<std::is_floating_point_v<typename PointHelper::PointTraits<PointType>::CoordinateType>, bool> = true>
	static auto GetVectorLength(const PointType& vec)
	{
		using PointTraits = PointHelper::PointTraits<PointType>;
		using CoordType = PointTraits::CoordinateType;
		return GetDistanceBetweenPoints(vec, PointType{ static_cast<CoordType>(0.0), static_cast<CoordType>(0.0) });
	}

	static double GetDistanceToLineFromPoint(PointDouble l1, PointDouble l2, PointDouble point)
	{
		double numerator = abs((l2.y - l1.y) * point.x - (l2.x - l1.x) * point.y + l2.x * l1.y - l2.y * l1.x);
		double denominator = GetDistanceBetweenPoints(l2, l1);

		if (AreEqual(denominator, 0.0))
			return 0.0;

		return numerator / denominator;
	}

	static double GetDistanceToSegmentFromPoint(PointDouble s0, PointDouble s1, PointDouble p)
	{
		// Return minimum distance between line segment s0-s1 and point p
		const double l2 = GetDistanceBetweenPoints(s0, s1) * GetDistanceBetweenPoints(s0, s1);
		if (MathHelper::AreEqual(l2, 0.0))
		{
			return GetDistanceBetweenPoints(p, s0);   // s0 == s1 case
		}

		// Consider the line extending the segment, parameterized as s0 + t (s1 - s0).
		// We find projection of point p onto the line. 
		// It falls where t = [(p-s0) . (s1-s0)] / |s1-s0|^2
		// We clamp t from [0,1] to handle points outside the segment vw.
		const double t = std::clamp(DotProduct(p - s0, s1 - s0) / l2, 0.0, 1.0);
		const PointDouble projection = s0 + (s1 - s0) * t;  // Projection falls on the segment
		return GetDistanceBetweenPoints(p, projection);
	}

	/// @details Counterclockwise angle from vector lhs to vector rhs
	static double AngleBetweenVectors(const PointDouble& lhs, const PointDouble& rhs)
	{
		double dot = lhs.x * rhs.x + lhs.y * rhs.y;			// Dot product between[x1, y1] and [x2, y2]
		double det = lhs.x * rhs.y - lhs.y * rhs.x;			// Determinant
		return atan2(det, dot);								// atan2(y, x) or atan2(sin, cos)
	}

	static double ToRadians(double angleDegrees)
	{
		return angleDegrees / 180.0 * PI;
	}

	static double ToDegrees(double angleRadians)
	{
		return angleRadians / PI * 180.0;
	}

	//  @details convert angle to [-PI, PI] range
	static double NormalizeAngle(double angle)
	{
		return atan2(sin(angle), cos(angle));
	}

	static void RotateVector(PointDouble& vec, double angle)
	{
		double px = vec.x * cos(angle) - vec.y * sin(angle);
		double py = vec.x * sin(angle) + vec.y * cos(angle);
		vec.x = px;
		vec.y = py;
	}

	/// @brief Linear interpolation
	/// @tparam T	Numeric type
	/// @param x	Value used for interpolating a value inside [a, b] interval. x must be in [0, 1] interval
	/// @param a	Start of the [a, b] interval 
	/// @param b	End of the [a, b] interval 
	/// @return		Interpolated value in interval [a, b]
	template<class T>
	static T Lerp(T x, T a, T b)
	{
		if (a > b)
			std::swap(a, b);

		x = std::clamp(x, 0.0, 1.0);
		return a + x * (b - a);
	}

	/// @brief Reversed linear interpolation
	/// @tparam T Numeric type
	/// @param x  Value inside [a, b] interval to get its weight from [0, 1] in relation to [a, b] interval
	/// @param a  Start of the [a, b] interval 
	/// @param b  End of the [a, b] interval 
	/// @return	  Weight from [0, 1]
	template<class T>
	static T Unlerp(T x, T a, T b)
	{
		if (a > b)
			std::swap(a, b);

		x = std::clamp(x, a, b);

		return b != a ? (x - a) / (b - a) : -1;
	}

	/// @brief Combining lerp and unlerp to linearly map a value from interval [a1, a2] to interval [b1, b2]
	/// @tparam T	Numeric type
	/// @param x	Value inside [a1, a2] interval to be mapped to [b1, b2] interval
	/// @param a1	Start of the [a1, a2] interval 
	/// @param a2	End of the [a1, a2] interval
	/// @param b1	Start of the [b1, b2] interval 
	/// @param b2	End of the [b1, b2] interval
	/// @return		Remapped value inside [b1, b2] interval 
	template<class T>
	static T LerpRemap(T x, T a1, T a2, T b1, T b2)
	{
		if (a1 > a2)
			std::swap(a1, a2);

		if (b1 > b2)
			std::swap(b1, b2);

		return Lerp(Unlerp(x, a1, a2), b1, b2);
	}
};