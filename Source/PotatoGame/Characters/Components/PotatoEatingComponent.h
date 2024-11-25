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
	UPotatoEatingComponent();

	virtual bool IsHungry() const override;

	virtual float GetCaloriesNeeded() const override;

	virtual float GetCaloriesEaten() const override;

private:
	virtual void Server_EatHeldPotato_Implementation() override;

	virtual void Activate(bool reset) override;
	virtual void Deactivate() override;

	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	void OnSetupPlayerInput(UInputComponent* inputComponent);

	void Authority_EatHeldPotato();
	void Authority_EatPotato(APotato* potato);

	void SetCaloriesEaten(float calories);

	void Authority_SetScale(float scale);
	UFUNCTION()
	void OnReplicate_CurrentScale(float oldScale);
	void Local_UpdateCurrentScale(float oldScale);

	UPROPERTY(Transient, Replicated)
	float _caloriesEaten;

	UPROPERTY(Transient, Replicated, ReplicatedUsing = OnReplicate_CurrentScale)
	float _currentScale = 1.0f;

	UPROPERTY(Transient)
	int32 _initialSpringArmLenght;
};
