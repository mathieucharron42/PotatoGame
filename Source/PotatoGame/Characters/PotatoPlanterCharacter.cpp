#include "PotatoPlanterCharacter.h"

#include "PotatoGame/Gameplay/PotatoGameMode.h"
#include "PotatoGame/Characters/Components/PotatoPlantingComponent.h"
#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"

#include "Components/InputComponent.h"
#include "GameplayTagContainer.h"

void APotatoPlanterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		SpawnComponent<UPotatoPlantingComponent>(_potatoPlantingComponentClass);
		SpawnComponent<UPotatoPickUpComponent>(_potatoPickupComponentClass);
	}
}