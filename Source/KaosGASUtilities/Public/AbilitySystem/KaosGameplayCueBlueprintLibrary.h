// Copyright ©2024 Daniel Moss. ©2024 InterKaos Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayEffectTypes.h"
#include "KaosGameplayCueBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosGameplayCueBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Add a gameplay cue locally on Target. Experimental. Use with caution. */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void AddGameplayCueLocal(AActor* Target, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& CueParameters);

	/** Remove a gameplay cue locally on Target. Experimental. Use with caution.  */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void RemoveGameplayCueLocal(AActor* Target, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& CueParameters);

	/** Execute a gameplay cue locally on Target */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void ExecuteGameplayCueLocal(AActor* Target, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& CueParameters);

	/** Build a gameplay cue params from passed in source actor */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void BuildCueParametersFromSource(AActor* SourceActor, FGameplayCueParameters& OutCueParameters);

	/** Build a gameplay cue params from passed in hitresult and source actor */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	static void BuildCueParametersFromHitResult(AActor* SourceActor, FHitResult& HitResult, FGameplayCueParameters& OutCueParameters);
};
