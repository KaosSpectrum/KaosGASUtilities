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

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "KaosBTTask_ExecuteGameplayAbility.generated.h"

/*
 * Behaviour to do when the Behaviour Tree task is aborted
 */
UENUM(BlueprintType)
enum class EKaosExecuteGameplayAbilityAbortBehaviour
{
	CancelAbility		UMETA(DisplayName = "Cancel Ability on Abort"),
	Nothing				UMETA(DisplayName = "Do Nothing on Abort")
};

/**
 * Executes an ability with the supplied GameplayTag
 */
UCLASS(MinimalAPI)
class UKaosBTTask_ExecuteGameplayAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UKaosBTTask_ExecuteGameplayAbility();
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
protected:
	void HandleAbilityEnded(const FAbilityEndedData& AbilityEndedData, UBehaviorTreeComponent* BehaviorTreeComponent, uint8* NodeMemory);

	/** Target we want to activate ability on (normally self) */
	UPROPERTY(EditAnywhere, Category=ExecuteGameplayAbility)
	FBlackboardKeySelector TargetActorKey;

	/** Tag of the ability we want to activate */
	UPROPERTY(EditAnywhere, Category=ExecuteGameplayAbility)
	FGameplayTag AbilityToActivate;

	/** What do we do if this task is aborted */
	UPROPERTY(EditAnywhere, Category=ExecuteGameplayAbility)
	EKaosExecuteGameplayAbilityAbortBehaviour AbortBehaviour;
};
