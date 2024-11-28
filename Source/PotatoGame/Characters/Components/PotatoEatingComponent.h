// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PotatoGame/Characters/Components/PotatoEatingBaseComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PotatoEatingComponent.generated.h"

class APotato;
class USpringArmComponent;

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoEatingComponent : public UPotatoEatingBaseComponent
{
	GENERATED_BODY()

public:		
	
private:
	virtual void Authority_EatPotato(APotato* potato) override;
};
