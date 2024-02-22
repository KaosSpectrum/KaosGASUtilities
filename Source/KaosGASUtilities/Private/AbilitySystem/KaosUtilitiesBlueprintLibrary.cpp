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

#include "AbilitySystem/KaosUtilitiesBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "KaosUtilitiesLogging.h"
#include "GameplayEffect.h"
#include "Logging/StructuredLog.h"

bool UKaosUtilitiesBlueprintLibrary::CanActivateAbilityWithMatchingTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags)
{
	if (AbilitySystemComponent)
	{
		//Get a copy of the ability specs.
		TArray<FGameplayAbilitySpec> Specs = AbilitySystemComponent->GetActivatableAbilities();

		//Get the Actor info as we need it.
		const FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();

		//Loop through all specs and find if we can activate any ability
		for (const FGameplayAbilitySpec& Spec : Specs)
		{
			//If ability cdo is null, we skip.
			if (Spec.Ability == nullptr)
			{
				continue;
			}

			//If tags match and we can activate, return the call to CanActivateAbility.
			if (Spec.Ability->AbilityTags.HasAll(GameplayAbilityTags))
			{
				return Spec.Ability->CanActivateAbility(Spec.Handle, ActorInfo);
			}
		}
	}
	return false;
}

bool UKaosUtilitiesBlueprintLibrary::HasActiveAbilityWithMatchingTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags)
{
	if (AbilitySystemComponent)
	{
		//Get a copy of the ability specs.
		TArray<FGameplayAbilitySpec> Specs = AbilitySystemComponent->GetActivatableAbilities();

		//Get the Actor info as we need it.
		const FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();

		//Loop through all specs and find if we can activate any ability
		for (const FGameplayAbilitySpec& Spec : Specs)
		{
			//If ability cdo is null, we skip.
			if (Spec.Ability == nullptr)
			{
				continue;
			}

			//If tags match then we have the ability.
			if (Spec.Ability->AbilityTags.HasAll(GameplayAbilityTags) && Spec.IsActive())
			{
				return Spec.Ability->CanActivateAbility(Spec.Handle, ActorInfo);
			}
		}
	}
	return false;
}

void UKaosUtilitiesBlueprintLibrary::CancelAbilityWithAllTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags)
{
	if (AbilitySystemComponent)
	{
		//Get a copy of the ability specs.
		TArray<FGameplayAbilitySpec> Specs = AbilitySystemComponent->GetActivatableAbilities();

		//Loop through all specs and find if we can activate any ability
		for (const FGameplayAbilitySpec& Spec : Specs)
		{
			//If ability cdo is null, we skip.
			if (Spec.Ability == nullptr)
			{
				continue;
			}

			//If tags match, cancel the ability
			if (Spec.Ability->AbilityTags.HasAll(GameplayAbilityTags) && Spec.IsActive())
			{
				AbilitySystemComponent->CancelAbilityHandle(Spec.Handle);
			}
		}
	}
}

bool UKaosUtilitiesBlueprintLibrary::HasAbilityWithAllTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags)
{
	if (AbilitySystemComponent)
	{
		//Get a copy of the ability specs.
		TArray<FGameplayAbilitySpec> Specs = AbilitySystemComponent->GetActivatableAbilities();

		//Loop through all specs and find if we can activate any ability
		for (const FGameplayAbilitySpec& Spec : Specs)
		{
			//If ability cdo is null, we skip.
			if (Spec.Ability == nullptr)
			{
				continue;
			}

			//If tags match then we have the ability
			if (Spec.Ability->AbilityTags.HasAll(GameplayAbilityTags))
			{
				return true;
			}
		}
	}
	return false;
}

bool UKaosUtilitiesBlueprintLibrary::IsAbilityOnCooldownWithAllTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags, float& TimeRemaining, float& CooldownDuration)
{
	if (AbilitySystemComponent)
	{
		//Get a copy of the ability specs.
		TArray<FGameplayAbilitySpec> Specs = AbilitySystemComponent->GetActivatableAbilities();

		//Loop through all specs and find if we can activate any ability
		for (const FGameplayAbilitySpec& Spec : Specs)
		{
			//If ability cdo is null, we skip.
			if (Spec.Ability == nullptr)
			{
				continue;
			}

			//If tags match, check if the cooldown tags are applied to the ASC.
			if (Spec.Ability->AbilityTags.HasAll(GameplayAbilityTags))
			{
				const FGameplayTagContainer* CooldownTags = Spec.Ability->GetCooldownTags();
				if (CooldownTags && CooldownTags->Num() > 0 && AbilitySystemComponent->HasAnyMatchingGameplayTags(*CooldownTags))
				{
					FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(*CooldownTags);
					TArray<TPair<float, float>> DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
					if (DurationAndTimeRemaining.Num() > 0)
					{
						// Iterate over all the effects applying the cooldown (if there are, somehow, multiple) and find the longest
						int32 BestIdx = 0;
						float LongestTime = DurationAndTimeRemaining[0].Key;
						for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
						{
							if (DurationAndTimeRemaining[Idx].Key > LongestTime)
							{
								LongestTime = DurationAndTimeRemaining[Idx].Key;
								BestIdx = Idx;
							}
						}

						TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
						CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

						return true;
					}
				}
			}
		}
	}
	return false;
}

