// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "KaosAttributeSet.h"
#include "KaosUtilitiesTypes.h"
#include "UObject/Object.h"
#include "KaosAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	static UKaosAbilitySystemGlobals& Get() { return *Cast<UKaosAbilitySystemGlobals>(IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()); }
	
	TArray<FSoftObjectPath> GetCurveInitTables() const { return GlobalAttributeSetDefaultsTableNames; }

	FKaosAttributeSetInitter* GetKaosAttributeSetInitter() const;

	void InitAttributeSetDefaults(UAbilitySystemComponent* AbilitySystemComponent, const FKaosAttributeInitializationKey& Key, int32 Level, bool bInitialInit);
	void ApplyAttributeSetDefaults(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute& InAttribute, const FKaosAttributeInitializationKey& Key, int32 Level);
	TArray<float> GetAttributeSetValues(UClass* AttributeSetClass, FProperty* AttributeProperty, const FKaosAttributeInitializationKey& Key) const;

	virtual TSharedPtr<FKaosAttributeBasics> AllocKaosAttributeBasics() const;

protected:
	virtual void AllocAttributeSetInitter() override;
};
