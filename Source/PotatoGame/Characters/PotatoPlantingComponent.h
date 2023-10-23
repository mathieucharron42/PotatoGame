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

	virtual void TickComponent(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	void Authority_PlantPotato();

	UFUNCTION(Server, Reliable)
	void Server_PlantPotato();

protected:
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

private:
	void OnSetupPlayerInput(UInputComponent* inputComponent);

	UPROPERTY(EditAnywhere)
	FName _spawnSocketName = FName("socket_spawn");

	UPROPERTY(EditAnywhere)
	float _spawnVelocity;

	float _timeUntilNextPlant = 0;
};