bool UKaosUtilitiesBlueprintLibrary::CanActivateAbilityByClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (AbilitySystemComponent)
	{
		//Get a copy of the ability specs.
		TArray<FGameplayAbilitySpec> Specs = AbilitySystemComponent->GetActivatableAbilities();

		for (const FGameplayAbilitySpec& AbilitySpec : Specs)
		{
			const FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
			const UGameplayAbility* Ability = AbilitySpec.GetPrimaryInstance() ? AbilitySpec.GetPrimaryInstance() : AbilitySpec.Ability.Get();
			if (Ability->GetClass() == AbilityClass)
			{
				return Ability->CanActivateAbility(AbilitySpec.Handle, ActorInfo, nullptr, nullptr, nullptr);
			}
		}
	}

	return false;
}

FKaosAbilitySetHandle UKaosUtilitiesBlueprintLibrary::GiveAbilitySetToActor(AActor* Actor, UKaosGameplayAbilitySet* Set, UObject* OptionalOverrideSourceObject)
{
	if (!IsValid(Actor))
	{
		UE_LOGFMT(LogKaosUtilities, Warning, "Tried to give ability set to actor which is null");
		return {};
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
	if (!IsValid(ASC))
	{
		UE_LOGFMT(LogKaosUtilities, Warning, "Tried to give ability set to actor {Actor} which does not have an Ability System Component", Actor->GetName());
		return {};
	}

	if (!IsValid(Set))
	{
		UE_LOGFMT(LogKaosUtilities, Warning, "Tried to give ability set to actor {Actor} but the Ability Set is null", Actor->GetName());
		return {};
	}

	return Set->GiveAbilitySetTo(ASC, OptionalOverrideSourceObject);
}

FKaosAbilitySetHandle UKaosUtilitiesBlueprintLibrary::GiveAbilitySetToASC(UAbilitySystemComponent* AbilitySystemComponent, UKaosGameplayAbilitySet* Set, UObject* OptionalOverrideSourceObject)
{
	if (!IsValid(Set))
	{
		UE_LOGFMT(LogKaosUtilities, Warning, "Tried to give ability set to AbilitySystemComponent {ASC} but the Ability Set is null", *GetNameSafe(AbilitySystemComponent));
		return {};
	}

	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOGFMT(LogKaosUtilities, Warning, "Tried to give ability set {Set} with a null AbilitySystemComponent", Set->GetName());
		return {};
	}

	return Set->GiveAbilitySetTo(AbilitySystemComponent, OptionalOverrideSourceObject);
}

FKaosAbilitySetHandle UKaosUtilitiesBlueprintLibrary::GiveAbilitySetToInterface(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface, UKaosGameplayAbilitySet* Set, UObject* OptionalOverrideSourceObject)
{
	if (!IsValid(Set))
	{
		UE_LOGFMT(LogKaosUtilities, Warning, "Tried to give ability set to AbilitySystemInterface {ASC} but the Ability Set is null", *GetNameSafe(AbilitySystemInterface.GetObject()));
		return {};
	}

	if (!IsValid(AbilitySystemInterface.GetObject()))
	{
		UE_LOGFMT(LogKaosUtilities, Warning, "Tried to give ability set {Set} with a invalid interface", Set->GetName());
		return {};
	}

	return Set->GiveAbilitySetToInterface(AbilitySystemInterface, OptionalOverrideSourceObject);
}

void UKaosUtilitiesBlueprintLibrary::TakeAbilitySet(FKaosAbilitySetHandle& AbilitySetHandle)
{
	if (!AbilitySetHandle.IsValid())
	{
		UE_LOGFMT(LogKaosUtilities, Warning, "Tried to remove ability set with an inavalid Ability Set Handle.");
		return;
	}
	AbilitySetHandle.RemoveSet();
}

int32 UKaosUtilitiesBlueprintLibrary::GetAbilityLevel(const FGameplayEffectContextHandle& EffectContextHandle)
{
	return EffectContextHandle.GetAbilityLevel();
}

void UKaosUtilitiesBlueprintLibrary::GetAssetTags(const FGameplayEffectSpecHandle& SpecHandle, FGameplayTagContainer& OutAssetTags)
{
	const FGameplayEffectSpec* Spec = SpecHandle.IsValid() ? SpecHandle.Data.Get() : nullptr;
	if (Spec)
	{
		Spec->GetAllAssetTags(OutAssetTags);
	}
}

