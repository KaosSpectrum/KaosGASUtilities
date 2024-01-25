// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "AbilitySystem/KaosGameplayAbilitySet.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "KaosUtilitiesLogging.h"
#include "Abilities/GameplayAbility.h"

namespace AresAbilitySetHandle_Impl
{
	static int32 LastHandleId = 0;
	static int32 GetNextQueuedHandleIdForUse() { return ++LastHandleId; }
}


UKaosGameplayAbilitySet::UKaosGameplayAbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FKaosAbilitySetHandle UKaosGameplayAbilitySet::GiveAbilitySetTo(UAbilitySystemComponent* ASC, UObject* OverrideSourceObject) const
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return FKaosAbilitySetHandle();
	}

	FKaosAbilitySetHandle OutHandle;
	OutHandle.HandleId = AresAbilitySetHandle_Impl::GetNextQueuedHandleIdForUse();
	OutHandle.AbilitySystemComponent = ASC;

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FKaosAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogKaosUtilities, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = OverrideSourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);
		OutHandle.AddAbilitySpecHandle(AbilitySpecHandle);
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FKaosAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogKaosUtilities, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, ASC->MakeEffectContext());
		OutHandle.AddGameplayEffectHandle(GameplayEffectHandle);
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributeSets.Num(); ++SetIndex)
	{
		const FKaosAbilitySet_AttributeSet& Set = GrantedAttributeSets[SetIndex];

		if (!IsValid(Set.AttributeSet))
		{
			UE_LOG(LogKaosUtilities, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(ASC->GetOwner(), Set.AttributeSet);
		ASC->AddSpawnedAttribute(NewSet);

		OutHandle.AddAttributeSet(NewSet);
	}

	return OutHandle;
}

FKaosAbilitySetHandle UKaosGameplayAbilitySet::GiveAbilitySetToInterface(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface, UObject* OverrideSourceObject) const
{
	UAbilitySystemComponent* AresASC = Cast<UAbilitySystemComponent>(AbilitySystemInterface.GetObject());
	return GiveAbilitySetTo(AresASC, OverrideSourceObject);
}
