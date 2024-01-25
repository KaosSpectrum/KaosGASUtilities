// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "KaosUtilitiesTypes.h"

#include "AbilitySystemComponent.h"
#include "KaosUtilitiesLogging.h"
#include "Logging/StructuredLog.h"


void FKaosAbilitySetHandle::RemoveSet()
{
	if (!AbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		UE_LOGFMT(LogKaosUtilities, Warning, "Tried to remove set when not authoritive");
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystemComponent->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : AttributeSets)
	{
		AbilitySystemComponent->RemoveSpawnedAttribute(Set);
	}

	UE_LOGFMT(LogKaosUtilities, Log, "Removed ability set with handle {Handle}", HandleId);
	Reset();
}

void FKaosAbilitySetHandle::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FKaosAbilitySetHandle::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FKaosAbilitySetHandle::AddAttributeSet(UAttributeSet* Set)
{
	AttributeSets.Add(Set);
}
