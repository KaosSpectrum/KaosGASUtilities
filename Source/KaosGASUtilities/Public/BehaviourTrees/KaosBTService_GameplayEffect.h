// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

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
	None UMETA(DisplayName = "Do Nothing"),
	Apply UMETA(DisplayName = "Apply a GameplayEffect"),
	Remove UMETA(DisplayName = "Remove a GameplayEffect")
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
