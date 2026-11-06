#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "PotatoPlantingComponent.generated.h"

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPlantingComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPotatoPlantingComponent();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_PlantPotato();

	UFUNCTION(BlueprintCallable)
	bool CanPlantPotato() const;

protected:
	virtual void Activate(bool reset) override;
	virtual void Deactivate() override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Authority_PlantPotato();

private:
	void SetupPlayerInput(UInputComponent* inputComponent);

	UPROPERTY(EditAnywhere)
	FName _spawnSocketName = FName("socket_spawn");

	UPROPERTY(EditAnywhere)
	float _spawnVelocity = 5;

	UPROPERTY(EditAnywhere)
	float _plantingRate = 1;
};
