// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "BehaviourTrees/KaosBTService_GameplayEffect.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "GameplayEffect.h"
#include "GameplayTagAssetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UKaosBTService_GameplayEffect::UKaosBTService_GameplayEffect()
{
	NodeName = "Kaos Gameplay Effect Service";
	bNotifyTick = false;
	bNotifyOnSearch = false;
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;

	ActivationEffectTargetBlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UKaosBTService_GameplayEffect, ActivationEffectTargetBlackboardKey), AActor::StaticClass());
	DeactivationEffectTargetBlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UKaosBTService_GameplayEffect, DeactivationEffectTargetBlackboardKey), AActor::StaticClass());
	ActivationEffectTargetBlackboardKey.SelectedKeyName = FBlackboard::KeySelf;
	DeactivationEffectTargetBlackboardKey.SelectedKeyName = FBlackboard::KeySelf;
}

void UKaosBTService_GameplayEffect::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		ActivationEffectTargetBlackboardKey.ResolveSelectedKey(*BBAsset);
		DeactivationEffectTargetBlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}


void UKaosBTService_GameplayEffect::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	switch (ActivationBehaviour)
	{
	case EKaosBTServiceGameplayEffectBehaviour::None:
		break;
	case EKaosBTServiceGameplayEffectBehaviour::Apply:
		ApplyGameplayEffect(OwnerComp, false);
		break;
	case EKaosBTServiceGameplayEffectBehaviour::Remove:
		RemoveGameplayEffect(OwnerComp, false);
		break;
	}
}

void UKaosBTService_GameplayEffect::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	switch (DeactivationBehaviour)
	{
	case EKaosBTServiceGameplayEffectBehaviour::None:
		break;
	case EKaosBTServiceGameplayEffectBehaviour::Apply:
		ApplyGameplayEffect(OwnerComp, true);
		break;
	case EKaosBTServiceGameplayEffectBehaviour::Remove:
		RemoveGameplayEffect(OwnerComp, true);
		break;
	}
}

FString UKaosBTService_GameplayEffect::GetStaticDescription() const
{
	FString TagDesc = TEXT("Activation:\n");
	TagDesc += FString::Printf(TEXT("ActivationBehaviour: %s"), *StaticEnum<EKaosBTServiceGameplayEffectBehaviour>()->GetNameStringByValue(static_cast<int64>(ActivationBehaviour)));
	TagDesc += FString::Printf(TEXT("ActivationGameplayEffect: %s\n"), *GetNameSafe(ActivationGameplayEffect));
	TagDesc += FString::Printf(TEXT("ActivationTargetKey: %s\n"), *ActivationEffectTargetBlackboardKey.SelectedKeyName.ToString());
	TagDesc += FString::Printf(TEXT("ActivationEffectTargetTagQuery: %s\n"), *ActivationEffectTargetTagQuery.GetDescription());
	TagDesc += FString::Printf(TEXT("ActivationOwnerTagQuery: %s\n"), *ActivationOwnerTagQuery.GetDescription());
	TagDesc += TEXT("Deactivation:\n");
	TagDesc += FString::Printf(TEXT("DeactivationBehaviour: %s"), *StaticEnum<EKaosBTServiceGameplayEffectBehaviour>()->GetNameStringByValue(static_cast<int64>(DeactivationBehaviour)));
	TagDesc += FString::Printf(TEXT("DeactivationGameplayEffect: %s\n"), *GetNameSafe(DeactivationGameplayEffect));
	TagDesc += FString::Printf(TEXT("DeactivationTargetKey: %s\n"), *DeactivationEffectTargetBlackboardKey.SelectedKeyName.ToString());
	TagDesc += FString::Printf(TEXT("DeactivationEffectTargetTagQuery: %s\n"), *DeactivationEffectTargetTagQuery.GetDescription());
	TagDesc += FString::Printf(TEXT("DeactivationOwnerTagQuery: %s"), *DeactivationOwnerTagQuery.GetDescription());
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *TagDesc);;
}

