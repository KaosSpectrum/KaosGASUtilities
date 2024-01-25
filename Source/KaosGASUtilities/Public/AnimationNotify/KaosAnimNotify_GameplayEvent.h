// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "UObject/Object.h"
#include "KaosAnimNotify_GameplayEvent.generated.h"

/**
 * Sends out a Gameplay Event Tag when this notify is reached.
 */
UCLASS(MinimalAPI)
class UKaosAnimNotify_GameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UKaosAnimNotify_GameplayEvent();
	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual FString GetNotifyName_Implementation() const override;

protected:
	UPROPERTY(EditAnywhere, Category=GameplayEvent)
	FGameplayTag EventTag;
};
