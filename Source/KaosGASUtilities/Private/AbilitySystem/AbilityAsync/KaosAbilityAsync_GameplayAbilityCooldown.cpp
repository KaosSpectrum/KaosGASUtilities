// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "AbilitySystem/AbilityAsync/KaosAbilityAsync_GameplayAbilityCooldown.h"

#include "AbilitySystemComponent.h"

UKaosAbilityAsync_GameplayAbilityCooldown* UKaosAbilityAsync_GameplayAbilityCooldown::WaitAbilityCooldown(AActor* TargetActor, FGameplayTag WithAbilityTag, FGameplayTag WithoutAbilityTag, bool OnlyTriggerOnce)
{
	UKaosAbilityAsync_GameplayAbilityCooldown* MyObj = NewObject<UKaosAbilityAsync_GameplayAbilityCooldown>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->CachedWithAbilityTag = WithAbilityTag;
	MyObj->CachedCooldownTag = FGameplayTag::EmptyTag;
	MyObj->CachedWithoutAbilityTag = WithoutAbilityTag;
	MyObj->CachedAbilityClass = nullptr;
	MyObj->bCachedOnlyTriggerOnce = OnlyTriggerOnce;
	return MyObj;
}

UKaosAbilityAsync_GameplayAbilityCooldown* UKaosAbilityAsync_GameplayAbilityCooldown::WaitAbilityCooldownClass(AActor* TargetActor, TSubclassOf<UGameplayAbility> AbilityClass, bool OnlyTriggerOnce)
{
	UKaosAbilityAsync_GameplayAbilityCooldown* MyObj = NewObject<UKaosAbilityAsync_GameplayAbilityCooldown>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->CachedAbilityClass = AbilityClass;
	MyObj->CachedCooldownTag = FGameplayTag::EmptyTag;
	MyObj->bCachedOnlyTriggerOnce = OnlyTriggerOnce;
	return MyObj;
}

UKaosAbilityAsync_GameplayAbilityCooldown* UKaosAbilityAsync_GameplayAbilityCooldown::WaitAbilityCooldownSpecificTag(AActor* TargetActor, FGameplayTag CooldownTag, bool OnlyTriggerOnce)
{
	UKaosAbilityAsync_GameplayAbilityCooldown* MyObj = NewObject<UKaosAbilityAsync_GameplayAbilityCooldown>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->CachedCooldownTag = CooldownTag;
	MyObj->CachedAbilityClass = nullptr;
	MyObj->bCachedOnlyTriggerOnce = OnlyTriggerOnce;
	return MyObj;
}

void UKaosAbilityAsync_GameplayAbilityCooldown::HandleTagChanged(FGameplayTag GameplayTag, int32 Count)
{
	//Cooldown applied
	if (Count > 0)
	{
		CooldownApplied.Broadcast(GameplayTag);
	}
	else
	{
		CooldownRemoved.Broadcast(GameplayTag);
	}
	
	if (bCachedOnlyTriggerOnce)
	{
		EndAction();
	}
}

void UKaosAbilityAsync_GameplayAbilityCooldown::Activate()
{
	Super::Activate();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		CooldownTagsToListenFor.Reset();

		//Use supplied ability class.
		if (CachedAbilityClass != nullptr)
		{
			UGameplayAbility* GA_CDO = CachedAbilityClass->GetDefaultObject<UGameplayAbility>();
			if (GA_CDO)
			{
				const FGameplayTagContainer* CDTags = GA_CDO->GetCooldownTags();
				if (CDTags)
				{
					CooldownTagsToListenFor.AppendTags(*CDTags);
				}
			}
		}
		else if (CachedCooldownTag.IsValid())
		{
			CooldownTagsToListenFor.AddTagFast(CachedCooldownTag);
		}
		else //Search all activatable abilities.
		{
			//Make a copy, cause of ability scope locking issues.
			TArray<FGameplayAbilitySpec> Specs = ASC->GetActivatableAbilities();
			for (const FGameplayAbilitySpec& Spec : Specs)
			{
				if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(CachedWithAbilityTag) && !Spec.Ability->AbilityTags.HasTag(CachedWithoutAbilityTag))
				{
					const FGameplayTagContainer* CDTags = Spec.Ability->GetCooldownTags();
					if (CDTags)
					{
						CooldownTagsToListenFor.AppendTags(*CDTags);
					}
				}
			}
		}

		if (CooldownTagsToListenFor.Num() > 0)
		{
			for (const FGameplayTag& Tag : CooldownTagsToListenFor)
			{
				BoundASCHandles.Add(Tag, ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UKaosAbilityAsync_GameplayAbilityCooldown::HandleTagChanged));
			}
		}
		else
		{
			EndAction();
		}
	}
	else
	{
		EndAction();
	}
}

void UKaosAbilityAsync_GameplayAbilityCooldown::EndAction()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (auto& Elem : BoundASCHandles)
		{
			ASC->RegisterGameplayTagEvent(Elem.Key, EGameplayTagEventType::NewOrRemoved).Remove(Elem.Value);
		}
	}
	Super::EndAction();
}
