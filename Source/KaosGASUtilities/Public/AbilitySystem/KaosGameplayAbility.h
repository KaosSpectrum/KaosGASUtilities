// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UObject/Object.h"
#include "KaosGameplayAbility.generated.h"

class UKaosAbilityCosts;

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UKaosGameplayAbility();

	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
												   FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
									FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	/** Returns the cooldown tags for this ability */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Cooldown Tags"), Category=Ability)
	FGameplayTagContainer K2_GetCooldownTags() const;

	/** Returns the ability tags for this ability */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Cooldown Tags"), Category=Ability)
	FGameplayTagContainer GetAbilityTags() const;

	/** Checks to see if Target has the supplied tags */
	UFUNCTION(BlueprintCallable, Category=Ability)
	bool DoesTargetHaveTags(const AActor* Target, const FGameplayTagContainer& Tags) const;
	
	/** If the ability should be removed after activation */
	virtual bool ShouldRemoveAfterActivation() const { return bRemoveAfterActivation; };

	/** Called when the ability failed to activate */
	virtual void OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	/** If the ability is auto activated (passive) */
	virtual bool IsPassiveAbility() const { return bPassiveAbility; }

	/** Returns true is ability is blocked by tags */
	virtual bool IsAbilityBlockedByTags(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const;
	
	/** If the ability starts on cooldown */
	virtual bool DoesStartOnCooldown() const { return bStartWithCooldown; }

	/** Returns array of addition game specific costs to apply and check */
	virtual TArray<TObjectPtr<UKaosAbilityCosts>> GetAdditionalCosts() const { return AdditionalCosts; }

	/** Does the cost only apply on hit */
	virtual bool DoesApplyCostOnlyOnHit() const { return bOnlyApplyCostOnHit; }
	
	/** Allows child classes to handle adding additional relevent info. */
	virtual void NotifyAbilityBlocked(const FGameplayTagContainer& AbilitySystemComponentTags, FGameplayTagContainer* OptionalRelevantTags) const { }
	
	/** Called to try to active the passive ability */
	virtual void TryActivatePassiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;
	
	/** Called when the ability fails to activate */
	virtual void NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	/** Called when a pawn avatar is set */
	virtual void OnPawnAvatarSet();
	
	/** Called when a passive ability fails to activate */
	UFUNCTION()
	void NotifyFailedToActivatePassiveAbility() const;

	/** Used to determine if ability target data hit a target */
	virtual bool DetermineIfAbilityHitTarget(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;
	
protected:
	/** If the ability starts on cooldown already */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Advanced)
	bool bStartWithCooldown = false;

	/** Remove this ability after activation. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Advanced)
	bool bRemoveAfterActivation;

	/** if the ability auto activates when given (Passive Ability) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Advanced)
	bool bPassiveAbility;

	// Additional costs that must be paid to activate this ability
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Costs)
	TArray<TObjectPtr<UKaosAbilityCosts>> AdditionalCosts;

	// If we apply the cost GE only on hits or all the time.
	UPROPERTY(EditDefaultsOnly, Category = Costs)
	bool bOnlyApplyCostOnHit;
	
	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "On Ability Added")
	void K2_OnAbilityAdded();

	/** Called when the ability system is initialized with a pawn avatar. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();
	
	/** Called when this ability is removed from the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "On Ability Removed")
	void K2_OnAbilityRemoved();

	/** Called when the ability fails to activate */
	UFUNCTION(BlueprintImplementableEvent, Category=Ability, meta = (DisplayName = "On Ability Failed To Activate"))
	void K2_OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;
	
	/** Called when a passive ability fails to activate */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "Failed to Activate Passive Ability")
	void K2_FailedToActivatePassiveAbility() const;
	
	/** Check if ability is blocked by tags */
	UFUNCTION(BlueprintCallable, Category=Ability, DisplayName = "IsAbilityBlockedByTags")
	bool K2_IsAbilityBlockedByTags() const;
	
	friend class UKaosAbilitySystemComponent;
};
