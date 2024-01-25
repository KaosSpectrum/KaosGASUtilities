// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GameplayTagContainer.h"
#include "KaosBTService_ActivateAbilityByTag.generated.h"

/*
 * Behaviour to do when the Behaviour Service ceases relevance
 */
UENUM(BlueprintType)
enum class EKaosActivateAbilityByTagOnCeaseRelevanceBehaviour
{
	CancelAbility UMETA(DisplayName = "Cancel ability when service not relevant"),
	Nothing UMETA(DisplayName = "Do Nothing when service not relevant")
};

/**
 * Try to always run and activate ability with the supplied GameplayTag.
 */
UCLASS(MinimalAPI)
class UKaosBTService_ActivateAbilityByTag : public UBTService
{
	GENERATED_BODY()

public:
	UKaosBTService_ActivateAbilityByTag();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

protected:
	/** Ability to activate */
	UPROPERTY(EditAnywhere, Category=ActivateAbilityByTag)
	FGameplayTag AbilityToActivate;

	/** Target to activate ability on (normally self) */
	UPROPERTY(EditAnywhere, Category=ActivateAbilityByTag)
	FBlackboardKeySelector TargetBlackboardKey;

	/** What to do when service becomes non relevant */
	UPROPERTY(EditAnywhere, Category=ActivateAbilityByTag)
	EKaosActivateAbilityByTagOnCeaseRelevanceBehaviour NodeCeaseRelevanceBehaviour = EKaosActivateAbilityByTagOnCeaseRelevanceBehaviour::Nothing;
};
