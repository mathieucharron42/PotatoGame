// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FoodGroup.h"
#include "NutritionalInformations.h"

#include "Potato.generated.h"

UCLASS(Abstract)
class POTATOGAME_API APotato : public AActor
{
	GENERATED_BODY()

public:	
	friend class UPotatoCheatManager;

	APotato();

	float GetWeight() const { return _weight; }

	const FNutritionalInformations& GetNutritionalInformations() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	void Cheat_Scale(float scale);

	UPROPERTY(EditAnywhere)
	float _weight;

	UPROPERTY(EditAnywhere)
	FNutritionalInformations _nutritionalInformations;
};
