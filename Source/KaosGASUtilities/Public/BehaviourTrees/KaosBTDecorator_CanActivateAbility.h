// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GameplayTagContainer.h"

#include "KaosBTDecorator_CanActivateAbility.generated.h"

/**
 * Checks to see if we can activate an ability with the supplied ability tag
 */
UCLASS(MinimalAPI)
class UKaosBTDecorator_CanActivateAbility : public UBTDecorator
{
	GENERATED_BODY()

public:
	UKaosBTDecorator_CanActivateAbility(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;

protected:
	/** Actor for ability activation (normally self) */
	UPROPERTY(EditAnywhere, Category=CanActivateAbility, Meta=(ToolTips="Which Actor (from the blackboard) should be checked for this gameplay tag query?"))
	FBlackboardKeySelector ActorForAbilityActivation;

	/** Tag for ability to check if we can activate */
	UPROPERTY(EditAnywhere, Category=CanActivateAbility)
	FGameplayTag AbilityTag;
};
