// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

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
