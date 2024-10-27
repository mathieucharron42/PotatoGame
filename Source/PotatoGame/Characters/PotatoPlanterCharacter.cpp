#include "PotatoPlanterCharacter.h"

#include "PotatoGame/Gameplay/PotatoGameMode.h"
#include "PotatoGame/Characters/PotatoPlantingComponent.h"
#include "PotatoGame/Characters/PotatoPickUpComponent.h"

#include "Components/InputComponent.h"
#include "GameplayTagContainer.h"


APotatoPlanterCharacter::APotatoPlanterCharacter()
{
	_potatoPlantingComponent = CreateDefaultSubobject<UPotatoPlantingComponent>(TEXT("UPotatoPlantComponent"));
	_potatoPlantingComponent->SetupAttachment(RootComponent);

	_potatoPickUpComponent = CreateDefaultSubobject<UPotatoPickUpComponent>(TEXT("PotatoPickUpComponent"));
	_potatoPickUpComponent->SetupAttachment(RootComponent);
}