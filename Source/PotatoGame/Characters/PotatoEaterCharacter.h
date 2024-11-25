#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"

#include "CoreMinimal.h"

#include "PotatoEaterCharacter.generated.h"

class UPotatoEatingComponent;
class UPotatoPickUpComponent;
class USpringArmComponent;

UCLASS()
class POTATOGAME_API APotatoEaterCharacter : public APotatoBaseCharacter
{
	GENERATED_BODY()

public:

private:
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoPickUpComponent> _potatoPickupComponentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoEatingComponent> _potatoEatingComponentClass;
};
