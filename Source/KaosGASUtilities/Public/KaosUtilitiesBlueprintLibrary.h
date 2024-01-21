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
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "KaosUtilitiesBlueprintLibrary.generated.h"

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
	static bool IsAbilityOnCooldownWithAllTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags);

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
