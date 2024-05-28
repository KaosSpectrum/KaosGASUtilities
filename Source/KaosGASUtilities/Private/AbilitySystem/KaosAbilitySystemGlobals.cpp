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

void UKaosAbilitySystemGlobals::ReloadAttributeDefaults()
{
	Super::ReloadAttributeDefaults();
}

TSharedPtr<FKaosAttributeBasics> UKaosAbilitySystemGlobals::AllocKaosAttributeBasics() const
{
	return MakeShared<FKaosAttributeBasics>();
}