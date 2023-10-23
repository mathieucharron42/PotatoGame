#include "PotatoGathererCharacter.h"

#include "PotatoGame/Characters/PotatoPickUpComponent.h"

APotatoGathererCharacter::APotatoGathererCharacter()
{
	_potatoPickUpComponent = CreateDefaultSubobject<UPotatoPickUpComponent>(TEXT("PotatoPickUpComponent"));
	_potatoPickUpComponent->SetupAttachment(RootComponent);
}