// Copyright © Daniel Moss
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the disclaimer
// below) provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
// THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
// NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "AnimationNotify/KaosAnimNotify_TriggerGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "KaosUtilitiesBlueprintLibrary.h"

UKaosAnimNotify_TriggerGameplayAbility::UKaosAnimNotify_TriggerGameplayAbility(): bSetNextMontageSectionIfAbilityFailed(false)
{
	bIsNativeBranchingPoint = true;
}

void UKaosAnimNotify_TriggerGameplayAbility::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	AActor* Actor = MeshComp ? MeshComp->GetOwner() : nullptr;
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
	if (!ASC)
	{
		HandleJumpSection(MeshComp, nullptr, Cast<UAnimMontage>(BranchingPointPayload.SequenceAsset));
		return;
	}

	if (ASC->IsOwnerActorAuthoritative())
	{
		TArray<FGameplayAbilitySpec*> OutSpecs;
		ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(AbilityTriggerTag), OutSpecs);
		for (const FGameplayAbilitySpec* Spec : OutSpecs)
		{
			const bool bValidAbility = Spec->Ability != nullptr;
			const bool bTagsPass = Spec->Ability->AbilityTags.HasAll(FGameplayTagContainer(AbilityTriggerTag));
			if (bValidAbility && bTagsPass && ASC->TryActivateAbility(Spec->Handle, false))
			{
				//We are done, 
				return;
			}
		}
	
		HandleJumpSection(MeshComp, ASC, Cast<UAnimMontage>(BranchingPointPayload.SequenceAsset));
	}
}

FString UKaosAnimNotify_TriggerGameplayAbility::GetNotifyName_Implementation() const
{
	return AbilityTriggerTag.ToString();
}

#if WITH_EDITOR
bool UKaosAnimNotify_TriggerGameplayAbility::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}
#endif

void UKaosAnimNotify_TriggerGameplayAbility::HandleJumpSection(USkeletalMeshComponent* MeshComponent, UAbilitySystemComponent* ASC, UAnimMontage* CurrentMontage)
{
	//Should we handle GAS(?)
	if (bSetNextMontageSectionIfAbilityFailed)
	{
		const FName CurrentMontageSectionName = MeshComponent->GetAnimInstance()->Montage_GetCurrentSection(CurrentMontage);
		if (ASC && ASC->GetCurrentMontage() == CurrentMontage)
		{
			ASC->CurrentMontageSetNextSectionName(CurrentMontageSectionName, NextMontageSectionIfAbilityFailed);
		}
		else
		{
			MeshComponent->GetAnimInstance()->Montage_SetNextSection(CurrentMontageSectionName, NextMontageSectionIfAbilityFailed, CurrentMontage);
		}
	}
}
