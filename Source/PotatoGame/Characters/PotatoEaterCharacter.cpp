#include "PotatoEaterCharacter.h"

#include "PotatoGame/Characters/Components/PotatoEatingComponent.h"
#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "Net/UnrealNetwork.h"

APotatoEaterCharacter::APotatoEaterCharacter()
{
	_potatoEatingComponent = CreateDefaultSubobject<UPotatoEatingComponent>(TEXT("PotatoEatingComponent"));
	_potatoEatingComponent->SetupAttachment(RootComponent);

	_potatoPickUpComponent = CreateDefaultSubobject<UPotatoPickUpComponent>(TEXT("PotatoPickUpComponent"));
	_potatoPickUpComponent->SetupAttachment(RootComponent);
}
