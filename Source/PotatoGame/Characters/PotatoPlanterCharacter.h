#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"

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

};
