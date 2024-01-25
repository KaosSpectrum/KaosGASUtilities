// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GameplayTagContainer.h"
#include "KaosBTDecorator_GameplayTag.generated.h"

/*
 * If we want to match against a single tag or a container
 */
UENUM(BlueprintType)
enum class EKaosBTDecoratorMatchType : uint8
{
	GameplayTag UMETA(DisplayName = "Match against a single tag"),
	GameplayTagContainer UMETA(DisplayName = "Match against a gameplay tag container")
};

/*
 * How we want to match the container if DecoratorMatchType is GameplayTagContainer
 */
UENUM(BlueprintType)
enum class EKaosBTDecoratorGameplayTagContainerMatching : uint8
{
	HasAny UMETA(DisplayName = "Has any of the tags in the container"),
	HasAll UMETA(DisplayName = "Has all of the tags in the container"),
	HasAnyExact UMETA(DisplayName = "Has any of the tags in the container (exact)"),
	HasAllExact UMETA(DisplayName = "Has all of the tags in the container (exact)"),
};

/**
 * Decorator to match against GameplayTags.
 * Conditions is hidden due this being a query.
 */
UCLASS(MinimalAPI, HideCategories=(Condition))
class UKaosBTDecorator_GameplayTag : public UBTDecorator
{
	GENERATED_BODY()

public:
	UKaosBTDecorator_GameplayTag(const FObjectInitializer& ObjectInitializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;


	virtual FString GetStaticDescription() const override;

	virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;

protected:
	/** Target we are checking for tags */
	UPROPERTY(EditAnywhere, Category="GameplayTag", Meta=(ToolTips="Which Actor (from the blackboard) should be checked for this gameplay tag(s) check?"))
	FBlackboardKeySelector ActorForGameplayTagCheck;

	/** Do we want to match single tag or container */
	UPROPERTY(EditAnywhere, Category="GameplayTag")
	EKaosBTDecoratorMatchType GameplayTagMatchType = EKaosBTDecoratorMatchType::GameplayTag;

	/** Gameplay tag used in Gameplay Tag matching type: GameplayTag */
	UPROPERTY(EditAnywhere, Category="GameplayTag", meta = (EditCondition = "GameplayTagMatchType == EKaosBTDecoratorMatchType::GameplayTag"))
	FGameplayTag GameplayTag;

	/** If the tag check is exact */
	UPROPERTY(EditAnywhere, Category="GameplayTag", meta = (EditCondition = "GameplayTagMatchType == EKaosBTDecoratorMatchType::GameplayTag"))
	bool bExact = false;

	/** Gameplay tags used in Gameplay Tag matching type: GameplayTagContainer */
	UPROPERTY(EditAnywhere, Category="GameplayTag", meta = (EditCondition = "GameplayTagMatchType == EKaosBTDecoratorMatchType::GameplayTagContainer"))
	FGameplayTagContainer GameplayTags;

	/** How we want to match the container */
	UPROPERTY(EditAnywhere, Category="GameplayTag", meta = (EditCondition = "GameplayTagMatchType == EKaosBTDecoratorMatchType::GameplayTagContainer"))
	EKaosBTDecoratorGameplayTagContainerMatching GameplayTagContainerMatchingType = EKaosBTDecoratorGameplayTagContainerMatching::HasAny;

	bool MatchContainer(const FGameplayTagContainer& SelectedGameplayTags) const;

	/** Callback for when a tag is updated (added/removed) */
	void OnGameplayTagsChanged(const FGameplayTag InTag, int32 NewCount, TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent, uint8* NodeMemory);


	/** called when execution flow controller becomes active */
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** called when execution flow controller becomes inactive */
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
};
