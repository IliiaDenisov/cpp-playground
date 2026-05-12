#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>
#include "Points.h"

namespace PointHelper
{
	template<typename T>
	struct PointTraits
	{
		using CoordinateType = std::remove_reference_t<decltype(std::declval<T>().x)>;

		static auto& x(T& point) { return point.x; }
		static auto x(const T& point) { return point.x; }

		static auto& y(T& point) { return point.y; }
		static auto y(const T& point) { return point.y; }

	};

	template<>
	struct PointTraits<PointFloat>
	{
		using CoordinateType = std::remove_reference_t<decltype(std::declval<PointFloat>().X)>;
		
		static auto& x(PointFloat& point) { return point.X; }
		static auto x(const PointFloat& point) { return point.X; }

		static auto& y(PointFloat& point) { return point.Y; }
		static auto y(const PointFloat& point) { return point.Y; }
	};

	template <typename TCurrentType, typename TDesiredType>
	TDesiredType Convert(const TCurrentType& point)
	{
		using BaseCoordinateType = PointTraits<TCurrentType>::CoordinateType;
		using TargetCoordinateType =  PointTraits<TDesiredType>::CoordinateType;

		constexpr bool floatToIntegral = std::is_floating_point_v<BaseCoordinateType> && std::is_integral_v<TargetCoordinateType>;

		TDesiredType convertedPoint;

		if constexpr (floatToIntegral)
		{
			PointTraits<TDesiredType>::x(convertedPoint) = static_cast<TargetCoordinateType>(std::round(PointTraits<TCurrentType>::x(point)));
			PointTraits<TDesiredType>::y(convertedPoint) = static_cast<TargetCoordinateType>(std::round(PointTraits<TCurrentType>::y(point)));
		}
		else
		{
			PointTraits<TDesiredType>::x(convertedPoint) = static_cast<TargetCoordinateType>(PointTraits<TCurrentType>::x(point));
			PointTraits<TDesiredType>::y(convertedPoint) = static_cast<TargetCoordinateType>(PointTraits<TCurrentType>::y(point));
		}
		return convertedPoint;
	}

	template <typename TCurrentType, typename TDesiredType>
	std::vector<TDesiredType> ConvertPoints(const std::vector<TCurrentType>& points)
	{
		std::vector<TDesiredType> convertedPoints(points.size());
		std::transform(points.begin(), points.end(), convertedPoints.begin(),
			[](const TCurrentType& point)
			{
				return Convert<TCurrentType, TDesiredType>(point);
			}
		);
		return convertedPoints;
	}
}
