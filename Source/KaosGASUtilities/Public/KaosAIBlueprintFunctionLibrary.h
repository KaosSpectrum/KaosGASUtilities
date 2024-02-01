// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KaosAIBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosAIBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	* @brief Get the attitude between 2 actors
	* @param ActorA First actor 
	* @param ActorB Second Actor
	* @return true if both actors are valid and actors are neutral towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Get Attitude Towards"))
	static TEnumAsByte<ETeamAttitude::Type> GetAttitudeTowards(const AActor* ActorA, const AActor* ActorB);

	/**
	* @brief Get the attitude towards interface and actor
	* @param ActorA First actor 
	* @param ActorB Second Actor
	* @return true if both actors are valid and actors are neutral towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Get Attitude Towards"))
	static TEnumAsByte<ETeamAttitude::Type> GetAttitudeTowardsInterfaceActor(TScriptInterface<IGenericTeamAgentInterface> ActorA, const AActor* ActorB);


	/**
	* @brief Get the attitude between 2 interfaces
	* @param ActorA First actor 
	* @param ActorB Second Actor
	* @return true if both actors are valid and actors are neutral towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Get Attitude Towards"))
	static TEnumAsByte<ETeamAttitude::Type> GetAttitudeTowardsInterface(TScriptInterface<IGenericTeamAgentInterface> ActorA, TScriptInterface<IGenericTeamAgentInterface> ActorB);

	
	/**
	* @brief Tests actor's for hostility (Actor version)
	* @param ActorA First actor 
	* @param ActorB Second Actor
	* @return true if both actors are valid and actors are hostile towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Hostile"))
	static bool IsHostile(const AActor* ActorA, const AActor* ActorB);

	/**
	* @brief Tests actor's for hostility (Interface actor version)
	* @param ActorA First actor 
	* @param ActorB Second Actor
	* @return true if both actors are valid and actors are hostile towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Hostile"))
    static bool IsHostileInterfaceActor(TScriptInterface<IGenericTeamAgentInterface> ActorA, const AActor* ActorB);

	/**
	* @brief Tests actor's for hostility (Interface version)
	* @param ActorA First actor 
	* @param ActorB Second Actor
	* @return true if both actors are valid and actors are hostile towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Hostile"))
	static bool IsHostileInterface(TScriptInterface<IGenericTeamAgentInterface> ActorA, TScriptInterface<IGenericTeamAgentInterface> ActorB);

	/**
* @brief Tests actor's for Friendly (Actor version)
* @param ActorA First actor 
* @param ActorB Second Actor
* @return true if both actors are valid and actors are Friendly towards each other.
*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Friendly"))
	static bool IsFriendly(const AActor* ActorA, const AActor* ActorB);

	/**
	* @brief Tests actor's for Friendly (Interface actor version)
	* @param ActorA First actor 
	* @param ActorB Second Actor
	* @return true if both actors are valid and actors are Friendly towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Friendly"))
	static bool IsFriendlyInterfaceActor(TScriptInterface<IGenericTeamAgentInterface> ActorA, const AActor* ActorB);

	/**
	* @brief Tests actor's for Friendly (Interface version)
	* @param ActorA First actor 
	* @param ActorB Second Actor
	* @return true if both actors are valid and actors are Friendly towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Friendly"))
	static bool IsFriendlyInterface(TScriptInterface<IGenericTeamAgentInterface> ActorA, TScriptInterface<IGenericTeamAgentInterface> ActorB);
	
	/**
	* @brief Tests actor's for hostility and neutrality (Team interface and Actor version)
	* @param ActorA First actor (Team interface)
	* @param ActorB Second Actor
	* @return true if both actors are valid and actors are neutral or hostile towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Hostile or Neutral"))
	static bool IsHostileOrNeutralInterfaceActor(TScriptInterface<IGenericTeamAgentInterface> ActorA, const AActor* ActorB);

	/**
	 * @brief Tests actor's for hostility and neutrality (Actor version)
	 * @param ActorA First actor
	 * @param ActorB Second Actor
	 * @return true if both actors are valid and actors are neutral or hostile towards each other.
	 */
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Hostile or Neutral"))
	static bool IsHostileOrNeutral(const AActor* ActorA, const AActor* ActorB);

	/**
	* @brief Tests actor's for hostility and neutrality (Team interface version)
	* @param ActorA First actor (Team Interface)
	* @param ActorB Second Actor (Team Interface)
	* @return true if both actors are valid and actors are neutral or hostile towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Hostile or Neutral"))
	static bool IsHostileOrNeutralInterface(TScriptInterface<IGenericTeamAgentInterface> ActorA, TScriptInterface<IGenericTeamAgentInterface> ActorB);

	/**
* @brief Tests actor's for Friendly and neutrality (Team interface and Actor version)
* @param ActorA First actor (Team interface)
* @param ActorB Second Actor
* @return true if both actors are valid and actors are neutral or Friendly towards each other.
*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Friendly or Neutral"))
	static bool IsFriendlyOrNeutralInterfaceActor(TScriptInterface<IGenericTeamAgentInterface> ActorA, const AActor* ActorB);

	/**
	 * @brief Tests actor's for Friendly and neutrality (Actor version)
	 * @param ActorA First actor
	 * @param ActorB Second Actor
	 * @return true if both actors are valid and actors are neutral or Friendly towards each other.
	 */
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Friendly or Neutral"))
	static bool IsFriendlyOrNeutral(const AActor* ActorA, const AActor* ActorB);

	/**
	* @brief Tests actor's for Friendly and neutrality (Team interface version)
	* @param ActorA First actor (Team Interface)
	* @param ActorB Second Actor (Team Interface)
	* @return true if both actors are valid and actors are neutral or Friendly towards each other.
	*/
	UFUNCTION(BlueprintPure, Category = "Perception", meta = (DisplayName = "Is Friendly or Neutral"))
	static bool IsFriendlyOrNeutralInterface(TScriptInterface<IGenericTeamAgentInterface> ActorA, TScriptInterface<IGenericTeamAgentInterface> ActorB);


	/**
	 * @brief Get the navigation system for the given world context object.
	 * @param WorldContextObject The object representing the world context.
	 * @return The navigation system associated with the world context object.
	 */
	UFUNCTION(BlueprintPure, Category = "Navigation", Meta = (WorldContext = "WorldContextObject", CompactNodeTitle = "Navigation System"))
	static class UNavigationSystemV1* GetNavigationSystem(const UObject* WorldContextObject);

	/**
	 * @brief Retrieves the navigation data associated with the given actor.
	 *
	 * This function returns the navigation data object associated with the specified actor.
	 * The navigation data is used by the pathfinding system to generate paths and navigate
	 * through the game world. If no navigation data is found, nullptr is returned.
	 *
	 * @param Actor The actor for which to retrieve the navigation data.
	 * @return A pointer to the navigation data object. If no navigation data is found, nullptr is returned.
	 */
	UFUNCTION(BlueprintPure, Category = "Navigation", Meta = (WorldContext = "WorldContextObject", DisplayName = "Get Navigation Data"))
	static ANavigationData* GetNavigationData(const AActor* Actor);

	/**
	 * \brief Retrieves the navigation data associated with an actor using the specified agent properties.
	 *
	 * This function returns the navigation data associated with a given actor. The navigation data is filtered using the specified agent properties.
	 *
	 * \param InActor The actor for which to retrieve the navigation data.
	 * \param Props The agent properties used to filter the navigation data.
	 * \return The navigation data associated with the actor, filtered by the agent properties.
	 *
	 * \see ANavigationData
	 * \see FNavAgentProperties
	 */
	UFUNCTION(BlueprintPure, Category = "Navigation", Meta = (WorldContext = "WorldContextObject", DisplayName = "Get Navigation Data With Props"))
	static ANavigationData* GetNavigationDataAgentProps(AActor* InActor, FNavAgentProperties Props);

private:
	static TEnumAsByte<ETeamAttitude::Type> GetAttitudeTowards_Internal(const IGenericTeamAgentInterface* A, const IGenericTeamAgentInterface* B);
};


