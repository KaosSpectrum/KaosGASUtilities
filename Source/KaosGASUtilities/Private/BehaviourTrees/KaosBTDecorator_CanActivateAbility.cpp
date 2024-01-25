// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "BehaviourTrees/KaosBTDecorator_CanActivateAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/KaosUtilitiesBlueprintLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UKaosBTDecorator_CanActivateAbility::UKaosBTDecorator_CanActivateAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Kaos Can Activate Ability";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	// Accept only actors
	ActorForAbilityActivation.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UKaosBTDecorator_CanActivateAbility, ActorForAbilityActivation), AActor::StaticClass());

	// Default to using Self Actor
	ActorForAbilityActivation.SelectedKeyName = FBlackboard::KeySelf;
}

bool UKaosBTDecorator_CanActivateAbility::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	const AActor* SelectedActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(ActorForAbilityActivation.GetSelectedKeyID()));
	if (!SelectedActor)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SelectedActor);
	if (!ASC)
	{
		return false;
	}

	return UKaosUtilitiesBlueprintLibrary::CanActivateAbilityWithMatchingTags(ASC, FGameplayTagContainer(AbilityTag));
}

void UKaosBTDecorator_CanActivateAbility::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		ActorForAbilityActivation.ResolveSelectedKey(*BBAsset);
	}
}

FString UKaosBTDecorator_CanActivateAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: AbilityTag: %s"), *Super::GetStaticDescription(), *AbilityTag.ToString());
}
