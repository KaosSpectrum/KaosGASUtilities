// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "UObject/Object.h"
#include "KaosBTDecorator_IsInRange.generated.h"

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosBTDecorator_IsInRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UKaosBTDecorator_IsInRange();
	virtual void PostLoad() override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	bool InternalCalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	bool GetShouldAbort(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

protected:
	UPROPERTY(EditAnywhere, Category=Range)
	float Range;
	
	UPROPERTY(EditAnywhere, Category=Range)
	FBlackboardKeySelector SourceActorKey;
	
	UPROPERTY(EditAnywhere, Category=Range)
	FBlackboardKeySelector TargetActorKey;
	
};
