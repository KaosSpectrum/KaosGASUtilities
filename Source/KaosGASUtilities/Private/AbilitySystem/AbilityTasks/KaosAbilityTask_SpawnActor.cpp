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
