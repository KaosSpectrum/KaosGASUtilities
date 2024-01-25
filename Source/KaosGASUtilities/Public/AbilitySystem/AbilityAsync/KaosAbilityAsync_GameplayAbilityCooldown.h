// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync.h"
#include "UObject/Object.h"
#include "KaosAbilityAsync_GameplayAbilityCooldown.generated.h"

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosAbilityAsync_GameplayAbilityCooldown : public UAbilityAsync
{
	GENERATED_BODY()

public:
	/**
	 * Listens for tags being removed and applied to owner for cooldown
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UKaosAbilityAsync_GameplayAbilityCooldown* WaitAbilityCooldown(AActor* TargetActor, FGameplayTag WithAbilityTag, FGameplayTag WithoutAbilityTag, bool OnlyTriggerOnce = false);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UKaosAbilityAsync_GameplayAbilityCooldown* WaitAbilityCooldownClass(AActor* TargetActor, TSubclassOf<UGameplayAbility> AbilityClass, bool OnlyTriggerOnce = false);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UKaosAbilityAsync_GameplayAbilityCooldown* WaitAbilityCooldownSpecificTag(AActor* TargetActor, FGameplayTag CooldownTag, bool OnlyTriggerOnce = false);
	
	 DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKaosAbilityAsync_GameplayAbilityCooldownDelegate, const FGameplayTag&, CooldownTag);
	
	 UPROPERTY(BlueprintAssignable)
	 FKaosAbilityAsync_GameplayAbilityCooldownDelegate CooldownApplied;
	
	UPROPERTY(BlueprintAssignable)
	FKaosAbilityAsync_GameplayAbilityCooldownDelegate CooldownRemoved;
	

protected:
	void HandleTagChanged(FGameplayTag GameplayTag, int32 Count);
	virtual void Activate() override;
	virtual void EndAction() override;
	
	FDelegateHandle MyHandle;
	
	bool bCachedOnlyTriggerOnce;
	
	FGameplayTag CachedWithoutAbilityTag;
	FGameplayTag CachedWithAbilityTag;
	FGameplayTag CachedCooldownTag;

	FGameplayTagContainer CooldownTagsToListenFor;

	TMap<FGameplayTag, FDelegateHandle> BoundASCHandles;
	
	UPROPERTY(Transient)
	TSubclassOf<UGameplayAbility> CachedAbilityClass;
};
