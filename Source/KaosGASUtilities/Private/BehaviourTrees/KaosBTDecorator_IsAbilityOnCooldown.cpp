// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "BehaviourTrees/KaosBTDecorator_IsAbilityOnCooldown.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/KaosUtilitiesBlueprintLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UKaosBTDecorator_IsAbilityOnCooldown::UKaosBTDecorator_IsAbilityOnCooldown()
{
	NodeName = "Kaos Is Ability On Cooldown";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	// Accept only actors
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UKaosBTDecorator_IsAbilityOnCooldown, TargetActorKey), AActor::StaticClass());

	// Default to using Self Actor
	TargetActorKey.SelectedKeyName = FBlackboard::KeySelf;
}

bool UKaosBTDecorator_IsAbilityOnCooldown::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	float TimeRemaining, Duration; // Not used, but needed for the function
	return UKaosUtilitiesBlueprintLibrary::IsAbilityOnCooldownWithAllTags(ASC, FGameplayTagContainer(AbilityTag), TimeRemaining, Duration);
}

FString UKaosBTDecorator_IsAbilityOnCooldown::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: AbilityTag: %s"), *Super::GetStaticDescription(), *AbilityTag.ToString());
}

void UKaosBTDecorator_IsAbilityOnCooldown::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		TargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}
