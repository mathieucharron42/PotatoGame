#include "PotatoEaterCharacter.h"

#include "PotatoGame/Characters/Components/PotatoEatingComponent.h"
#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"

APotatoEaterCharacter::APotatoEaterCharacter()
{
	_potatoPickupComponent = CreateDefaultSubobject<UPotatoPickUpComponent>(TEXT("PotatoPickupComponent"));
	_potatoPickupComponent->SetupAttachment(RootComponent);
	_potatoEatingComponent = CreateDefaultSubobject<UPotatoEatingComponent>(TEXT("PotatoEatingComponent"));
	_potatoEatingComponent->SetupAttachment(RootComponent);
}
