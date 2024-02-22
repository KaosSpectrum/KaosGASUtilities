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

#include "BehaviourTrees/KaosBTTask_ExecuteGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/KaosUtilitiesBlueprintLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

struct FKaosBTTask_ExecuteGameplayAbilityMemory
{
	TWeakObjectPtr<UAbilitySystemComponent> CachedAbilitySystemComponent;
	FGameplayAbilitySpecHandle CachedSpecHandle;
};


UKaosBTTask_ExecuteGameplayAbility::UKaosBTTask_ExecuteGameplayAbility()
{
	NodeName = "Kaos Execute Gameplay Ability";

	INIT_TASK_NODE_NOTIFY_FLAGS();

	// Accept only actors
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UKaosBTTask_ExecuteGameplayAbility, TargetActorKey), AActor::StaticClass());

	// Default to using Self Actor
	TargetActorKey.SelectedKeyName = FBlackboard::KeySelf;
}

void UKaosBTTask_ExecuteGameplayAbility::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		TargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UKaosBTTask_ExecuteGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	checkSlow(OwnerComp.GetAIOwner() && OwnerComp.GetBlackboardComponent());

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	const AActor* SelectedActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(TargetActorKey.GetSelectedKeyID()));
	if (!SelectedActor)
	{
		return EBTNodeResult::Failed;
	}

	FKaosBTTask_ExecuteGameplayAbilityMemory* MyMemory = CastInstanceNodeMemory<FKaosBTTask_ExecuteGameplayAbilityMemory>(NodeMemory);

	MyMemory->CachedAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(SelectedActor);
	if (!MyMemory->CachedAbilitySystemComponent.IsValid())
	{
		return EBTNodeResult::Failed;
	}

	TArray<FGameplayAbilitySpec*> OutSpecs;
	MyMemory->CachedAbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(AbilityToActivate), OutSpecs);
	if (OutSpecs.Num() <= 0)
	{
		return EBTNodeResult::Failed;
	}

	const FGameplayAbilitySpec* Spec = OutSpecs[0];
	if (MyMemory->CachedAbilitySystemComponent->TryActivateAbility(Spec->Handle))
	{
		MyMemory->CachedSpecHandle = Spec->Handle;
		MyMemory->CachedAbilitySystemComponent->OnAbilityEnded.AddUObject(this, &UKaosBTTask_ExecuteGameplayAbility::HandleAbilityEnded, &OwnerComp, NodeMemory);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UKaosBTTask_ExecuteGameplayAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FKaosBTTask_ExecuteGameplayAbilityMemory* MyMemory = CastInstanceNodeMemory<FKaosBTTask_ExecuteGameplayAbilityMemory>(NodeMemory);
	if (AbortBehaviour == EKaosExecuteGameplayAbilityAbortBehaviour::CancelAbility && MyMemory->CachedAbilitySystemComponent.IsValid() && MyMemory->CachedSpecHandle.IsValid())
	{
		FGameplayAbilitySpec* Spec = MyMemory->CachedAbilitySystemComponent->FindAbilitySpecFromHandle(MyMemory->CachedSpecHandle);
		if (Spec && Spec->IsActive())
		{
			MyMemory->CachedAbilitySystemComponent->CancelAbilityHandle(MyMemory->CachedSpecHandle);
		}
	}
	MyMemory->CachedSpecHandle = FGameplayAbilitySpecHandle();
	MyMemory->CachedAbilitySystemComponent.Reset();
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UKaosBTTask_ExecuteGameplayAbility::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	FKaosBTTask_ExecuteGameplayAbilityMemory* MyMemory = CastInstanceNodeMemory<FKaosBTTask_ExecuteGameplayAbilityMemory>(NodeMemory);
	MyMemory->CachedSpecHandle = FGameplayAbilitySpecHandle();
	MyMemory->CachedAbilitySystemComponent.Reset();
}

uint16 UKaosBTTask_ExecuteGameplayAbility::GetInstanceMemorySize() const
{
	return sizeof(FKaosBTTask_ExecuteGameplayAbilityMemory);
}

FString UKaosBTTask_ExecuteGameplayAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: AbilityTag: %s\nAbort Behaviour: %s"), *Super::GetStaticDescription(), *AbilityToActivate.ToString(),
	                       *StaticEnum<EKaosExecuteGameplayAbilityAbortBehaviour>()->GetNameStringByValue(static_cast<int64>(AbortBehaviour)));
}

void UKaosBTTask_ExecuteGameplayAbility::HandleAbilityEnded(const FAbilityEndedData& AbilityEndedData, UBehaviorTreeComponent* BehaviorTreeComponent, uint8* NodeMemory)
{
	FKaosBTTask_ExecuteGameplayAbilityMemory* MyMemory = CastInstanceNodeMemory<FKaosBTTask_ExecuteGameplayAbilityMemory>(NodeMemory);
	if (BehaviorTreeComponent && AbilityEndedData.AbilitySpecHandle == MyMemory->CachedSpecHandle)
	{
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
	}
}
