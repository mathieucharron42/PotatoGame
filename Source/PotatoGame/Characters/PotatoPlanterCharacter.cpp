#include "PotatoPlanterCharacter.h"

#include "PotatoGame/Gameplay/PotatoGameMode.h"
#include "PotatoGame/Characters/Components/PotatoPlantingBaseComponent.h"
#include "PotatoGame/Characters/Components/PotatoPickUpBaseComponent.h"

#include "Components/InputComponent.h"
#include "GameplayTagContainer.h"

void APotatoPlanterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		SpawnComponent<UPotatoPlantingBaseComponent>(_potatoPlantingComponentClass);
		SpawnComponent<UPotatoPickUpBaseComponent>(_potatoPickupComponentClass);
	}
}