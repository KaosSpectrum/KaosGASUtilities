// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/Object.h"
#include "KaosAbilitySystemComponent.generated.h"

class UKaosAbilityTagRelationships;
DECLARE_DELEGATE_OneParam(FKaosOnGiveAbility, FGameplayAbilitySpec&);

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;
	
	/** Helper function for blueprint to get abilities TargetData */
	UFUNCTION(BlueprintCallable)
	void GetAbilityTargetData(FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle) const;

	/** Allow blueprints to unblock abilities */
	UFUNCTION(BlueprintCallable, DisplayName = "UnBlock Abilities With Tags")
	void K2_UnBlockAbilitiesWithTags(UPARAM(ref) FGameplayTagContainer& Tags);

	/** Allow blueprints to block abilities */
	UFUNCTION(BlueprintCallable, DisplayName = "Block Abilities With Tags")
	void K2_BlockAbilitiesWithTags(UPARAM(ref) FGameplayTagContainer& Tags);

	/** Return a mutable pointer to the ActiveGameplayEffect from the supplied handle. */
	FActiveGameplayEffect* GetActiveGameplayEffect_Mutable(FActiveGameplayEffectHandle Handle);

	/** Returns all active gameplay effect handles */
	TArray<FActiveGameplayEffectHandle> GetAllActiveEffectHandles() const;

	/** Accessor for the OnGiveAbility delegate */
	FKaosOnGiveAbility& GetKaosOnGiveAbilityDelegate() { return KaosOnGiveAbility; }

	/** Returns the relationship for activation requirements from the supplied ability tags */
	virtual void GetRelationshipActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const;

protected:
	//Returns the ability tag relationship data asset, overridable by game's to provide a different a different asset to the default ASC one
	virtual const UKaosAbilityTagRelationships* GetAbilityTagRelationships() const;
	
	/** Notify client that an ability failed to activate */
	UFUNCTION(Client, Unreliable)
	void ClientNotifyAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);

	/** Notify the ability it failed */
	virtual void HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);

	/** Callback when an ability is given */
	FKaosOnGiveAbility KaosOnGiveAbility;

	//Mapping of abilities tags to block and cancel tags. Can be overriden using GetAbilityTagRelationships()
	UPROPERTY(EditDefaultsOnly, Category = "Relationship")
	TObjectPtr<UKaosAbilityTagRelationships> AbilityTagRelationship;
};
