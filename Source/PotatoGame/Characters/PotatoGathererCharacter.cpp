#include "PotatoGathererCharacter.h"

#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"
#include "PotatoGame/Characters/Components/ObstacleCrosserComponent.h"

APotatoGathererCharacter::APotatoGathererCharacter()
{
	_potatoPickupComponent = CreateDefaultSubobject<UPotatoPickUpComponent>(TEXT("PotatoPickupComponent"));
	_potatoPickupComponent->SetupAttachment(RootComponent);
	_obstacleCrosserComponent = CreateDefaultSubobject<UObstacleCrosserComponent>(TEXT("ObstacleCrosserComponent"));
	_obstacleCrosserComponent->SetupAttachment(RootComponent);
}