void UKaosBTService_GameplayEffect::ApplyGameplayEffect(UBehaviorTreeComponent& OwnerComp, bool bFromDeactivation)
{
	const TSubclassOf<UGameplayEffect> ChosenGE = bFromDeactivation ? DeactivationGameplayEffect : ActivationGameplayEffect;
	const FGameplayTagQuery& ChosenOwnerQuery = bFromDeactivation ? DeactivationOwnerTagQuery : ActivationOwnerTagQuery;
	const FBlackboardKeySelector& ChosenEffectBlackboardKey = bFromDeactivation ? DeactivationEffectTargetBlackboardKey : ActivationEffectTargetBlackboardKey;
	const FGameplayTagQuery ChosenBlackboardKeyTagQuery = bFromDeactivation ? DeactivationEffectTargetTagQuery : ActivationEffectTargetTagQuery;

	//No GE? Do nothing.
	if (ChosenGE)
	{
		return;
	}

	UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner()->GetPawn());

	//No owner ASC? Do nothing.
	if (!OwnerASC)
	{
		return;
	}

	//Owner tag query failed? We do nothing.
	if (!CheckGameplayTagQuery(OwnerASC, ChosenOwnerQuery))
	{
		return;
	}

	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ChosenEffectBlackboardKey.SelectedKeyName));
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);

	// No target ASC, do nothing.
	if (!TargetASC)
	{
		return;
	}

	//Target tag query failed? We do nothing.
	if (!CheckGameplayTagQuery(TargetASC, ChosenBlackboardKeyTagQuery))
	{
		return;
	}

	UGameplayEffect* GameplayEffectCDO = ChosenGE->GetDefaultObject<UGameplayEffect>();
	OwnerASC->ApplyGameplayEffectToTarget(GameplayEffectCDO, TargetASC, 1, TargetASC->MakeEffectContext());
}

void UKaosBTService_GameplayEffect::RemoveGameplayEffect(UBehaviorTreeComponent& OwnerComp, bool bFromDeactivation)
{
	const TSubclassOf<UGameplayEffect> ChosenGE = bFromDeactivation ? DeactivationGameplayEffect : ActivationGameplayEffect;
	const FGameplayTagQuery& ChosenOwnerQuery = bFromDeactivation ? DeactivationOwnerTagQuery : ActivationOwnerTagQuery;
	const FBlackboardKeySelector& ChosenEffectBlackboardKey = bFromDeactivation ? DeactivationEffectTargetBlackboardKey : ActivationEffectTargetBlackboardKey;
	const FGameplayTagQuery ChosenBlackboardKeyTagQuery = bFromDeactivation ? DeactivationEffectTargetTagQuery : ActivationEffectTargetTagQuery;
	const int32 StackCountToRemove = bFromDeactivation ? DeactivationEffectStacksToRemove : ActivationEffectStacksToRemove;

	//No GE? can't do anything
	if (!ChosenGE)
	{
		return;
	}

	UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerASC)
	{
		return;
	}

	if (!CheckGameplayTagQuery(OwnerASC, ChosenOwnerQuery))
	{
		return;
	}

	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ChosenEffectBlackboardKey.SelectedKeyName));
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);

	if (TargetASC)
	{
		if (!CheckGameplayTagQuery(TargetASC, ChosenBlackboardKeyTagQuery))
		{
			return;
		}

		TargetASC->RemoveActiveGameplayEffectBySourceEffect(ChosenGE, OwnerASC, StackCountToRemove);
	}
}

bool UKaosBTService_GameplayEffect::CheckGameplayTagQuery(UAbilitySystemComponent* ASC, const FGameplayTagQuery& Query)
{
	if (!Query.IsEmpty() && ASC)
	{
		const IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(ASC);
		if (GameplayTagAssetInterface)
		{
			FGameplayTagContainer Tags;
			GameplayTagAssetInterface->GetOwnedGameplayTags(Tags);
			if (Query.Matches(Tags))
			{
				return true;
			}
		}
	}
	return false;
}
