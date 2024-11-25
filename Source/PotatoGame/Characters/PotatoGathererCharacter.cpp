#include "PotatoGathererCharacter.h"

#include "PotatoGame/Characters/Components/PotatoPickUpBaseComponent.h"
#include "PotatoGame/Characters/Components/ObstacleCrosserBaseComponent.h"

void APotatoGathererCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		SpawnComponent<UPotatoPickUpBaseComponent>(_potatoPickupComponentClass);
		SpawnComponent<UObstacleCrosserBaseComponent>(_obstacleCrosserComponentClass);
	}
}