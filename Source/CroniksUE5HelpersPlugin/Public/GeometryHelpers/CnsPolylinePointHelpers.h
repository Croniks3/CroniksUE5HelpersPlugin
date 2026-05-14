#pragma once

#include "CoreMinimal.h"
#include <type_traits>

namespace CnsGeometryHelpers
{
	template<typename T>
	FORCEINLINE void RemoveClosingDuplicatePoint(
		TArray<T>& PolylinePoints, 
		const double Tolerance = UE_KINDA_SMALL_NUMBER,
		const EAllowShrinking AllowShrinking = EAllowShrinking::No
	)
	{
		using CleanT = std::remove_cv_t<std::remove_reference_t<T>>;

		static_assert(
			std::is_same_v<CleanT, FVector>
			|| std::is_same_v<CleanT, FVector2D>
			|| std::is_same_v<CleanT, FVector4>
			|| std::is_same_v<CleanT, FVector2f>
			|| std::is_same_v<CleanT, FVector3f>
			|| std::is_same_v<CleanT, FVector4f>
			|| std::is_same_v<CleanT, FVector2d>
			|| std::is_same_v<CleanT, FVector3d>
			|| std::is_same_v<CleanT, FVector4d>,
			"T must be an Unreal vector type"
		);
		
		if (PolylinePoints.Num() < 2)
		{
			return;
		}
		
		const T& First = PolylinePoints[0];
		const T& Last = PolylinePoints.Last();

		if (First.Equals(Last, Tolerance))
		{
			PolylinePoints.RemoveAt(PolylinePoints.Num() - 1, 1, AllowShrinking);
		}
	}
	
	/**
	* Computes signed 2D polygon area using X/Y coordinates.
	*
	* @return Positive for one winding direction, negative for the opposite one.
	*/
	template<typename T>
	double ComputeSignedArea2D(const TArray<T>& PolylinePoints)
	{
		using CleanT = std::remove_cv_t<std::remove_reference_t<T>>;

		static_assert(
			std::is_same_v<CleanT, FVector>
			|| std::is_same_v<CleanT, FVector2D>
			|| std::is_same_v<CleanT, FVector4>
			|| std::is_same_v<CleanT, FVector2f>
			|| std::is_same_v<CleanT, FVector3f>
			|| std::is_same_v<CleanT, FVector4f>
			|| std::is_same_v<CleanT, FVector2d>
			|| std::is_same_v<CleanT, FVector3d>
			|| std::is_same_v<CleanT, FVector4d>,
			"T must be an Unreal vector type"
		);

		const int32 Num = PolylinePoints.Num();

		if (Num < 3)
		{
			return 0.0;
		}

		double TwiceArea = 0.0;

		for (int32 Index = 0; Index < Num - 1; ++Index)
		{
			const T& A = PolylinePoints[Index];
			const T& B = PolylinePoints[Index + 1];

			TwiceArea += static_cast<double>(A.X) * static_cast<double>(B.Y)
				- static_cast<double>(B.X) * static_cast<double>(A.Y);
		}

		const T& Last = PolylinePoints[Num - 1];
		const T& First = PolylinePoints[0];

		TwiceArea += static_cast<double>(Last.X) * static_cast<double>(First.Y)
			- static_cast<double>(First.X) * static_cast<double>(Last.Y);

		return TwiceArea * 0.5;
	}
}