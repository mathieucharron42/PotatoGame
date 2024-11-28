// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PotatoEatingBaseComponent.generated.h"

class APotato;
class USpringArmComponent;

UCLASS(ClassGroup = (Custom), Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class POTATOGAME_API UPotatoEatingBaseComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPotatoEatingBaseComponent();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_EatHeldPotato();

	bool IsHungry() const;

	float GetCaloriesNeeded() const;

	float GetCaloriesEaten() const;

	void IncrementCaloriesEaten(float calories);

protected:
	virtual void Authority_EatHeldPotato();

	virtual void Activate(bool reset) override;
	virtual void Deactivate() override;

	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	
private:
	virtual void Authority_EatPotato(APotato* potato) PURE_VIRTUAL(UPotatoEatingBaseComponent::Authority_EatPotato);

	void OnSetupPlayerInput(UInputComponent* inputComponent);
	void SetCaloriesEaten(float calories);

	UPROPERTY(Transient, Replicated)
	float _caloriesEaten;

	UPROPERTY(EditAnywhere)
	float _caloriesNeeded;

	UPROPERTY(EditAnywhere)
	float _caloryScale;
};
