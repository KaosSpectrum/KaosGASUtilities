// Copyright © Daniel Moss
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the disclaimer
// below) provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
// THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
// NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "BehaviourTrees/KaosBTDecorator_GameplayTagQuery.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(KaosBTDecorator_GameplayTagQuery)

struct FKaosBTDecorator_GameplayTagQueryMemory
{
	TWeakObjectPtr<UAbilitySystemComponent> CachedAbilitySystemComponent;

	/** Array of handles for our gameplay tag query delegates */
	TArray<TTuple<FGameplayTag, FDelegateHandle>> GameplayTagEventHandles;
};

UKaosBTDecorator_GameplayTagQuery::UKaosBTDecorator_GameplayTagQuery(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Kaos Gameplay Tag Query";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	// Accept only actors
	ActorForGameplayTagQuery.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UKaosBTDecorator_GameplayTagQuery, ActorForGameplayTagQuery), AActor::StaticClass());

	// Default to using Self Actor
	ActorForGameplayTagQuery.SelectedKeyName = FBlackboard::KeySelf;
}

bool UKaosBTDecorator_GameplayTagQuery::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		// Not calling super here since it returns true
		return false;
	}

	const IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(ActorForGameplayTagQuery.GetSelectedKeyID()));
	if (!GameplayTagAssetInterface)
	{
		// Not calling super here since it returns true
		return false;
	}

	FGameplayTagContainer SelectedActorTags;
	GameplayTagAssetInterface->GetOwnedGameplayTags(SelectedActorTags);

	return GameplayTagQuery.Matches(SelectedActorTags);
}

void UKaosBTDecorator_GameplayTagQuery::OnGameplayTagInQueryChanged(const FGameplayTag InTag, int32 NewCount, TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent, uint8* NodeMemory)
{
	if (!BehaviorTreeComponent.IsValid())
	{
		return;
	}

	ConditionalFlowAbort(*BehaviorTreeComponent, EBTDecoratorAbortRequest::ConditionResultChanged);
}

FString UKaosBTDecorator_GameplayTagQuery::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *GameplayTagQuery.GetDescription());
}

void UKaosBTDecorator_GameplayTagQuery::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	const FKaosBTDecorator_GameplayTagQueryMemory* MyMemory = CastInstanceNodeMemory<FKaosBTDecorator_GameplayTagQueryMemory>(NodeMemory);
	ensureMsgf(MyMemory->GameplayTagEventHandles.Num() == 0, TEXT("Dangling gameplay tag event handles for decorator %s"), *GetStaticDescription());
}

void UKaosBTDecorator_GameplayTagQuery::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		// Not calling super here since it does nothing
		return;
	}

	const AActor* SelectedActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(ActorForGameplayTagQuery.GetSelectedKeyID()));
	if (!SelectedActor)
	{
		// Not calling super here since it does nothing
		return;
	}

	FKaosBTDecorator_GameplayTagQueryMemory* MyMemory = CastInstanceNodeMemory<FKaosBTDecorator_GameplayTagQueryMemory>(NodeMemory);
	MyMemory->CachedAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SelectedActor);

	if (MyMemory->CachedAbilitySystemComponent.IsValid())
	{
		for (const FGameplayTag& CurrentTag : QueryTags)
		{
			FDelegateHandle GameplayTagEventCallbackDelegate = MyMemory->CachedAbilitySystemComponent.Get()->RegisterGameplayTagEvent(CurrentTag, EGameplayTagEventType::Type::AnyCountChange).AddUObject(this, &UKaosBTDecorator_GameplayTagQuery::OnGameplayTagInQueryChanged, TWeakObjectPtr<UBehaviorTreeComponent>(&OwnerComp), NodeMemory);
			MyMemory->GameplayTagEventHandles.Emplace(CurrentTag, GameplayTagEventCallbackDelegate);
		}
	}
}

void UKaosBTDecorator_GameplayTagQuery::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FKaosBTDecorator_GameplayTagQueryMemory* MyMemory = CastInstanceNodeMemory<FKaosBTDecorator_GameplayTagQueryMemory>(NodeMemory);

	if (MyMemory->CachedAbilitySystemComponent.IsValid())
	{
		for (const TTuple<FGameplayTag, FDelegateHandle>& GameplayTagEvent : MyMemory->GameplayTagEventHandles)
		{
			MyMemory->CachedAbilitySystemComponent.Get()->RegisterGameplayTagEvent(GameplayTagEvent.Key, EGameplayTagEventType::Type::AnyCountChange).Remove(GameplayTagEvent.Value);
		}
	}

	MyMemory->GameplayTagEventHandles.Reset();
	MyMemory->CachedAbilitySystemComponent = nullptr;
}

uint16 UKaosBTDecorator_GameplayTagQuery::GetInstanceMemorySize() const
{
	return sizeof(FKaosBTDecorator_GameplayTagQueryMemory);
}

#if WITH_EDITOR
void UKaosBTDecorator_GameplayTagQuery::CacheGameplayTagsInsideQuery()
{
	QueryTags.Reset();
	GameplayTagQuery.GetGameplayTagArray(QueryTags);
}

void UKaosBTDecorator_GameplayTagQuery::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property == nullptr)
	{
		return;
	}

	CacheGameplayTagsInsideQuery();
}
#endif	// WITH_EDITOR

void UKaosBTDecorator_GameplayTagQuery::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		ActorForGameplayTagQuery.ResolveSelectedKey(*BBAsset);
	}
}
