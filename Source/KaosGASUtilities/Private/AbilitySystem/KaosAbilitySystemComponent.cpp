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

#include "AbilitySystem/KaosAbilitySystemComponent.h"

#include "AbilitySystem/KaosAbilityTagRelationships.h"
#include "AbilitySystem/KaosGameplayAbility.h"

void UKaosAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags,
                                                                 const FGameplayTagContainer& CancelTags)
{
	FGameplayTagContainer AbilityBlockTags = BlockTags;
	FGameplayTagContainer AbilityCancelTags = CancelTags;

	const UKaosAbilityTagRelationships* TagRelationship = GetAbilityTagRelationships();
	if (TagRelationship)
	{
		TagRelationship->GetAbilityTagsToBlockAndCancel(AbilityTags, &AbilityBlockTags, &AbilityCancelTags);
	}

	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, AbilityBlockTags, bExecuteCancelTags, AbilityCancelTags);
}

void UKaosAbilitySystemComponent::K2_UnBlockAbilitiesWithTags(FGameplayTagContainer& Tags)
{
	ApplyAbilityBlockAndCancelTags(FGameplayTagContainer(), nullptr, false, Tags, false, FGameplayTagContainer());
}

void UKaosAbilitySystemComponent::K2_BlockAbilitiesWithTags(FGameplayTagContainer& Tags)
{
	ApplyAbilityBlockAndCancelTags(FGameplayTagContainer(), nullptr, true, Tags, false, FGameplayTagContainer());
}

void UKaosAbilitySystemComponent::GetRelationshipActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const
{
	const UKaosAbilityTagRelationships* TagRelationship = GetAbilityTagRelationships();
	if (TagRelationship)
	{
		TagRelationship->GetRequiredAndBlockedActivationTags(AbilityTags, &OutActivationRequired, &OutActivationBlocked);
	}
}

void UKaosAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	if (const APawn* Avatar = Cast<APawn>(GetAvatarActor()))
	{
		if (!Avatar->IsLocallyControlled() && Ability->IsSupportedForNetworking())
		{
			ClientNotifyAbilityFailed(Ability, FailureReason);
			return;
		}
	}

	HandleAbilityFailed(Ability, FailureReason);
}

void UKaosAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle);
	ENetRole OwnerRole = GetOwnerRole();
	UKaosGameplayAbility* KaosGA = Cast<UKaosGameplayAbility>(Ability);
	if (OwnerRole == ROLE_Authority && !Spec->RemoveAfterActivation)
	{
		if (KaosGA && KaosGA->IsInstantiated() && KaosGA->ShouldRemoveAfterActivation() && !Spec->IsActive())
		{
			ClearAbility(Handle);
		}
	}
}

FActiveGameplayEffect* UKaosAbilitySystemComponent::GetActiveGameplayEffect_Mutable(FActiveGameplayEffectHandle Handle)
{
	return ActiveGameplayEffects.GetActiveGameplayEffect(Handle);
}

TArray<FActiveGameplayEffectHandle> UKaosAbilitySystemComponent::GetAllActiveEffectHandles() const
{
	return ActiveGameplayEffects.GetAllActiveEffectHandles();
}

void UKaosAbilitySystemComponent::GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle) const
{
	const TSharedPtr<FAbilityReplicatedDataCache> ReplicatedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, ActivationInfo.GetActivationPredictionKey()));
	if (ReplicatedData.IsValid())
	{
		OutTargetDataHandle = ReplicatedData->TargetData;
	}
}

const UKaosAbilityTagRelationships* UKaosAbilitySystemComponent::GetAbilityTagRelationships() const
{
	return AbilityTagRelationship;
}

void UKaosAbilitySystemComponent::ClientNotifyAbilityFailed_Implementation(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	HandleAbilityFailed(Ability, FailureReason);
}

void UKaosAbilitySystemComponent::HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	if (const UKaosGameplayAbility* KaosAbility = Cast<const UKaosGameplayAbility>(Ability))
	{
		KaosAbility->OnAbilityFailedToActivate(FailureReason);
		if (KaosAbility->IsPassiveAbility())
		{
			KaosAbility->NotifyFailedToActivatePassiveAbility();
		}
	}
}
