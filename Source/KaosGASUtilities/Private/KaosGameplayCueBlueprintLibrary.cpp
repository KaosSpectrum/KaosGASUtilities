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


#include "KaosGameplayCueBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UKaosGameplayCueBlueprintLibrary::AddGameplayCueLocal(AActor* Target, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& CueParameters)
{
	if (!Target)
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (!ASC)
	{
		return;
	}
	
	const FStructProperty* ActiveGameplayCuesStructProp = CastField<FStructProperty>(FActiveGameplayCueContainer::StaticStruct()->FindPropertyByName("ActiveGameplayCues"));
	if (ActiveGameplayCuesStructProp)
	{
		FActiveGameplayCueContainer* ActiveGameplayCueContainer = ActiveGameplayCuesStructProp->ContainerPtrToValuePtr<FActiveGameplayCueContainer>(ASC);
		if (ActiveGameplayCueContainer)
		{
			ActiveGameplayCueContainer->AddCue(GameplayCueTag, FPredictionKey(), CueParameters);
			ASC->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::OnActive, CueParameters);
			ASC->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::WhileActive, CueParameters);
		}
	}
}

void UKaosGameplayCueBlueprintLibrary::RemoveGameplayCueLocal(AActor* Target, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& CueParameters)
{
	if (!Target)
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (!ASC)
	{
		return;
	}
	
	const FStructProperty* ActiveGameplayCuesStructProp = CastField<FStructProperty>(FActiveGameplayCueContainer::StaticStruct()->FindPropertyByName("ActiveGameplayCues"));
	if (ActiveGameplayCuesStructProp)
	{
		FActiveGameplayCueContainer* ActiveGameplayCueContainer = ActiveGameplayCuesStructProp->ContainerPtrToValuePtr<FActiveGameplayCueContainer>(ASC);
		if (ActiveGameplayCueContainer)
		{
			ActiveGameplayCueContainer->RemoveCue(GameplayCueTag);
			ASC->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::Removed, CueParameters);
		}
	}
}

void UKaosGameplayCueBlueprintLibrary::ExecuteGameplayCueLocal(AActor* Target, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& CueParameters)
{
	if (!Target)
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (!ASC)
	{
		return;
	}

	ASC->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::Executed, CueParameters);
}

void UKaosGameplayCueBlueprintLibrary::BuildCueParametersFromSource(AActor* SourceActor, FGameplayCueParameters& OutCueParameters)
{
	if (!SourceActor)
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(SourceActor);
	FGameplayTagContainer SourceTags;
	if (ASC)
	{
		ASC->GetOwnedGameplayTags(SourceTags);
	}
	else if (GameplayTagAssetInterface)
	{
		GameplayTagAssetInterface->GetOwnedGameplayTags(SourceTags);
	}
	OutCueParameters.AggregatedSourceTags.AppendTags(SourceTags);
	OutCueParameters.Location = SourceActor->GetActorLocation();
	OutCueParameters.Instigator = SourceActor;
}

void UKaosGameplayCueBlueprintLibrary::BuildCueParametersFromHitResult(AActor* SourceActor, FHitResult& HitResult, FGameplayCueParameters& OutCueParameters)
{
	if (!SourceActor)
	{
		return;
	}

	const UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SourceActor);
	const IGameplayTagAssetInterface* SourceGameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(SourceActor);
	FGameplayTagContainer SourceTags;
	
	if (SourceASC)
	{
		SourceASC->GetOwnedGameplayTags(SourceTags);
	}
	else if (SourceGameplayTagAssetInterface)
	{
		SourceGameplayTagAssetInterface->GetOwnedGameplayTags(SourceTags);
	}
	
	const UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitResult.GetActor());
	const IGameplayTagAssetInterface* TargetGameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(HitResult.GetActor());
	FGameplayTagContainer TargetTags;
	
	if (TargetASC)
	{
		TargetASC->GetOwnedGameplayTags(TargetTags);
	}
	else if (TargetGameplayTagAssetInterface)
	{
		TargetGameplayTagAssetInterface->GetOwnedGameplayTags(TargetTags);
	}
	
	OutCueParameters.AggregatedSourceTags.AppendTags(SourceTags);
	OutCueParameters.AggregatedTargetTags.AppendTags(TargetTags);
	OutCueParameters.Location = HitResult.Location;
	OutCueParameters.Normal = HitResult.Normal;
	OutCueParameters.Instigator = SourceActor;
	OutCueParameters.PhysicalMaterial = HitResult.PhysMaterial;
}
