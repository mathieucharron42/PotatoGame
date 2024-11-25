#include "PotatoEaterCharacter.h"

#include "PotatoGame/Characters/Components/PotatoEatingComponent.h"
#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "Net/UnrealNetwork.h"

void APotatoEaterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		SpawnComponent<UPotatoPickUpComponent>(_potatoPickupComponentClass);
		SpawnComponent<UPotatoEatingComponent>(_potatoEatingComponentClass);
	}
}