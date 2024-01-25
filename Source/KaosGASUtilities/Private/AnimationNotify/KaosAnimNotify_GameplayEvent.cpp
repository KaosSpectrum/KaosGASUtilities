// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "AnimationNotify/KaosAnimNotify_GameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"

UKaosAnimNotify_GameplayEvent::UKaosAnimNotify_GameplayEvent()
{
	bIsNativeBranchingPoint = true;
}

void UKaosAnimNotify_GameplayEvent::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	if (USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (AActor* Actor = MeshComp->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, FGameplayEventData());
		}
	}
}

FString UKaosAnimNotify_GameplayEvent::GetNotifyName_Implementation() const
{
	return EventTag.ToString();
}
