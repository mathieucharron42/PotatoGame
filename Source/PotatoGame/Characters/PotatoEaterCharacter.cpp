#include "PotatoEaterCharacter.h"

#include "PotatoGame/Characters/Components/PotatoEatingBaseComponent.h"
#include "PotatoGame/Characters/Components/PotatoPickUpBaseComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "Net/UnrealNetwork.h"

void APotatoEaterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		SpawnComponent<UPotatoPickUpBaseComponent>(_potatoPickupComponentClass);
		SpawnComponent<UPotatoEatingBaseComponent>(_potatoEatingComponentClass);
	}
}