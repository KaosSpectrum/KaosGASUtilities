// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/KaosAbilitySystemGlobals.h"

FKaosAttributeSetInitter* UKaosAbilitySystemGlobals::GetKaosAttributeSetInitter() const
{
	return static_cast<FKaosAttributeSetInitter*>(GetAttributeSetInitter());
}


void UKaosAbilitySystemGlobals::InitAttributeSetDefaults(UAbilitySystemComponent* AbilitySystemComponent, const FKaosAttributeInitializationKey& Key, int32 Level, bool bInitialInit)
{
	if (ensure(Key.IsValid()))
	{
		FName GroupName = Key.GetAttributeInitCategory();
		if (!Key.GetAttributeInitSubCategory().IsNone())
		{
			GroupName = FName(*FString::Printf(TEXT("%s.%s"), *Key.GetAttributeInitCategory().ToString(), *Key.GetAttributeInitSubCategory().ToString()));
		}
		GetKaosAttributeSetInitter()->InitAttributeSetDefaults(AbilitySystemComponent, GroupName, Level, bInitialInit);
	}
}

void UKaosAbilitySystemGlobals::ApplyAttributeSetDefaults(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute& InAttribute, const FKaosAttributeInitializationKey& Key, int32 Level)
{
	if (ensure(Key.IsValid()))
	{
		FName GroupName = Key.GetAttributeInitCategory();
		if (!Key.GetAttributeInitSubCategory().IsNone())
		{
			GroupName = FName(*FString::Printf(TEXT("%s.%s"), *Key.GetAttributeInitCategory().ToString(), *Key.GetAttributeInitSubCategory().ToString()));
		}
		GetKaosAttributeSetInitter()->ApplyAttributeDefault(AbilitySystemComponent, InAttribute, GroupName, Level);
	}
}

TArray<float> UKaosAbilitySystemGlobals::GetAttributeSetValues(UClass* AttributeSetClass, FProperty* AttributeProperty, const FKaosAttributeInitializationKey& Key) const
{
	if (ensure(Key.IsValid()))
	{
		FName GroupName = Key.GetAttributeInitCategory();
		if (!Key.GetAttributeInitSubCategory().IsNone())
		{
			GroupName = FName(*FString::Printf(TEXT("%s.%s"), *Key.GetAttributeInitCategory().ToString(), *Key.GetAttributeInitSubCategory().ToString()));
		}
		return GetKaosAttributeSetInitter()->GetAttributeSetValues(AttributeSetClass, AttributeProperty, GroupName);
	}
	return {};
}

void UKaosAbilitySystemGlobals::AllocAttributeSetInitter()
{
	GlobalAttributeSetInitter = MakeShared<FKaosAttributeSetInitter>();
}

TSharedPtr<FKaosAttributeBasics> UKaosAbilitySystemGlobals::AllocKaosAttributeBasics() const
{
	return MakeShared<FKaosAttributeBasics>();
}