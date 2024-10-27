// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PotatoEatingComponent.generated.h"

class APotato;
class UPotatoPickUpComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoEatingComponent : public USceneComponent
{
	GENERATED_BODY()

public:		
	UPotatoEatingComponent();

	DECLARE_MULTICAST_DELEGATE(FCaloriesEatenChanged);
	FCaloriesEatenChanged OnCaloriesEatenChanged;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_EatHeldPotato();

	UFUNCTION(Server, Reliable)
	void Server_EatPotato(APotato* potato);

	bool IsHungry() const;

	float GetCaloriesNeeded() const;

	float GetCaloriesEaten() const;

private:
	virtual void BeginPlay() override;
	void Authority_EatHeldPotato();
	void Authority_EatPotato(APotato* potato);

	virtual void InitializeComponent() override;

	virtual void UninitializeComponent() override;

	void OnSetupPlayerInput(UInputComponent* inputComponent);

	void SetCaloriesEaten(float calories);

	UPROPERTY(EditAnywhere)
	float _caloriesNeeded;

	UPROPERTY(Transient, Replicated)
	float _caloriesEaten;

	UPROPERTY(Transient)
	UPotatoPickUpComponent* _potatoPickUpComponent = nullptr;
};
