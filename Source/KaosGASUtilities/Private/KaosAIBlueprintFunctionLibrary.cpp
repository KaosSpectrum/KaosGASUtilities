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

#include "KaosAIBlueprintFunctionLibrary.h"

#include "NavigationSystem.h"
#include "AI/Navigation/NavAgentInterface.h"
#include "Engine/Engine.h"

TEnumAsByte<ETeamAttitude::Type> UKaosAIBlueprintFunctionLibrary::GetAttitudeTowards(const AActor* ActorA, const AActor* ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA);
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB);

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB);
}

TEnumAsByte<ETeamAttitude::Type> UKaosAIBlueprintFunctionLibrary::GetAttitudeTowardsInterfaceActor(TScriptInterface<IGenericTeamAgentInterface> ActorA, const AActor* ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA.GetObject());
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB);

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB);
}

TEnumAsByte<ETeamAttitude::Type> UKaosAIBlueprintFunctionLibrary::GetAttitudeTowardsInterface(TScriptInterface<IGenericTeamAgentInterface> ActorA, TScriptInterface<IGenericTeamAgentInterface> ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA.GetObject());
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB.GetObject());

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB);
}

bool UKaosAIBlueprintFunctionLibrary::IsHostile(const AActor* ActorA, const AActor* ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA);
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB);

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) == ETeamAttitude::Hostile;
}

bool UKaosAIBlueprintFunctionLibrary::IsHostileInterfaceActor(TScriptInterface<IGenericTeamAgentInterface> ActorA, const AActor* ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA.GetObject());
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB);

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) == ETeamAttitude::Hostile;
}

bool UKaosAIBlueprintFunctionLibrary::IsHostileInterface(TScriptInterface<IGenericTeamAgentInterface> ActorA, TScriptInterface<IGenericTeamAgentInterface> ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA.GetObject());
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB.GetObject());

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) == ETeamAttitude::Hostile;
}

bool UKaosAIBlueprintFunctionLibrary::IsFriendly(const AActor* ActorA, const AActor* ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA);
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB);

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) == ETeamAttitude::Friendly;
}

bool UKaosAIBlueprintFunctionLibrary::IsFriendlyInterfaceActor(TScriptInterface<IGenericTeamAgentInterface> ActorA, const AActor* ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA.GetObject());
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB);

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) == ETeamAttitude::Friendly;
}

bool UKaosAIBlueprintFunctionLibrary::IsFriendlyInterface(TScriptInterface<IGenericTeamAgentInterface> ActorA, TScriptInterface<IGenericTeamAgentInterface> ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA.GetObject());
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB.GetObject());

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) == ETeamAttitude::Friendly;
}

bool UKaosAIBlueprintFunctionLibrary::IsFriendlyOrNeutralInterfaceActor(TScriptInterface<IGenericTeamAgentInterface> ActorA, const AActor* ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA.GetObject());
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB);

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) != ETeamAttitude::Hostile;
}

bool UKaosAIBlueprintFunctionLibrary::IsFriendlyOrNeutral(const AActor* ActorA, const AActor* ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA);
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB);

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) != ETeamAttitude::Hostile;
}

bool UKaosAIBlueprintFunctionLibrary::IsFriendlyOrNeutralInterface(TScriptInterface<IGenericTeamAgentInterface> ActorA, TScriptInterface<IGenericTeamAgentInterface> ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA.GetObject());
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB.GetObject());

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) != ETeamAttitude::Hostile;
}

bool UKaosAIBlueprintFunctionLibrary::IsHostileOrNeutralInterfaceActor(TScriptInterface<IGenericTeamAgentInterface> ActorA, const AActor* ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA.GetObject());
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB);

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) != ETeamAttitude::Friendly;
}

bool UKaosAIBlueprintFunctionLibrary::IsHostileOrNeutral(const AActor* ActorA, const AActor* ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA);
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB);

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) != ETeamAttitude::Friendly;
}

bool UKaosAIBlueprintFunctionLibrary::IsHostileOrNeutralInterface(TScriptInterface<IGenericTeamAgentInterface> ActorA, TScriptInterface<IGenericTeamAgentInterface> ActorB)
{
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA.GetObject());
	const IGenericTeamAgentInterface* TeamAgentB = Cast<const IGenericTeamAgentInterface>(ActorB.GetObject());

	return GetAttitudeTowards_Internal(TeamAgentA, TeamAgentB) != ETeamAttitude::Friendly;
}

UNavigationSystemV1* UKaosAIBlueprintFunctionLibrary::GetNavigationSystem(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	return World ? Cast<UNavigationSystemV1>(World->GetNavigationSystem()) : nullptr;
}

ANavigationData* UKaosAIBlueprintFunctionLibrary::GetNavigationData(const AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Actor->GetWorld());
	ANavigationData* NavData = nullptr;
	const INavAgentInterface* AsNavAgent = CastChecked<INavAgentInterface>(Actor);
	if (AsNavAgent)
	{
		const FNavAgentProperties& AgentProps = AsNavAgent->GetNavAgentPropertiesRef();
		NavData = NavSys->GetNavDataForProps(AgentProps, AsNavAgent->GetNavAgentLocation());
	}
	if (NavData == nullptr)
	{
		NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	}
	return NavData;
}

ANavigationData* UKaosAIBlueprintFunctionLibrary::GetNavigationDataAgentProps(AActor* InActor, FNavAgentProperties Props)
{
	if (!InActor)
	{
		return nullptr;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(InActor->GetWorld());
	ANavigationData* NavData = NavSys->GetNavDataForProps(Props, InActor->GetActorLocation());

	if (NavData == nullptr)
	{
		NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	}
	return NavData;
}

TEnumAsByte<ETeamAttitude::Type> UKaosAIBlueprintFunctionLibrary::GetAttitudeTowards_Internal(const IGenericTeamAgentInterface* A, const IGenericTeamAgentInterface* B)
{
	if (!A || !B)
	{
		return ETeamAttitude::Neutral;
	}

	if (A->GetGenericTeamId() == 255 || B->GetGenericTeamId() == 255)
	{
		return ETeamAttitude::Neutral;
	}

	return A->GetGenericTeamId() != B->GetGenericTeamId() ? ETeamAttitude::Hostile : ETeamAttitude::Friendly;
}
