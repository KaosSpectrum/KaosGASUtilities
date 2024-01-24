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
