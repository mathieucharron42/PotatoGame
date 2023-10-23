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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UPotatoPlantingComponent* _potatoPlantingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UPotatoPickUpComponent* _potatoPickUpComponent = nullptr;
};
