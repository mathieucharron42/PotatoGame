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
	APotatoGathererCharacter();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPotatoPickUpComponent> _potatoPickupComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UObstacleCrosserComponent> _obstacleCrosserComponent;
};
