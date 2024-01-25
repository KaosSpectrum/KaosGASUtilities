// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "TargetingSystemTasks/KaosTargetingFilterTask_IsInsideCone.h"

bool UKaosTargetingFilterTask_IsInsideCone::ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const
{
	const float ConeHalfAngle = GetConeHalfAngle(TargetingHandle);
	const FVector SourceLocation = GetSourceLocation(TargetingHandle);
	const FVector SourceDirection = GetSourceDirection(TargetingHandle);

	if (const AActor* TargetActor = TargetData.HitResult.GetActor())
	{
		if (IsInsideCone(SourceLocation, SourceDirection, ConeHalfAngle, TargetActor->GetActorLocation()))
		{
			return true;
		}
	}
	return false;
}

float UKaosTargetingFilterTask_IsInsideCone::GetConeHalfAngle_Implementation(const FTargetingRequestHandle& TargetingHandle) const
{
	return DefaultConeHalfAngle.GetValue();
}

FVector UKaosTargetingFilterTask_IsInsideCone::GetSourceLocation(const FTargetingRequestHandle& TargetingHandle) const
{
	FVector SourceLocation = FVector::ZeroVector;
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (SourceContext->SourceActor)
		{
			SourceLocation = SourceContext->SourceActor->GetActorLocation();
		}
		else if (!SourceContext->SourceLocation.IsZero())
		{
			SourceLocation = SourceContext->SourceLocation;
		}
	}

	return SourceLocation;
}

FVector UKaosTargetingFilterTask_IsInsideCone::GetSourceDirection(const FTargetingRequestHandle& TargetingHandle) const
{
	FVector SourceLocation = FVector::ZeroVector;
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (SourceContext->SourceActor)
		{
			SourceLocation = SourceContext->SourceActor->GetActorForwardVector();
		}
		else if (!SourceContext->SourceLocation.IsZero())
		{
			SourceLocation = FVector::ForwardVector;
		}
	}

	return SourceLocation;
}
