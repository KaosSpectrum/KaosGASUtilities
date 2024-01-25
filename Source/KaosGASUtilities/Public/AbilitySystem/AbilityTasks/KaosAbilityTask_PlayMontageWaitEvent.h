// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AnimationNotify/KaosAnimNotify_GameplayEvent.h"
#include "UObject/Object.h"
#include "KaosAbilityTask_PlayMontageWaitEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FKaosSimpleMontageWaitEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);


/**
 * Plays a montage and waits for a GameplayEvent whilst the montage is active.
 */
UCLASS(MinimalAPI)
class UKaosAbilityTask_PlayMontageWaitEvent : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FKaosSimpleMontageWaitEventDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FKaosSimpleMontageWaitEventDelegate OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FKaosSimpleMontageWaitEventDelegate OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FKaosSimpleMontageWaitEventDelegate OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FKaosSimpleMontageWaitEventDelegate OnEvent;

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	/** Callback function for when the owning Gameplay Ability is cancelled */
	UFUNCTION()
	void OnGameplayAbilityCancelled();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/** 
	 * Start playing an animation montage on the avatar actor and wait for it to finish
	 * If StopWhenAbilityEnds is true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled.
	 * On normal execution, OnBlendOut is called when the montage is blending out, and OnCompleted when it is completely done playing
	 * OnInterrupted is called if another montage overwrites this, and OnCancelled is called if the ability or task is cancelled
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param MontageToPlay The montage to play on the character
	 * @param Rate Change to play the montage faster or slower
	 * @param StartSection If not empty, named montage section to start from
	 * @param bStopWhenAbilityEnds If true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled
	 * @param AnimRootMotionTranslationScale Change to modify size of root motion or set to 0 to block it entirely
	 * @param StartTimeSeconds Starting time offset in montage, this will be overridden by StartSection if that is also set
	 * @param bAllowInterruptAfterBlendOut If true, you can receive OnInterrupted after an OnBlendOut started (otherwise OnInterrupted will not fire when interrupted, but you will not get OnComplete).
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="PlayMontageAndWaitForEvents",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UKaosAbilityTask_PlayMontageWaitEvent* CreatePlayMontageAndWaitProxyTags(UGameplayAbility* OwningAbility,
	                                                                                FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate = 1.f, FName StartSection = NAME_None,
	                                                                                bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f, bool bAllowInterruptAfterBlendOut = false,
	                                                                                bool bOnlyExact = false);

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="PlayMontageAndWaitForEvent",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UKaosAbilityTask_PlayMontageWaitEvent* CreatePlayMontageAndWaitProxy(UGameplayAbility* OwningAbility,
	                                                                            FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTag EventTag, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true,
	                                                                            float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f, bool bAllowInterruptAfterBlendOut = false, bool bOnlyExact = false);

	void GameplayEventContainerCallback(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData);

	virtual void Activate() override;

	/** Called when the ability is asked to cancel from an outside node. What this means depends on the individual task. By default, this does nothing other than ending the task. */
	virtual void ExternalCancel() override;

	virtual FString GetDebugString() const override;

protected:
	virtual void OnDestroy(bool AbilityEnded) override;

	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY()
	bool bOnlyExact;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	FGameplayTagContainer EventTags;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	float StartTimeSeconds;

	UPROPERTY()
	bool bStopWhenAbilityEnds;

	UPROPERTY()
	bool bAllowInterruptAfterBlendOut;

	FDelegateHandle SingleHandle;
};
