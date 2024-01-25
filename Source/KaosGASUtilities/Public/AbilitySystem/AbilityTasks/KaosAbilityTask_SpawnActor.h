// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/Object.h"
#include "KaosAbilityTask_SpawnActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKaosSpawnActorDelegate, AActor*, SpawnedActor);

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosAbilityTask_SpawnActor : public UAbilityTask
{
	GENERATED_BODY()

	/** Called when the actor is spawned */
	UPROPERTY(BlueprintAssignable)
	FKaosSpawnActorDelegate Success;

	/** Called when we can't spawn: on clients or potentially on server if they fail to spawn (rare) */
	UPROPERTY(BlueprintAssignable)
	FKaosSpawnActorDelegate DidNotSpawn;

	/** Called when the actor is pre-spawned but has not yet called FinishSpawningActor */
	UPROPERTY(BlueprintAssignable)
	FKaosSpawnActorDelegate PreSpawned;

	/** Spawn new Actor on the network authority (server) */
	UFUNCTION(BlueprintCallable, meta=(HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category="Ability|Tasks")
	static UKaosAbilityTask_SpawnActor* SpawnActor(UGameplayAbility* OwningAbility, FGameplayAbilityTargetDataHandle TargetData, TSubclassOf<AActor> Class, AActor* Owner = nullptr, APawn* Instigator = nullptr, int32 TargetDataIndex = 0);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Abilities")
	bool BeginSpawningActor(UGameplayAbility* OwningAbility, FGameplayAbilityTargetDataHandle TargetData, TSubclassOf<AActor> Class, AActor*& SpawnedActor);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Abilities")
	void FinishSpawningActor(UGameplayAbility* OwningAbility, FGameplayAbilityTargetDataHandle TargetData, AActor* SpawnedActor);

protected:
	FGameplayAbilityTargetDataHandle CachedTargetDataHandle;

	UPROPERTY(Transient)
	TObjectPtr<AActor> CachedOwner = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<APawn> CachedInstigator = nullptr;

	int32 CachedTargetDataIndex = 0;
};
