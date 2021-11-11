// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "FoodGroup.generated.h"

UCLASS()
class POTATOGATHEREREATER_API UFoodGroup : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	bool HasEatenEnough(int32 servings) 
	{ 
		return servings >= _dailyRecommandedServings; 
	}

private:
	UPROPERTY(EditAnywhere)
	FString _name;

	UPROPERTY(EditAnywhere)
	int32 _dailyRecommandedServings;
};
