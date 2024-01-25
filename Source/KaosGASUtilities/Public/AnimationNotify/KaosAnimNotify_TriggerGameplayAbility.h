// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "KaosAnimNotify_TriggerGameplayAbility.generated.h"

class UAbilitySystemComponent;
/**
 * **EXPERIMENTAL**
 * Trigger a gameplay ability from a notify. Highly experimental, may have issues.
 */
UCLASS(MinimalAPI)
class UKaosAnimNotify_TriggerGameplayAbility : public UAnimNotify
{
	GENERATED_BODY()

public:
	UKaosAnimNotify_TriggerGameplayAbility();
	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual FString GetNotifyName_Implementation() const override;
#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif

protected:
	void HandleJumpSection(USkeletalMeshComponent* MeshComponent, UAbilitySystemComponent* ASC, UAnimMontage* CurrentMontage);


	UPROPERTY(EditAnywhere, Category=TriggerGameplayAbility)
	FGameplayTag AbilityTriggerTag;

	/** If ability fails, should we jump to a new montage section */
	UPROPERTY(EditAnywhere, Category=TriggerGameplayAbility)
	bool bSetNextMontageSectionIfAbilityFailed;

	/** Section to jump to if ability fails */
	UPROPERTY(EditAnywhere, Category=TriggerGameplayAbility)
	FName NextMontageSectionIfAbilityFailed;
};
