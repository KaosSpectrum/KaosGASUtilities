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
