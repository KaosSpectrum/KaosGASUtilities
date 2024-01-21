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
#include "BehaviorTree/BTService.h"
#include "GameplayTagContainer.h"
#include "KaosBTService_ActivateAbilityByTag.generated.h"

/*
 * Behaviour to do when the Behaviour Service ceases relevance
 */
UENUM(BlueprintType)
enum class EKaosActivateAbilityByTagOnCeaseRelevanceBehaviour
{
	CancelAbility		UMETA(DisplayName = "Cancel ability when service not relevant"),
	Nothing				UMETA(DisplayName = "Do Nothing when service not relevant")
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
