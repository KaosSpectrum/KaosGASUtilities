// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GameplayTagContainer.h"
#include "KaosBTDecorator_HasGameplayAbility.generated.h"

/**
 * Check if we have an ability with the supplied AbilityTag
 */
UCLASS(MinimalAPI)
class UKaosBTDecorator_HasGameplayAbility : public UBTDecorator
{
	GENERATED_BODY()

public:
	UKaosBTDecorator_HasGameplayAbility();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	/** Target we want to check if we have ability on (normally self) */
	UPROPERTY(EditAnywhere, Category=HasGameplayAbility)
	FBlackboardKeySelector TargetActorKey;

	/** Tag of the ability we want to see if they have */
	UPROPERTY(EditAnywhere, Category=HasGameplayAbility)
	FGameplayTag AbilityTag;
};
