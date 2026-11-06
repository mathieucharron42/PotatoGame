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
	APotatoPlanterCharacter();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPotatoPlantingComponent> _potatoPlantingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPotatoPickUpComponent> _potatoPickupComponent;
};
