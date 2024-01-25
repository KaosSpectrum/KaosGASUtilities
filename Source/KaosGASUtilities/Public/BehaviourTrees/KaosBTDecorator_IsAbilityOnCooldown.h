// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "KaosBTDecorator_IsAbilityOnCooldown.generated.h"

/**
 * Check to see if an ability is on cooldown with the supplied AbilityTag
 */
UCLASS(MinimalAPI)
class UKaosBTDecorator_IsAbilityOnCooldown : public UBTDecorator
{
	GENERATED_BODY()

public:
	UKaosBTDecorator_IsAbilityOnCooldown();
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
