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

#include "BehaviourTrees/KaosBTService_ActivateAbilityByTag.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "KaosUtilitiesBlueprintLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UKaosBTService_ActivateAbilityByTag::UKaosBTService_ActivateAbilityByTag()
{
	NodeName = "Kaos Activate Ability With GameplayTag";
	bNotifyTick = true;
	bNotifyCeaseRelevant = true;

	// Accept only actors
	TargetBlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UKaosBTService_ActivateAbilityByTag, TargetBlackboardKey), AActor::StaticClass());

	// Default to using Self Actor
	TargetBlackboardKey.SelectedKeyName = FBlackboard::KeySelf;
}

void UKaosBTService_ActivateAbilityByTag::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	const AActor* SelectedActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(TargetBlackboardKey.GetSelectedKeyID()));
	if (!SelectedActor)
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SelectedActor);
	if (!ASC)
	{
		return;
	}
	
	if (UKaosUtilitiesBlueprintLibrary::HasActiveAbilityWithMatchingTags(ASC, FGameplayTagContainer(AbilityToActivate)))
	{
		//Do nothing as the ability is active
		return;
	}
	
	if (!UKaosUtilitiesBlueprintLibrary::CanActivateAbilityWithMatchingTags(ASC, FGameplayTagContainer(AbilityToActivate)))
	{
		//Do nothing if we can not activate the ability
		return;
	}

	//Activate the ability
	ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityToActivate));
}

void UKaosBTService_ActivateAbilityByTag::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	if (NodeCeaseRelevanceBehaviour == EKaosActivateAbilityByTagOnCeaseRelevanceBehaviour::CancelAbility)
	{
		const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (!BlackboardComp)
		{
			return;
		}

		const AActor* SelectedActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(TargetBlackboardKey.GetSelectedKeyID()));
		if (!SelectedActor)
		{
			return;
		}

		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SelectedActor);
		if (!ASC)
		{
			return;
		}
		
		UKaosUtilitiesBlueprintLibrary::CancelAbilityWithAllTags(ASC, FGameplayTagContainer(AbilityToActivate));
	}
}

FString UKaosBTService_ActivateAbilityByTag::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: AbilityTag: %s\nCease Releavance: %s"), *Super::GetStaticDescription(), *AbilityToActivate.ToString(), *StaticEnum<EKaosActivateAbilityByTagOnCeaseRelevanceBehaviour>()->GetNameStringByValue(static_cast<int64>(NodeCeaseRelevanceBehaviour)));
}
