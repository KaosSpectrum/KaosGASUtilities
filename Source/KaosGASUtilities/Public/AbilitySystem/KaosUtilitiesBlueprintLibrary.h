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
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "KaosGameplayAbilitySet.h"
#include "UObject/Object.h"
#include "KaosUtilitiesBlueprintLibrary.generated.h"

class UKaosGameplayAbilitySet;
class UGameplayAbility;
class UAbilitySystemComponent;

/**
 * Collection of helper functions for Gameplay Ability System.
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosUtilitiesBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Check to see if we can activate an ability with ALL passed in Tags.
	 * Example: Ability has tags: A.1 and B.1, and GameplayAbilityTags has A.1, it will return true. But if GameplayAbilityTags
	 * has A.1 and C.1, it will return false.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static bool CanActivateAbilityWithMatchingTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags);

	/**
	 * Check to see if an ability is active with matching tags.
	 * Example: Ability has tags: A.1 and B.1, and GameplayAbilityTags has A.1, it will return true. But if GameplayAbilityTags
	 * has A.1 and C.1, it will return false.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static bool HasActiveAbilityWithMatchingTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags);

	/**
	 * Cancels ability with all tags
	 * Example: Ability has tags: A.1 and B.1, and GameplayAbilityTags has A.1, it will return true. But if GameplayAbilityTags
	 * has A.1 and C.1, it will return false.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void CancelAbilityWithAllTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags);

	/**
	 * Checks to see if we have an ability with all tags.
	 * Example: Ability has tags: A.1 and B.1, and GameplayAbilityTags has A.1, it will return true. But if GameplayAbilityTags
	 * has A.1 and C.1, it will return false.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static bool HasAbilityWithAllTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags);

	/**
	 * Checks to see if ability is on cooldown with all tags.
	 * Example: Ability has tags: A.1 and B.1, and GameplayAbilityTags has A.1, it will return true. But if GameplayAbilityTags
	 * has A.1 and C.1, it will return false.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static bool IsAbilityOnCooldownWithAllTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags, float& TimeRemaining, float& Duration);

	/**
	 * Returns true if we can activate ability by the supplied class.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static bool CanActivateAbilityByClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> AbilityClass);

	/**
	 * Returns true if we have an attribute set of class type.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static bool HasAttributeSet(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UAttributeSet> AttributeClass);

	/**
	 * Returns true if a specific ability tag is blocked by the ASC.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static bool IsAbilityTagBlocked(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag AbilityTag);

	/**
	 * Returns true if an ability is active by the passed in handle
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static bool IsAbilityActive(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAbilitySpecHandle& InHandle);

	/**
	 * Returns true if we can activate ability by the supplied class and optional source object.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static bool IsAbilityActiveByClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> AbilityClass, UObject* OptionalSourceObject);

	/**
	 * Returns true if we can apply attribute modifies for a specific Effect Spec.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static bool CanApplyAttributeModifiers(UAbilitySystemComponent* AbilitySystemComponent, FGameplayEffectSpec EffectSpec);

	/**
	 * Will block abilities with the supplied tags
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void BlockAbilitiesWithTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags);

	/**
	 * Will unblock abilities with the supplied tags. (will affect Gameplay Ability blocking tags!)
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void UnblockAbilitiesWithTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags);

	/**
	 * Tries to give an ability set to Actor
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static FKaosAbilitySetHandle GiveAbilitySetToActor(AActor* Actor, UKaosGameplayAbilitySet* Set, UObject* OptionalOverrideSourceObject = nullptr);

	/**
	 * Tries to give ability set to ASC
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static FKaosAbilitySetHandle GiveAbilitySetToASC(UAbilitySystemComponent* AbilitySystemComponent, UKaosGameplayAbilitySet* Set, UObject* OptionalOverrideSourceObject = nullptr);

	/**
	 * Tries to give AbilitySet to interface owner.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static FKaosAbilitySetHandle GiveAbilitySetToInterface(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface, UKaosGameplayAbilitySet* Set, UObject* OptionalOverrideSourceObject = nullptr);

	/**
	 * Will unblock abilities with the supplied tags. (will affect Gameplay Ability blocking tags!)
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void TakeAbilitySet(UPARAM(ref) FKaosAbilitySetHandle& AbilitySetHandle);

	/**
	 * Returns the level of the ability the context was applied with
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static int32 GetAbilityLevel(const FGameplayEffectContextHandle& EffectContextHandle);

	/**
	 * Returns asset tags for a GameplayEffectSpecHandle
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void GetAssetTags(const FGameplayEffectSpecHandle& SpecHandle, FGameplayTagContainer& OutAssetTags);

	/**
	 * Sets the loose tag count directly for a specific tag.
	 */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void SetLooseGameplayTagCount(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag GameplayTag, int32 NewCount);

	/*
	 * These are C++ helper functions below here.
	 *
	 * 
	 */

	/*
	 * Find's an ability spec for a specific class with OptionalSourceObject
	 */
	static FGameplayAbilitySpec* FindAbilitySpecByClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> AbilityClass, UObject* OptionalSourceObject = nullptr);

	/*
	 * Find's an ability spec for an ability with all tags with OptionalSourceObject
	 * Example: Ability has tags: A.1 and B.1, and GameplayAbilityTags has A.1, it will return true. But if GameplayAbilityTags
	 * has A.1 and C.1, it will return false.
	 */
	static FGameplayAbilitySpec* FindAbilitySpecWithAllAbilityTags(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer GameplayAbilityTags, UObject* OptionalSourceObject = nullptr);
};
