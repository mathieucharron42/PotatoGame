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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UPotatoPickUpComponent* _potatoPickUpComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UObstacleCrosserComponent* _obstacleCrosserComponent = nullptr;
};
