// Copyright (C) 2024, Daniel Moss
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

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
