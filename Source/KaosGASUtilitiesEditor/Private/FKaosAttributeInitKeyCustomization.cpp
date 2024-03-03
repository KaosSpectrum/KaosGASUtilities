// Copyright (C) 2024, Daniel Moss
// 
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "FKaosAttributeInitKeyCustomization.h"
#include "AbilitySystemGlobals.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "KaosUtilitiesTypes.h"
#include "PropertyCustomizationHelpers.h"
#include "AbilitySystem/KaosAbilitySystemComponent.h"
#include "AbilitySystem/KaosAbilitySystemGlobals.h"
#include "Engine/AssetManager.h"

#define LOCTEXT_NAMESPACE "AttributeBasedFloatDetails"

TSharedRef<IPropertyTypeCustomization> FKaosAttributeInitKeyCustomization::MakeInstance()
{
	return MakeShareable(new FKaosAttributeInitKeyCustomization());
}

void FKaosAttributeInitKeyCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	const UKaosAbilitySystemGlobals& Globals = UKaosAbilitySystemGlobals::Get();
	TArray<UCurveTable*> LoadedCurves;
	for (const auto& Curve : Globals.GetCurveInitTables())
	{
		UCurveTable* AttribTable = Cast<UCurveTable>(Curve.TryLoad());
		LoadedCurves.Add(AttribTable);
	}

	FNameMap.Reset();

	if (LoadedCurves.Num() > 0)
	{
		for (const UCurveTable* CurTable : LoadedCurves)
		{
			if (!IsValid(CurTable))
			{
				continue;
			}

			for (const TPair<FName, FRealCurve*>& CurveRow : CurTable->GetRowMap())
			{
				FString RowName = CurveRow.Key.ToString();
				FString ClassName;
				FString SetName;
				FString AttributeName;
				FString Temp;

				RowName.Split(TEXT("."), &Temp, &AttributeName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				Temp.Split(TEXT("."), &ClassName, &SetName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				FString Primary;
				FString Secondary;
				ClassName.Split(TEXT("."), &Primary, &Secondary, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				TArray<FName>& Rows = FNameMap.FindOrAdd(FName(*Primary));
				Rows.AddUnique(FName(*Secondary));
			}
		}
	}

	PropertyUtilities = StructCustomizationUtils.GetPropertyUtilities();

	AttributeCalculationPrimaryPropertyHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FKaosAttributeInitializationKey, AttributeInitCategory));
	AttributeCalculationSecondaryPropertyHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FKaosAttributeInitializationKey, AttributeInitSubCategory));
	if (AttributeCalculationPrimaryPropertyHandle.IsValid() && AttributeCalculationSecondaryPropertyHandle.IsValid())
	{
		HeaderRow
			.NameContent()
			[
				StructPropertyHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			.MinDesiredWidth(600)
			.MaxDesiredWidth(4096)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					  .HAlign(HAlign_Fill)
					  .Padding(1.f, 0.f, 2.f, 0.f)
					[
						PropertyCustomizationHelpers::MakePropertyComboBox(AttributeCalculationPrimaryPropertyHandle,
						                                                   FOnGetPropertyComboBoxStrings::CreateStatic(&FKaosAttributeInitKeyCustomization::GeneratePrimaryComboboxStrings, true, false, &FNameMap),
						                                                   FOnGetPropertyComboBoxValue::CreateSP(this, &FKaosAttributeInitKeyCustomization::GeneratePrimaryString),
						                                                   FOnPropertyComboBoxValueSelected::CreateSP(this, &FKaosAttributeInitKeyCustomization::OnPrimaryValueSelected))
					]

					+ SVerticalBox::Slot()
					  .HAlign(HAlign_Fill)
					  .Padding(2.f, 0.f, 2.f, 0.f)
					[
						PropertyCustomizationHelpers::MakePropertyComboBox(AttributeCalculationPrimaryPropertyHandle,
						                                                   FOnGetPropertyComboBoxStrings::CreateStatic(&FKaosAttributeInitKeyCustomization::GenerateSecondaryComboboxStrings, true, false, &FNameMap,
						                                                                                               AttributeCalculationPrimaryPropertyHandle),
						                                                   FOnGetPropertyComboBoxValue::CreateSP(this, &FKaosAttributeInitKeyCustomization::GenerateSecondaryString),
						                                                   FOnPropertyComboBoxValueSelected::CreateSP(this, &FKaosAttributeInitKeyCustomization::OnSecondaryValueSelected))
					]
				]
			];
	}
}


void FKaosAttributeInitKeyCustomization::GeneratePrimaryComboboxStrings(TArray<TSharedPtr<FString>>& OutComboBoxStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems, bool bAllowClear, bool bAllowAll,
                                                                    TMap<FName, TArray<FName>>* InItems)
{
	for (auto Iter = InItems->CreateConstIterator(); Iter; ++Iter)
	{
		OutComboBoxStrings.Add(MakeShared<FString>(Iter.Key().ToString()));
	}
}

void FKaosAttributeInitKeyCustomization::GenerateSecondaryComboboxStrings(TArray<TSharedPtr<FString>>& OutComboBoxStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems, bool bAllowClear, bool bAllowAll,
                                                                      TMap<FName, TArray<FName>>* InItems, TSharedPtr<IPropertyHandle> PrimaryKey)
{
	void* TagDataPtr = nullptr;
	PrimaryKey->GetValueData(TagDataPtr);
	const FName* TagPtr = static_cast<FName*>(TagDataPtr);

	if (!TagPtr || TagPtr->IsNone())
	{
		OutComboBoxStrings.Add(MakeShared<FString>("Invalid"));
	}
	else
	{
		const auto Arr = InItems->Find(*TagPtr);
		if (Arr)
		{
			for (auto& Name : *Arr)
			{
				OutComboBoxStrings.Add(MakeShared<FString>(Name.ToString()));
			}
		}
	}
}

FString FKaosAttributeInitKeyCustomization::GeneratePrimaryString()
{
	void* TagDataPtr = nullptr;
	AttributeCalculationPrimaryPropertyHandle->GetValueData(TagDataPtr);
	const FName* TagPtr = static_cast<FName*>(TagDataPtr);

	FString TagString = TagPtr ? TagPtr->ToString() : "Invalid";

	return TagString;
}

FString FKaosAttributeInitKeyCustomization::GenerateSecondaryString()
{
	void* TagDataPtr = nullptr;
	AttributeCalculationSecondaryPropertyHandle->GetValueData(TagDataPtr);
	const FName* TagPtr = static_cast<FName*>(TagDataPtr);

	FString TagString = TagPtr ? TagPtr->ToString() : "Invalid";

	return TagString;
}

void FKaosAttributeInitKeyCustomization::OnPrimaryValueSelected(const FString& String)
{
	AttributeCalculationPrimaryPropertyHandle->SetValue(FName(*String));
	AttributeCalculationSecondaryPropertyHandle->ResetToDefault();
}

void FKaosAttributeInitKeyCustomization::OnSecondaryValueSelected(const FString& String)
{
	AttributeCalculationSecondaryPropertyHandle->SetValue(FName(*String));
}

void FKaosAttributeInitKeyCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}

#undef LOCTEXT_NAMESPACE
