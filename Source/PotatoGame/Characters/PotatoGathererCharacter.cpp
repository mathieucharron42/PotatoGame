#include "PotatoGathererCharacter.h"

#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"
#include "PotatoGame/Characters/Components/ObstacleCrosserComponent.h"

void APotatoGathererCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		SpawnComponent<UPotatoPickUpComponent>(_potatoPickupComponentClass);
		SpawnComponent<UObstacleCrosserComponent>(_obstacleCrosserComponentClass);
	}
}