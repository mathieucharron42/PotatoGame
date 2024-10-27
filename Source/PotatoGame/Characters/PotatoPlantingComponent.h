#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "PotatoPlantingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPlantingComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPotatoPlantingComponent();

	UFUNCTION(Server, Reliable)
	void Server_PlantPotato();

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	void Authority_PlantPotato();

	void OnSetupPlayerInput(UInputComponent* inputComponent);

	UPROPERTY(EditAnywhere)
	FName _spawnSocketName = FName("socket_spawn");

	UPROPERTY(EditAnywhere)
	float _spawnVelocity;

	float _timeUntilNextPlant = 0;
};
