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
	GameplayTag				UMETA(DisplayName = "Match against a single tag"),
	GameplayTagContainer	UMETA(DisplayName = "Match against a gameplay tag container")
};

/*
 * How we want to match the container if DecoratorMatchType is GameplayTagContainer
 */
UENUM(BlueprintType)
enum class EKaosBTDecoratorGameplayTagContainerMatching : uint8
{
	HasAny					UMETA(DisplayName = "Has any of the tags in the container"),
	HasAll					UMETA(DisplayName = "Has all of the tags in the container"),
	HasAnyExact				UMETA(DisplayName = "Has any of the tags in the container (exact)"),
	HasAllExact				UMETA(DisplayName = "Has all of the tags in the container (exact)"),
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
