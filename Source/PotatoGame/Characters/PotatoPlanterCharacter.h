#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Crops/Potato.h"

#include "CoreMinimal.h"

#include "PotatoPlanterCharacter.generated.h"

class UPotatoPlantingComponent;
class UPotatoPickUpComponent;

UCLASS(Abstract)
class POTATOGAME_API APotatoPlanterCharacter : public APotatoBaseCharacter
{
	GENERATED_BODY()

public:

private:
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoPlantingComponent> _potatoPlantingComponentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoPickUpComponent> _potatoPickupComponentClass;
};
