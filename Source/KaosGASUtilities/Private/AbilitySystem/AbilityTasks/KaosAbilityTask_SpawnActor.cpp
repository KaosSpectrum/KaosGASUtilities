// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#include "AbilitySystem/AbilityTasks/KaosAbilityTask_SpawnActor.h"
#include "AbilitySystemComponent.h"

UKaosAbilityTask_SpawnActor* UKaosAbilityTask_SpawnActor::SpawnActor(UGameplayAbility* OwningAbility, FGameplayAbilityTargetDataHandle TargetData, TSubclassOf<AActor> Class, AActor* Owner, APawn* Instigator, int32 TargetDataIndex)
{
	UKaosAbilityTask_SpawnActor* MyObj = NewAbilityTask<UKaosAbilityTask_SpawnActor>(OwningAbility);
	MyObj->CachedTargetDataHandle = MoveTemp(TargetData);
	MyObj->CachedInstigator = Instigator;
	MyObj->CachedOwner = Owner;
	MyObj->CachedTargetDataIndex = TargetDataIndex;
	return MyObj;
}

bool UKaosAbilityTask_SpawnActor::BeginSpawningActor(UGameplayAbility* OwningAbility, FGameplayAbilityTargetDataHandle TargetData, TSubclassOf<AActor> Class, AActor*& SpawnedActor)
{
	if (Ability && Ability->GetCurrentActorInfo()->IsNetAuthority() && ShouldBroadcastAbilityTaskDelegates())
	{
		UWorld* const World = GEngine->GetWorldFromContextObject(OwningAbility, EGetWorldErrorMode::LogAndReturnNull);
		if (World)
		{
			SpawnedActor = World->SpawnActorDeferred<AActor>(Class, FTransform::Identity, CachedOwner, CachedInstigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		}
	}

	if (SpawnedActor == nullptr)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(nullptr);
		}
		return false;
	}

	//Call prespawn delegate so abilities can do stuff before finish spawning actor is called.
	PreSpawned.Broadcast(SpawnedActor);

	return true;
}

void UKaosAbilityTask_SpawnActor::FinishSpawningActor(UGameplayAbility* OwningAbility, FGameplayAbilityTargetDataHandle TargetData, AActor* SpawnedActor)
{
	if (SpawnedActor)
	{
		bool bTransformSet = false;
		FTransform SpawnTransform;
		if (const FGameplayAbilityTargetData* LocationData = CachedTargetDataHandle.Get(CachedTargetDataIndex))
		{
			//Set location. Rotation is unaffected.
			if (LocationData->HasHitResult())
			{
				SpawnTransform.SetLocation(LocationData->GetHitResult()->Location);
				bTransformSet = true;
			}
			else if (LocationData->HasEndPoint())
			{
				SpawnTransform = LocationData->GetEndPointTransform();
				bTransformSet = true;
			}
		}
		if (!bTransformSet)
		{
			if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
			{
				SpawnTransform = ASC->GetOwner()->GetTransform();
			}
		}

		SpawnedActor->FinishSpawning(SpawnTransform);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			Success.Broadcast(SpawnedActor);
		}
	}

	EndTask();
}
