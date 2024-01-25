// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "BehaviourTrees/KaosBTService_ActivateAbilityByTag.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/KaosUtilitiesBlueprintLibrary.h"
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
	return FString::Printf(TEXT("%s: AbilityTag: %s\nCease Releavance: %s"), *Super::GetStaticDescription(), *AbilityToActivate.ToString(),
	                       *StaticEnum<EKaosActivateAbilityByTagOnCeaseRelevanceBehaviour>()->GetNameStringByValue(static_cast<int64>(NodeCeaseRelevanceBehaviour)));
}
