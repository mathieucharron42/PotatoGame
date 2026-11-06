#include "PotatoPlanterCharacter.h"

#include "PotatoGame/Characters/Components/PotatoPlantingComponent.h"
#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"

APotatoPlanterCharacter::APotatoPlanterCharacter()
{
	_potatoPlantingComponent = CreateDefaultSubobject<UPotatoPlantingComponent>(TEXT("PotatoPlantingComponent"));
	_potatoPlantingComponent->SetupAttachment(RootComponent);
	_potatoPickupComponent = CreateDefaultSubobject<UPotatoPickUpComponent>(TEXT("PotatoPickupComponent"));
	_potatoPickupComponent->SetupAttachment(RootComponent);
}
