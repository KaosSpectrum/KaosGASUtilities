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

#include "BehaviourTrees/KaosBTDecorator_IsInRange.h"

#include "BehaviorTree/BlackboardComponent.h"

UKaosBTDecorator_IsInRange::UKaosBTDecorator_IsInRange()
{
	NodeName = "Kaos Is In Range";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	// Accept only actors
	SourceActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UKaosBTDecorator_IsInRange, SourceActorKey), AActor::StaticClass());
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UKaosBTDecorator_IsInRange, TargetActorKey), AActor::StaticClass());

	// Default to using Self Actor
	TargetActorKey.SelectedKeyName = FBlackboard::KeySelf;
}

void UKaosBTDecorator_IsInRange::PostLoad()
{
	Super::PostLoad();

	bNotifyTick = (FlowAbortMode != EBTFlowAbortMode::None);
}

uint16 UKaosBTDecorator_IsInRange::GetInstanceMemorySize() const
{
	return Super::GetInstanceMemorySize();
}

FString UKaosBTDecorator_IsInRange::GetStaticDescription() const
{
	return FString::Printf(TEXT("Is In Range %f between %s and %s"), Range, *SourceActorKey.SelectedKeyName.ToString(), *TargetActorKey.SelectedKeyName.ToString());
}

bool UKaosBTDecorator_IsInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return InternalCalculateRawConditionValue(OwnerComp, NodeMemory);
}

bool UKaosBTDecorator_IsInRange::InternalCalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	const AActor* const SourceActor = Cast<AActor>(MyBlackboard->GetValueAsObject(SourceActorKey.SelectedKeyName));
	const AActor* const TargetActor = Cast<AActor>(MyBlackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (SourceActor && TargetActor)
	{
		const float RangeSq = FMath::Square(Range);
		const float DistanceSq = FVector::DistSquared(SourceActor->GetActorLocation(), TargetActor->GetActorLocation());
		return IsInversed() ? DistanceSq >= RangeSq : DistanceSq <= RangeSq;
	}
	return false;
}

void UKaosBTDecorator_IsInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (GetShouldAbort(OwnerComp, NodeMemory))
	{
		OwnerComp.RequestExecution(this);
	}
}

bool UKaosBTDecorator_IsInRange::GetShouldAbort(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const bool bIsOnActiveBranch = OwnerComp.IsExecutingBranch(GetMyNode(), GetChildIndex());

	bool bShouldAbort;
	if (bIsOnActiveBranch)
	{
		bShouldAbort = (FlowAbortMode == EBTFlowAbortMode::Self || FlowAbortMode == EBTFlowAbortMode::Both) && InternalCalculateRawConditionValue(OwnerComp, NodeMemory) == IsInversed();
	}
	else
	{
		bShouldAbort = (FlowAbortMode == EBTFlowAbortMode::LowerPriority || FlowAbortMode == EBTFlowAbortMode::Both) && InternalCalculateRawConditionValue(OwnerComp, NodeMemory) != IsInversed();
	}

	return bShouldAbort;
}
