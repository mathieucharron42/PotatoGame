#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"

#include "CoreMinimal.h"

#include "PotatoEaterCharacter.generated.h"

class UPotatoPickUpComponent;
class UPotatoEatingComponent;

UCLASS()
class POTATOGAME_API APotatoEaterCharacter : public APotatoBaseCharacter
{
	GENERATED_BODY()

public:
	APotatoEaterCharacter();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPotatoPickUpComponent> _potatoPickupComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPotatoEatingComponent> _potatoEatingComponent;
};
