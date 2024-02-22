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
#include "UObject/Object.h"
#include "KaosMathStatics.generated.h"

/**
 * 
 */
UCLASS()
class KAOSGASUTILITIES_API UKaosMathStatics : public UObject
{
	GENERATED_BODY()

	/**
	 * Generate a random sample of specified size from an exponential distribution.
	 *
	 * @param Mean The mean of the exponential distribution.
	 * @param SampleSize The size of the random sample to generate.
	 * @param bUseLowerBounds Flag indicating whether to use lower bounds for the generated values.
	 * @param LowerBounds The lower bounds for the generated values. Only used if bUseLowerBounds is true.
	 * @param bUseUpperBounds Flag indicating whether to use upper bounds for the generated values.
	 * @param UpperBounds The upper bounds for the generated values. Only used if bUseUpperBounds is true.
	 * @return The generated random sample as an array of floats.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static TArray<float> ExponentialDistributionRandomSampleSize(float Mean, int32 SampleSize, bool bUseLowerBounds, float LowerBounds, bool bUseUpperBounds, float UpperBounds);

	/**
	 * Clamps a point within a cone defined by its origin, direction, and half angle.
	 *
	 * @param ConeOrigin The origin of the cone.
	 * @param ConeDirection The direction of the cone.
	 * @param ConeHalfAngleDegrees The half angle of the cone in degrees.
	 * @param [in,out] TargetPoint The point to clamp within the cone.
	 * @param [out] bTargetPointAdjusted True if the point was adjusted to be within the cone, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static void ClampPointWithinCone(const FVector& ConeOrigin, const FVector& ConeDirection, float ConeHalfAngleDegrees, UPARAM(ref) FVector& TargetPoint, bool& bTargetPointAdjusted);

	/**
	 * Checks if a point is inside a cone specified by its origin, direction, and half angle.
	 *
	 * @param ConeOrigin The origin of the cone.
	 * @param ConeDirection The direction of the cone.
	 * @param ConeHalfAngle The half angle of the cone. The actual angle of the cone will be twice this value.
	 * @param Point The point to check.
	 *
	 * @return true if the point is inside the cone, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static bool IsInCone(const FVector& ConeOrigin, const FVector& ConeDirection, float ConeHalfAngle, const FVector& Point);

	/**
	 * Calculate the distance between two actors.
	 *
	 * @param Actor1 Pointer to the first actor.
	 * @param Actor2 Pointer to the second actor.
	 * @return The distance between the two actors.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static float DistanceBetweenActors(AActor* Actor1, AActor* Actor2);

	/**
	 * Calculates the distance between two actors in a 2D space.
	 *
	 * @param Actor1 The first actor.
	 * @param Actor2 The second actor.
	 * @return The distance between Actor1 and Actor2 in 2D space.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static float DistanceBetweenActors2D(AActor* Actor1, AActor* Actor2);

	/**
	 * \fn bool IsPointInsidePolygon(const FVector2D& TestPoint, const TArray<FVector2D>& PolygonPoints)
	 *
	 * \brief Determines whether a given point is inside a polygon.
	 *
	 * \param TestPoint The point to test if it is inside the polygon.
	 * \param PolygonPoints The list of points defining the vertices of the polygon.
	 *
	 * \return true if the TestPoint is inside the polygon, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static bool IsPointInsidePolygon(const FVector2D& TestPoint, const TArray<FVector2D>& PolygonPoints);

	/**
	 * GetEquidistantTargetLocationsInCone calculates equidistant target locations within a cone shape.
	 *
	 * @param InMiddleTargetLocation The middle target location within the cone.
	 * @param InCenterLocation The center location of the cone.
	 * @param InDistBetweenTargets The distance between each target location.
	 * @param InNumTargets The number of target locations to calculate within the cone.
	 * @return An array of equidistant target locations within the cone.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static TArray<FVector> GetEquidistantTargetLocationsInCone(const FVector& InMiddleTargetLocation, const FVector& InCenterLocation, float InDistBetweenTargets, int32 InNumTargets);

	/**
	 * Generates a random number from an exponential distribution.
	 *
	 * @param Mean The mean of the distribution.
	 * @param bUseLowerBounds Whether to use lower bounds for the generated number.
	 * @param LowerBounds The lower bound for the generated number.
	 * @param bUseUpperBounds Whether to use upper bounds for the generated number.
	 * @param UpperBounds The upper bound for the generated number.
	 *
	 * @return A random number from the exponential distribution.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static float ExponentialDistributionRandom(float Mean, bool bUseLowerBounds, float LowerBounds, bool bUseUpperBounds, float UpperBounds);


	/**
	 * @brief Generates a random float and negates it.
	 *
	 * @param InFloat The input float value.
	 * @return The generated random float value negated.
	 *
	 * @note This function returns a random float by multiplying a random value between 0 and 1 with the input float. The result is then negated.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static float RandomNegateFloat(float InFloat);

	/**
	 * \brief Calculates the intersection point of two lines in 2D space.
	 *
	 * This function determines whether two lines intersect in 2D space, and if so, calculates the intersection point.
	 * The lines are defined by their start and end points.
	 * The function returns true if the lines intersect, false otherwise.
	 *
	 * \param L1Start The start point of the first line.
	 * \param L1End The end point of the first line.
	 * \param L2Start The start point of the second line.
	 * \param L2End The end point of the second line.
	 * \param Intersect The reference to a FVector2D variable to store the intersection point if it exists.
	 * \param Tolerance The tolerance value to consider lines parallel and not intersecting.
	 *
	 * \returns True if the lines intersect, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static bool LineIntersect(const FVector2D& L1Start, const FVector2D& L1End, const FVector2D& L2Start, const FVector2D& L2End, FVector2D& Intersect, float Tolerance);

	/**
	 * Converts a timespan to a float value.
	 *
	 * @param InTimespan The timespan to convert.
	 *
	 * @return The float representation of the timespan.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static float TimeSpanToFloat(const FTimespan& InTimespan);


	/**
	 * Calculates the shorted angle between two vectors in degrees.
	 *
	 * @param VectorA The first vector.
	 * @param VectorB The second vector.
	 * @return The angle between the two vectors in degrees.
	 */
	UFUNCTION(BlueprintPure, Category = "Math")
	static float AngleBetweenVectors(const FVector& VectorA, const FVector& VectorB);
};
