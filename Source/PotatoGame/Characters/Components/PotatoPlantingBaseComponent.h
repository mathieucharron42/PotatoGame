#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "PotatoPlantingBaseComponent.generated.h"

UCLASS(Abstract, ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPlantingBaseComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void Server_PlantPotato();

	UFUNCTION(BlueprintCallable)
	virtual bool CanPlantPotato() const PURE_VIRTUAL(UPotatoPlantingBaseComponent::Server_PlantPotato, return false;);

protected:
	virtual void Server_PlantPotato_Implementation() PURE_VIRTUAL(UPotatoPlantingBaseComponent::Server_PlantPotato_Implementation);

	UPROPERTY(EditAnywhere)
	FName _spawnSocketName = FName("socket_spawn");

	UPROPERTY(EditAnywhere)
	float _spawnVelocity = 5;

	UPROPERTY(EditAnywhere)
	float _plantingRate = 1;
};
