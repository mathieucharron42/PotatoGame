#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"

#include "CoreMinimal.h"

#include "PotatoGathererCharacter.generated.h"

class UPotatoPickUpComponent;
class UObstacleCrosserComponent;

UCLASS(Abstract)
class POTATOGAME_API APotatoGathererCharacter : public APotatoBaseCharacter
{
	GENERATED_BODY()
public:

private:
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoPickUpComponent> _potatoPickupComponentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UObstacleCrosserComponent> _obstacleCrosserComponentClass;
};
