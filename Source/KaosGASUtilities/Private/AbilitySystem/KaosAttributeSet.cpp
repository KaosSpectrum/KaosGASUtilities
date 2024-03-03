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

#include "AbilitySystem/KaosAttributeSet.h"

#include "AbilitySystemLog.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/KaosAbilitySystemComponent.h"
#include "AbilitySystem/KaosAbilitySystemGlobals.h"


TSubclassOf<UAttributeSet> CommonFindBestAttributeClass(TArray<TSubclassOf<UAttributeSet>>& ClassList, FString PartialName)
{
	for (const auto Class : ClassList)
	{
		if (Class->GetName().Contains(PartialName))
		{
			return Class;
		}
	}

	return nullptr;
}

/**
 *	Transforms CurveTable data into format more efficient to read at runtime.
 *	UCurveTable requires string parsing to map to GroupName/AttributeSet/Attribute
 *	Each curve in the table represents a *single attribute's values for all levels*.
 *	At runtime, we want *all attribute values at given level*.
 *
 *	This code assumes that your curve data starts with a key of 1 and increases by 1 with each key.
 */
void FKaosAttributeSetInitter::PreloadAttributeSetData(const TArray<UCurveTable*>& CurveData)
{
	if (!ensure(CurveData.Num() > 0))
	{
		return;
	}

	/**
	 *	Get list of AttributeSet classes loaded
	 */

	TArray<TSubclassOf<UAttributeSet>> ClassList;
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		UClass* TestClass = *ClassIt;
		if (TestClass->IsChildOf(UAttributeSet::StaticClass()))
		{
			ClassList.Add(TestClass);
		}
	}

	/**
	 *	Loop through CurveData table and build sets of Defaults that keyed off of Name + Level
	 */
	for (const UCurveTable* CurTable : CurveData)
	{
		for (const TPair<FName, FRealCurve*>& CurveRow : CurTable->GetRowMap())
		{
			FString RowName = CurveRow.Key.ToString();
			FString ClassName;
			FString SetName;
			FString AttributeName;
			FString Temp;

			RowName.Split(TEXT("."), &Temp, &AttributeName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			Temp.Split(TEXT("."), &ClassName, &SetName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

			if (!ensure(!ClassName.IsEmpty() && !SetName.IsEmpty() && !AttributeName.IsEmpty()))
			{
				ABILITY_LOG(Verbose, TEXT("FAttributeSetInitterDiscreteLevels::PreloadAttributeSetData Unable to parse row %s in %s"), *RowName, *CurTable->GetName());
				continue;
			}

			// Find the AttributeSet

			TSubclassOf<UAttributeSet> Set = CommonFindBestAttributeClass(ClassList, SetName);
			if (!Set)
			{
				// This is ok, we may have rows in here that don't correspond directly to attributes
				ABILITY_LOG(Verbose, TEXT("FAttributeSetInitterDiscreteLevels::PreloadAttributeSetData Unable to match AttributeSet from %s (row: %s)"), *SetName, *RowName);
				continue;
			}

			// Find the FProperty
			FProperty* Property = FindFProperty<FProperty>(*Set, *AttributeName);
			if (!IsSupportedProperty(Property))
			{
				ABILITY_LOG(Verbose, TEXT("FAttributeSetInitterDiscreteLevels::PreloadAttributeSetData Unable to match Attribute from %s (row: %s)"), *AttributeName, *RowName);
				continue;
			}

			const FRealCurve* Curve = CurveRow.Value;
			FName ClassFName = FName(*ClassName);
			FKaosAttributeSetDefaultsCollection& DefaultCollection = Defaults.FindOrAdd(ClassFName);

			// Check our curve to make sure the keys match the expected format
			int32 ExpectedLevel = 1;
			bool bShouldSkip = false;
			for (auto KeyIter = Curve->GetKeyHandleIterator(); KeyIter; ++KeyIter)
			{
				const FKeyHandle& KeyHandle = *KeyIter;
				if (KeyHandle == FKeyHandle::Invalid())
				{
					ABILITY_LOG(Verbose, TEXT("FAttributeSetInitterDiscreteLevels::PreloadAttributeSetData Data contains an invalid key handle (row: %s)"), *RowName);
					bShouldSkip = true;
					break;
				}

				const int32 Level = Curve->GetKeyTimeValuePair(KeyHandle).Key;
				if (ExpectedLevel != Level)
				{
					ABILITY_LOG(Verbose, TEXT("FAttributeSetInitterDiscreteLevels::PreloadAttributeSetData Keys are expected to start at 1 and increase by 1 for every key (row: %s)"), *RowName);
					bShouldSkip = true;
					break;
				}

				++ExpectedLevel;
			}

			if (bShouldSkip)
			{
				continue;
			}

			const int32 LastLevel = Curve->GetKeyTime(Curve->GetLastKeyHandle());
			DefaultCollection.LevelData.SetNum(FMath::Max(LastLevel, DefaultCollection.LevelData.Num()));

			//At this point we know the Name of this "class"/"group", the AttributeSet, and the Property Name. Now loop through the values on the curve to get the attribute default value at each level.
			for (auto KeyIter = Curve->GetKeyHandleIterator(); KeyIter; ++KeyIter)
			{
				const FKeyHandle& KeyHandle = *KeyIter;

				const TPair<float, float> LevelValuePair = Curve->GetKeyTimeValuePair(KeyHandle);
				int32 Level = LevelValuePair.Key;
				const float Value = LevelValuePair.Value;

				FKaosAttributeSetDefaults& SetDefaults = DefaultCollection.LevelData[Level - 1];

				FKaosAttributeDefaultValueList* DefaultDataList = SetDefaults.DataMap.Find(Set);
				if (DefaultDataList == nullptr)
				{
					ABILITY_LOG(Verbose, TEXT("Initializing new default set for %s[%d]. PropertySize: %d.. DefaultSize: %d"), *Set->GetName(), Level, Set->GetPropertiesSize(), UAttributeSet::StaticClass()->GetPropertiesSize());

					DefaultDataList = &SetDefaults.DataMap.Add(Set);
				}

				// Import curve value into default data

				check(DefaultDataList);
				DefaultDataList->AddPair(Property, Value);
			}
		}
	}
}

void FKaosAttributeSetInitter::InitAttributeSetDefaults(UAbilitySystemComponent* AbilitySystemComponent, FName GroupName, int32 Level, bool bInitialInit) const
{
	check(AbilitySystemComponent != nullptr);

	const FKaosAttributeSetDefaultsCollection* Collection = Defaults.Find(GroupName);
	if (!Collection)
	{
		ABILITY_LOG(Warning, TEXT("Unable to find DefaultAttributeSet Group %s. Falling back to Defaults"), *GroupName.ToString());
		Collection = Defaults.Find(FName(TEXT("Default")));
		if (!Collection)
		{
			ABILITY_LOG(Error, TEXT("FAttributeSetInitterDiscreteLevels::InitAttributeSetDefaults Default DefaultAttributeSet not found! Skipping Initialization"));
			return;
		}
	}

	if (!Collection->LevelData.IsValidIndex(Level - 1))
	{
		// We could eventually extrapolate values outside of the max defined levels
		ABILITY_LOG(Warning, TEXT("Attribute defaults for Level %d are not defined! Skipping"), Level);
		return;
	}

	const FKaosAttributeSetDefaults& SetDefaults = Collection->LevelData[Level - 1];
	for (const UAttributeSet* Set : AbilitySystemComponent->GetSpawnedAttributes())
	{
		if (!Set)
		{
			continue;
		}
		// Iterate to find the parent classes, as this could be a derived set
		TSubclassOf<UAttributeSet> ParentClass;
		for (ParentClass = Set->GetClass(); ParentClass ? ParentClass->GetSuperClass() : nullptr; ParentClass = ParentClass->GetSuperClass())
		{
			if (SetDefaults.DataMap.Contains(ParentClass))
			{
				break;
			}
		}
		const FKaosAttributeDefaultValueList* DefaultDataList = SetDefaults.DataMap.Find(ParentClass);
		if (DefaultDataList)
		{
			ABILITY_LOG(Log, TEXT("Initializing Set %s"), *Set->GetName());

			for (auto& DataPair : DefaultDataList->List)
			{
				check(DataPair.Property);

				if (Set->ShouldInitProperty(bInitialInit, DataPair.Property))
				{
					FGameplayAttribute AttributeToModify(DataPair.Property);
					AbilitySystemComponent->SetNumericAttributeBase(AttributeToModify, DataPair.Value);
				}
			}
		}
	}

	AbilitySystemComponent->ForceReplication();
}

void FKaosAttributeSetInitter::ApplyAttributeDefault(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute& InAttribute, FName GroupName, int32 Level) const
{
	const FKaosAttributeSetDefaultsCollection* Collection = Defaults.Find(GroupName);
	if (!Collection)
	{
		ABILITY_LOG(Warning, TEXT("Unable to find DefaultAttributeSet Group %s. Falling back to Defaults"), *GroupName.ToString());
		Collection = Defaults.Find(FName(TEXT("Default")));
		if (!Collection)
		{
			ABILITY_LOG(Error, TEXT("FAttributeSetInitterDiscreteLevels::InitAttributeSetDefaults Default DefaultAttributeSet not found! Skipping Initialization"));
			return;
		}
	}

	if (!Collection->LevelData.IsValidIndex(Level - 1))
	{
		// We could eventually extrapolate values outside of the max defined levels
		ABILITY_LOG(Warning, TEXT("Attribute defaults for Level %d are not defined! Skipping"), Level);
		return;
	}

	const FKaosAttributeSetDefaults& SetDefaults = Collection->LevelData[Level - 1];
	for (const UAttributeSet* Set : AbilitySystemComponent->GetSpawnedAttributes())
	{
		if (!Set)
		{
			continue;
		}

		// Iterate to find the parent classes, as this could be a derived set
		TSubclassOf<UAttributeSet> ParentClass;
		for (ParentClass = Set->GetClass(); ParentClass ? ParentClass->GetSuperClass() : nullptr; ParentClass = ParentClass->GetSuperClass())
		{
			if (SetDefaults.DataMap.Contains(ParentClass))
			{
				break;
			}
		}

		const FKaosAttributeDefaultValueList* DefaultDataList = SetDefaults.DataMap.Find(ParentClass);
		if (DefaultDataList)
		{
			ABILITY_LOG(Log, TEXT("Initializing Set %s"), *Set->GetName());

			for (auto& DataPair : DefaultDataList->List)
			{
				check(DataPair.Property);

				if (DataPair.Property == InAttribute.GetUProperty())
				{
					FGameplayAttribute AttributeToModify(DataPair.Property);
					AbilitySystemComponent->SetNumericAttributeBase(AttributeToModify, DataPair.Value);
				}
			}
		}
	}

	AbilitySystemComponent->ForceReplication();
}

TArray<float> FKaosAttributeSetInitter::GetAttributeSetValues(UClass* AttributeSetClass, FProperty* AttributeProperty, FName GroupName) const
{
	TArray<float> AttributeSetValues;
	const FKaosAttributeSetDefaultsCollection* Collection = Defaults.Find(GroupName);
	if (!Collection)
	{
		ABILITY_LOG(Error, TEXT("FAttributeSetInitterDiscreteLevels::InitAttributeSetDefaults Default DefaultAttributeSet not found! Skipping Initialization"));
		return TArray<float>();
	}

	for (const FKaosAttributeSetDefaults& SetDefaults : Collection->LevelData)
	{
		const FKaosAttributeDefaultValueList* DefaultDataList = SetDefaults.DataMap.Find(AttributeSetClass);
		if (DefaultDataList)
		{
			for (auto& DataPair : DefaultDataList->List)
			{
				check(DataPair.Property);
				if (DataPair.Property == AttributeProperty)
				{
					AttributeSetValues.Add(DataPair.Value);
				}
			}
		}
	}
	return AttributeSetValues;
}


bool FKaosAttributeSetInitter::IsSupportedProperty(FProperty* Property) const
{
	return (Property && (CastField<FNumericProperty>(Property) || FGameplayAttribute::IsGameplayAttributeDataProperty(Property)));
}

UWorld* UKaosAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UKaosAbilitySystemComponent* UKaosAttributeSet::GetKaosAbilitySystemComponent() const
{
	return Cast<UKaosAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

TSharedRef<FKaosAttributeBasics> UKaosAttributeSet::GetBasicsFromModData(const FGameplayEffectModCallbackData& Data) const
{
	const TSharedPtr<FKaosAttributeBasics> OutBasics = UKaosAbilitySystemGlobals::Get().AllocKaosAttributeBasics();
	OutBasics->Context = Data.EffectSpec.GetContext();
	OutBasics->SourceASC = Cast<UKaosAbilitySystemComponent>(OutBasics->Context.GetOriginalInstigatorAbilitySystemComponent());
	OutBasics->SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	OutBasics->TargetTags = *Data.EffectSpec.CapturedTargetTags.GetAggregatedTags();
	Data.EffectSpec.GetAllAssetTags(OutBasics->SpecAssetTags);

	OutBasics->TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.IsValid() ? Data.Target.AbilityActorInfo->AvatarActor.Get() : nullptr;

	OutBasics->SourceAvatarActor = OutBasics->SourceASC->AbilityActorInfo->AvatarActor.IsValid() ? OutBasics->SourceASC->AbilityActorInfo->AvatarActor.Get() : nullptr;
	
	OutBasics->SourceObject = Data.EffectSpec.GetEffectContext().GetSourceObject();
	
	return OutBasics.ToSharedRef();
}
