// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PotatoEatingComponent.generated.h"

class APotato;
class USpringArmComponent;
class UPotatoPickUpComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoEatingComponent : public USceneComponent
{
	GENERATED_BODY()

public:		
	UPotatoEatingComponent();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_EatHeldPotato();

	bool IsHungry() const;

	float GetCaloriesNeeded() const;

	float GetCaloriesEaten() const;

private:
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

	UPROPERTY(EditAnywhere)
	float _caloriesNeeded;

	UPROPERTY(Transient, Replicated)
	float _caloriesEaten;

	UPROPERTY(Transient, Replicated, ReplicatedUsing = OnReplicate_CurrentScale)
	float _currentScale = 1.0f;

	UPROPERTY(Transient)
	int32 _initialSpringArmLenght;

	UPROPERTY(EditAnywhere)
	float _caloryScale;
};
