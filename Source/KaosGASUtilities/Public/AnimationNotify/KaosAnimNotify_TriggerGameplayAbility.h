// Copyright (C) 2024, Daniel Moss
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

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
