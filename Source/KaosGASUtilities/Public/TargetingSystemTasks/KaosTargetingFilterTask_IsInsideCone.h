// Copyright (C) 2024, Daniel Moss
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Tasks/TargetingFilterTask_BasicFilterTemplate.h"
#include "UObject/Object.h"
#include "KaosTargetingFilterTask_IsInsideCone.generated.h"

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosTargetingFilterTask_IsInsideCone : public UTargetingFilterTask_BasicFilterTemplate
{
	GENERATED_BODY()

public:
	//~ Begin UTargetingFilterTask_BasicFilterTemplate Interface
	virtual bool ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const override;
	//~ End UTargetingFilterTask_BasicFilterTemplate Interface

	UFUNCTION(BlueprintNativeEvent)
	float GetConeHalfAngle(const FTargetingRequestHandle& TargetingHandle) const;

protected:
	FVector GetSourceLocation(const FTargetingRequestHandle& TargetingHandle) const;
	FVector GetSourceDirection(const FTargetingRequestHandle& TargetingHandle) const;


	UPROPERTY(EditAnywhere)
	FScalableFloat DefaultConeHalfAngle = 45.f;

	FORCEINLINE bool IsInsideCone(const FVector& ConeOrigin, const FVector& ConeDirection, float ConeHalfAngle, const FVector& Point) const
	{
		//Dot product between ConeDir and Point - ConeOrigin
		const float Dot = ConeDirection | (Point - ConeOrigin);
		const float CosineSquared = FMath::Pow(Dot, 2) / (FVector::DistSquared(ConeOrigin, Point) * ConeDirection.SizeSquared());

		const float CosineAngleSquared = FMath::Pow(FMath::Cos(ConeHalfAngle / 180.0f * PI), 2);

		return Dot > 0 && CosineSquared >= CosineAngleSquared;
	}
};
