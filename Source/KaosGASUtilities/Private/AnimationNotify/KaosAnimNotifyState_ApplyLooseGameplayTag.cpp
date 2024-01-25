// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "AnimationNotify/KaosAnimNotifyState_ApplyLooseGameplayTag.h"

#include "AbilitySystemBlueprintLibrary.h"

UKaosAnimNotifyState_ApplyLooseGameplayTag::UKaosAnimNotifyState_ApplyLooseGameplayTag()
{
}

void UKaosAnimNotifyState_ApplyLooseGameplayTag::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	AActor* Actor = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (Actor)
	{
		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Actor, GameplayTagsToApply, false);
	}
}

void UKaosAnimNotifyState_ApplyLooseGameplayTag::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	AActor* Actor = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (Actor)
	{
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Actor, GameplayTagsToApply, false);
	}
}

FString UKaosAnimNotifyState_ApplyLooseGameplayTag::GetNotifyName_Implementation() const
{
	return GameplayTagsToApply.ToStringSimple();
}
