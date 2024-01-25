// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "BehaviourTrees/KaosBTDecorator_HasGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/KaosUtilitiesBlueprintLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UKaosBTDecorator_HasGameplayAbility::UKaosBTDecorator_HasGameplayAbility()
{
	NodeName = "Kaos Has Gameplay Ability";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	// Accept only actors
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UKaosBTDecorator_HasGameplayAbility, TargetActorKey), AActor::StaticClass());

	// Default to using Self Actor
	TargetActorKey.SelectedKeyName = FBlackboard::KeySelf;
}

bool UKaosBTDecorator_HasGameplayAbility::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	const AActor* SelectedActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(TargetActorKey.GetSelectedKeyID()));
	if (!SelectedActor)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SelectedActor);
	if (!ASC)
	{
		return false;
	}

	return UKaosUtilitiesBlueprintLibrary::HasAbilityWithAllTags(ASC, FGameplayTagContainer(AbilityTag));
}

FString UKaosBTDecorator_HasGameplayAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: AbilityTag: %s"), *Super::GetStaticDescription(), *AbilityTag.ToString());
}

void UKaosBTDecorator_HasGameplayAbility::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		TargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}
