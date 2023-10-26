#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"

#include "CoreMinimal.h"

#include "PotatoGathererCharacter.generated.h"

class UPotatoPickUpComponent;

UCLASS(Abstract)
class POTATOGAME_API APotatoGathererCharacter : public APotatoBaseCharacter
{
	GENERATED_BODY()
public:
	APotatoGathererCharacter();
};