void UKaosUtilitiesBlueprintLibrary::SetLooseGameplayTagCount(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag GameplayTag, int32 NewCount)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(GameplayTag, NewCount);
	}
}

FGameplayAbilitySpec* UKaosUtilitiesBlueprintLibrary::FindAbilitySpecByClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> AbilityClass, UObject* OptionalSourceObject)
{
	if (AbilitySystemComponent)
	{
		//Get a copy of the ability specs.
		TArray<FGameplayAbilitySpec> Specs = AbilitySystemComponent->GetActivatableAbilities();

		for (FGameplayAbilitySpec& Spec : Specs)
		{
			const bool bMatchesSourceObject = OptionalSourceObject != nullptr ? OptionalSourceObject == Spec.SourceObject.Get() : true;
			if (Spec.Ability->GetClass() == AbilityClass && bMatchesSourceObject)
			{
				return &Spec;
			}
		}
	}
	return nullptr;
}

FGameplayAbilitySpec* UKaosUtilitiesBlueprintLibrary::FindAbilitySpecWithAllAbilityTags(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer AbilityTags, UObject* OptionalSourceObject)
{
	if (AbilitySystemComponent)
	{
		//Get a copy of the ability specs.
		TArray<FGameplayAbilitySpec> Specs = AbilitySystemComponent->GetActivatableAbilities();

		for (FGameplayAbilitySpec& Spec : Specs)
		{
			const bool bMatchesSourceObject = OptionalSourceObject != nullptr ? OptionalSourceObject == Spec.SourceObject.Get() : true;
			if (Spec.Ability->AbilityTags.HasAll(AbilityTags) && bMatchesSourceObject)
			{
				return &Spec;
			}
		}
	}
	return nullptr;
}

bool UKaosUtilitiesBlueprintLibrary::HasAttributeSet(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UAttributeSet> AttributeClass)
{
	if (AbilitySystemComponent)
	{
		for (const UAttributeSet* Set : AbilitySystemComponent->GetSpawnedAttributes())
		{
			if (Set && Set->IsA(AttributeClass))
			{
				return true;
			}
		}
	}
	return false;
}

bool UKaosUtilitiesBlueprintLibrary::IsAbilityTagBlocked(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag AbilityTag)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->AreAbilityTagsBlocked(FGameplayTagContainer(AbilityTag));
	}
	return false;
}

bool UKaosUtilitiesBlueprintLibrary::IsAbilityActive(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAbilitySpecHandle& InHandle)
{
	if (AbilitySystemComponent)
	{
		//Get a copy of the ability specs.
		TArray<FGameplayAbilitySpec> Specs = AbilitySystemComponent->GetActivatableAbilities();

		for (const FGameplayAbilitySpec& Spec : Specs)
		{
			if (Spec.Handle == InHandle)
			{
				return Spec.IsActive();
			}
		}
	}
	return false;
}

bool UKaosUtilitiesBlueprintLibrary::IsAbilityActiveByClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> AbilityClass, UObject* OptionalSourceObject)
{
	if (AbilitySystemComponent)
	{
		if (const FGameplayAbilitySpec* Spec = FindAbilitySpecByClass(AbilitySystemComponent, AbilityClass, OptionalSourceObject))
		{
			return Spec->IsActive();
		}
	}
	return false;
}

bool UKaosUtilitiesBlueprintLibrary::CanApplyAttributeModifiers(UAbilitySystemComponent* AbilitySystemComponent, FGameplayEffectSpec EffectSpec)
{
	if (AbilitySystemComponent)
	{
		EffectSpec.CalculateModifierMagnitudes();

		for (int32 ModIdx = 0; ModIdx < EffectSpec.Modifiers.Num(); ++ModIdx)
		{
			const FGameplayModifierInfo& ModDef = EffectSpec.Def->Modifiers[ModIdx];
			const FModifierSpec& ModSpec = EffectSpec.Modifiers[ModIdx];

			// It only makes sense to check additive operators
			if (ModDef.ModifierOp == EGameplayModOp::Additive)
			{
				if (!ModDef.Attribute.IsValid())
				{
					continue;
				}
				const UAttributeSet* Set = AbilitySystemComponent->GetAttributeSet(ModDef.Attribute.GetAttributeSetClass());
				const float CurrentValue = ModDef.Attribute.GetNumericValueChecked(Set);
				const float CostValue = ModSpec.GetEvaluatedMagnitude();

				if (CurrentValue + CostValue < 0.f)
				{
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

void UKaosUtilitiesBlueprintLibrary::BlockAbilitiesWithTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->BlockAbilitiesWithTags(GameplayAbilityTags);
	}
}

void UKaosUtilitiesBlueprintLibrary::UnblockAbilitiesWithTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& GameplayAbilityTags)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->UnBlockAbilitiesWithTags(GameplayAbilityTags);
	}
}
