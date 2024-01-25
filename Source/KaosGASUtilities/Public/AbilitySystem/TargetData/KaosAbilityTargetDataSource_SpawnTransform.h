// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "UObject/Object.h"
#include "KaosAbilityTargetDataSource_SpawnTransform.generated.h"

/** Target data with just a spawn transform */
USTRUCT(BlueprintType)
struct KAOSGASUTILITIES_API FKaosAbilityTargetDataSource_SpawnTransform : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()

	/** Generic location data for target */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Targeting)
	FTransform SpawnTransform;

	virtual bool HasOrigin() const override
	{
		return false;
	}


	virtual bool HasEndPoint() const override
	{
		return true;
	}

	virtual FVector GetEndPoint() const override
	{
		return SpawnTransform.GetLocation();
	}

	virtual FTransform GetEndPointTransform() const override
	{
		return SpawnTransform;
	}

	// -------------------------------------

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual FString ToString() const override
	{
		return TEXT("FKaosAbilityTargetDataSource_SpawnTransform");
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};
