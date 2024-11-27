#pragma once

#include "CoreMinimal.h"

#include "PotatoGame/Characters/Components/PotatoPlantingBaseComponent.h"

#include "Components/SceneComponent.h"

#include "PotatoPlantingComponent.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPlantingComponent : public UPotatoPlantingBaseComponent
{
	GENERATED_BODY()

public:

private:
	virtual void Authority_PlantPotato() override;

	UPROPERTY(EditAnywhere)
	FName _spawnSocketName = FName("socket_spawn");

	UPROPERTY(EditAnywhere)
	float _spawnVelocity = 5;

	UPROPERTY(EditAnywhere)
	float _plantingRate = 1;
};
