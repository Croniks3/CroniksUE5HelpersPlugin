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
}