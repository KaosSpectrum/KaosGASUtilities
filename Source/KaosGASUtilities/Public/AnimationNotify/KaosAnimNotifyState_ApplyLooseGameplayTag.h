// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "UObject/Object.h"
#include "KaosAnimNotifyState_ApplyLooseGameplayTag.generated.h"

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosAnimNotifyState_ApplyLooseGameplayTag : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UKaosAnimNotifyState_ApplyLooseGameplayTag();
	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;

	virtual FString GetNotifyName_Implementation() const override;
	
#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif
	
protected:
	UPROPERTY(EditAnywhere, Category=GameplayTags)
	FGameplayTagContainer GameplayTagsToApply;
};
