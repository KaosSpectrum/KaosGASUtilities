// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "KaosUtilitiesTypes.generated.h"

class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;

/**
 * FKaosAbilitySet_GameplayAbility
 *
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FKaosAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly, Category=Ability)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly, Category=Ability)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Category=Ability, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


/**
 * FKaosAbilitySet_GameplayEffect
 *
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FKaosAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly, Category=GameplayEffect)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// Level of gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly, Category=GameplayEffect)
	float EffectLevel = 1.0f;
};

/**
 * FKaosAbilitySet_GameplayEffect
 *
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FKaosAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Attribute set to apply
	UPROPERTY(EditDefaultsOnly, Category=AttributeSet)
	TSubclassOf<UAttributeSet> AttributeSet = nullptr;
};

/**
 * FKaosAbilitySetHandle
 *
 *	Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FKaosAbilitySetHandle
{
	GENERATED_BODY()

	bool IsValid() const
	{
		return AbilitySystemComponent.IsValid() && HandleId != 0;
	}

	void RemoveSet();

private:
	friend class UKaosGameplayAbilitySet;
	friend class UKaosUtilitiesBlueprintLibrary;

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Attribute Sets
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> AttributeSets;

	int32 HandleId = 0;

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	void Reset()
	{
		AbilitySpecHandles.Reset();
		GameplayEffectHandles.Reset();
		AbilitySystemComponent.Reset();
		HandleId = 0;
	}
};
