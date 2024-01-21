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
	void AddGameplayCueLocal(AActor* Target, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& CueParameters);

	/** Remove a gameplay cue locally on Target. Experimental. Use with caution.  */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	void RemoveGameplayCueLocal(AActor* Target, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& CueParameters);

	/** Execute a gameplay cue locally on Target */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	void ExecuteGameplayCueLocal(AActor* Target, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& CueParameters);

	/** Build a gameplay cue params from passed in source actor */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	void BuildCueParametersFromSource(AActor* SourceActor, FGameplayCueParameters& OutCueParameters);

	/** Build a gameplay cue params from passed in hitresult and source actor */
	UFUNCTION(BlueprintCallable, Category="KaosGAS")
	void BuildCueParametersFromHitResult(AActor* SourceActor, FHitResult& HitResult, FGameplayCueParameters& OutCueParameters);
};
