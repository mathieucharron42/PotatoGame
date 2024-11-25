#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Crops/Potato.h"

#include "CoreMinimal.h"

#include "PotatoPlanterCharacter.generated.h"

class UPotatoPlantingBaseComponent;
class UPotatoPickUpBaseComponent;

UCLASS(Abstract)
class POTATOGAME_API APotatoPlanterCharacter : public APotatoBaseCharacter
{
	GENERATED_BODY()

public:

private:
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoPlantingBaseComponent> _potatoPlantingComponentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoPickUpBaseComponent> _potatoPickupComponentClass;
};
