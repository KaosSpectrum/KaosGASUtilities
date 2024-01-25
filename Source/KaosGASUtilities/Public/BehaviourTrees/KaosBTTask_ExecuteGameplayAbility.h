// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

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
	CancelAbility UMETA(DisplayName = "Cancel Ability on Abort"),
	Nothing UMETA(DisplayName = "Do Nothing on Abort")
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
