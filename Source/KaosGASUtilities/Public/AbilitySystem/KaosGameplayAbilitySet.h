// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "KaosUtilitiesTypes.h"
#include "UObject/Object.h"
#include "KaosGameplayAbilitySet.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;


/**
 * 
 */
UCLASS(MinimalAPI)
class UKaosGameplayAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UKaosGameplayAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	KAOSGASUTILITIES_API const TArray<FKaosAbilitySet_GameplayAbility>& GetGrantedGameplayAbilities() const { return GrantedGameplayAbilities; }
	KAOSGASUTILITIES_API const TArray<FKaosAbilitySet_GameplayEffect>& GetGrantedGameplayEffects() const { return GrantedGameplayEffects; }

protected:
	KAOSGASUTILITIES_API virtual FKaosAbilitySetHandle GiveAbilitySetTo(UAbilitySystemComponent* ASC, UObject* OverrideSourceObject = nullptr) const;
	KAOSGASUTILITIES_API virtual FKaosAbilitySetHandle GiveAbilitySetToInterface(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface, UObject* OverrideSourceObject = nullptr) const;

	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FKaosAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FKaosAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=AttributeSet))
	TArray<FKaosAbilitySet_AttributeSet> GrantedAttributeSets;

	friend class UKaosUtilitiesBlueprintLibrary;
};
