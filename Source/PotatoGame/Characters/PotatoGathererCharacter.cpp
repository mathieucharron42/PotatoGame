#include "PotatoGathererCharacter.h"

#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"
#include "PotatoGame/Characters/Components/ObstacleCrosserComponent.h"

APotatoGathererCharacter::APotatoGathererCharacter()
{
	_potatoPickUpComponent = CreateDefaultSubobject<UPotatoPickUpComponent>(TEXT("PotatoPickUpComponent"));
	_potatoPickUpComponent->SetupAttachment(RootComponent);

	_obstacleCrosserComponent = CreateDefaultSubobject<UObstacleCrosserComponent>(TEXT("ObstacleCrosserComponent"));
	_obstacleCrosserComponent->SetupAttachment(RootComponent);
}