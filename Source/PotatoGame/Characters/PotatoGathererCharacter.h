#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"

#include "CoreMinimal.h"

#include "PotatoGathererCharacter.generated.h"

class UPotatoPickUpBaseComponent;
class UObstacleCrosserBaseComponent;

UCLASS(Abstract)
class POTATOGAME_API APotatoGathererCharacter : public APotatoBaseCharacter
{
	GENERATED_BODY()
public:

private:
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoPickUpBaseComponent> _potatoPickupComponentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UObstacleCrosserBaseComponent> _obstacleCrosserComponentClass;
};
