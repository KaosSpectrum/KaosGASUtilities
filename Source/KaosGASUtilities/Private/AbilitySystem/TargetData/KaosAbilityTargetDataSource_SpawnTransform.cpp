// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "AbilitySystem/TargetData/KaosAbilityTargetDataSource_SpawnTransform.h"

bool FKaosAbilityTargetDataSource_SpawnTransform::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	Ar << SpawnTransform;

	bOutSuccess = true;
	return true;
}
