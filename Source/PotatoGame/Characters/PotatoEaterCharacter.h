#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"

#include "CoreMinimal.h"

#include "PotatoEaterCharacter.generated.h"

class UPotatoEatingBaseComponent;
class UPotatoPickUpBaseComponent;
class USpringArmComponent;

UCLASS()
class POTATOGAME_API APotatoEaterCharacter : public APotatoBaseCharacter
{
	GENERATED_BODY()

public:

private:
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoPickUpBaseComponent> _potatoPickupComponentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoEatingBaseComponent> _potatoEatingComponentClass;
};
