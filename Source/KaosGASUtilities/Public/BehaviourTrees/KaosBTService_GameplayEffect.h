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
#include "BehaviorTree/BTService.h"
#include "GameplayTagContainer.h"
#include "KaosBTService_GameplayEffect.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

/*
 * Behaviour to do when the Behaviour Service Activates/Deactivates
 */
UENUM(BlueprintType)
enum class EKaosBTServiceGameplayEffectBehaviour : uint8
{
	None			UMETA(DisplayName = "Do Nothing"),
	Apply			UMETA(DisplayName = "Apply a GameplayEffect"),
	Remove			UMETA(DisplayName = "Remove a GameplayEffect")
};

/**
 * Apply or remove a GameplayEffect when service Activates/Deactivates
 */
UCLASS(MinimalAPI, HideCategories=(Service))
class UKaosBTService_GameplayEffect : public UBTService
{
	GENERATED_BODY()

public:
	UKaosBTService_GameplayEffect();
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
protected:
	void ApplyGameplayEffect(UBehaviorTreeComponent& OwnerComp, bool bFromDeactivation);
	void RemoveGameplayEffect(UBehaviorTreeComponent& OwnerComp, bool bFromDeactivation);
	bool CheckGameplayTagQuery(UAbilitySystemComponent* ASC, const FGameplayTagQuery& Query);

	/** What to do when this service activates (becomes relevant) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Activation")
	EKaosBTServiceGameplayEffectBehaviour ActivationBehaviour = EKaosBTServiceGameplayEffectBehaviour::Apply;

	/** What GameplayEffect should we apply to the Activation Effect Target */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Activation", meta = (EditCondition = "ActivationBehaviour != EKaosBTServiceGameplayEffectBehaviour::None"))
	TSubclassOf<UGameplayEffect> ActivationGameplayEffect;

	/** The target of the applied/removed Effect. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Activation", meta = (EditCondition = "ActivationBehaviour != EKaosBTServiceGameplayEffectBehaviour::None"))
	FBlackboardKeySelector ActivationEffectTargetBlackboardKey;

	/** Tag query we perform on the target to allow adding of the Gameplay Effect. If it fails, we will not apply/remove the GE */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Activation", meta = (EditCondition = "ActivationBehaviour != EKaosBTServiceGameplayEffectBehaviour::None"))
	FGameplayTagQuery ActivationEffectTargetTagQuery;

	/** Tag query we perform on the owner (AI) to allow adding of the Gameplay Effect. If it fails, we will not apply/remove the GE */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Activation", meta = (EditCondition = "ActivationBehaviour != EKaosBTServiceGameplayEffectBehaviour::None"))
	FGameplayTagQuery ActivationOwnerTagQuery;

	/** Amount of stacks of the GE to remove (-1 is all stacks) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Activation", meta = (EditCondition = "ActivationBehaviour == EKaosBTServiceGameplayEffectBehaviour::Remove"))
	int32 ActivationEffectStacksToRemove = -1;

	/** What to do when this service activates (becomes not relevant) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Deactivation")
	EKaosBTServiceGameplayEffectBehaviour DeactivationBehaviour = EKaosBTServiceGameplayEffectBehaviour::Remove;

	/** What GameplayEffect should we apply to the Deactivation Effect Target */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Deactivation", meta = (EditCondition = "DeactivationBehaviour != EKaosBTServiceGameplayEffectBehaviour::None"))
	TSubclassOf<UGameplayEffect> DeactivationGameplayEffect;

	/** The target of the applied/removed Effect. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Deactivation", meta = (EditCondition = "DeactivationBehaviour != EKaosBTServiceGameplayEffectBehaviour::None"))
	FBlackboardKeySelector DeactivationEffectTargetBlackboardKey;

	/** Tag query we perform on the target to allow adding of the Gameplay Effect. If it fails, we will not apply/remove the GE */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Deactivation", meta = (EditCondition = "DeactivationBehaviour != EKaosBTServiceGameplayEffectBehaviour::None"))
	FGameplayTagQuery DeactivationEffectTargetTagQuery;

	/** Tag query we perform on the owner (AI) to allow adding of the Gameplay Effect. If it fails, we will not apply/remove the GE */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Deactivation", meta = (EditCondition = "DeactivationBehaviour != EKaosBTServiceGameplayEffectBehaviour::None"))
	FGameplayTagQuery DeactivationOwnerTagQuery;

	/** Amount of stacks of the GE to remove (-1 is all stacks) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffect|Deactivation", meta = (EditCondition = "DeactivationBehaviour != EKaosBTServiceGameplayEffectBehaviour::None"))
	int32 DeactivationEffectStacksToRemove = -1;
};
