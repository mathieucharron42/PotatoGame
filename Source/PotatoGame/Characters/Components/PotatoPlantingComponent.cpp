#include "PotatoPlantingComponent.h"

#include "PotatoGame/AbilitySystem/Abilities/PotatoPlantAbility.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/PotatoLogs.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarPotatoAutoPlantPotato (
	TEXT("Potato.AutoPlantPotato"),
	0,
	TEXT("Auto plant potato (0 for false, 1 for true)"),
	ECVF_Cheat
);

static TAutoConsoleVariable<float> CVarPotatoPlantRate(
	TEXT("Potato.PlantRate"),
	-1,
	TEXT("Overrides plant rate (-1 for default"),
	ECVF_Cheat
);

void UPotatoPlantingComponent::Authority_PlantPotato()
{
	if (CanPlantPotato())
	{
		ACharacter* character = GetOwner<ACharacter>();
		if (IsValid(character))
		{
			bool success = UPotatoPlantAbility::Authority_PlantPotato(character, _spawnSocketName, _spawnVelocity);
			if (success)
			{
				const float effectivePlantRate = CVarPotatoPlantRate.GetValueOnGameThread() > 0? CVarPotatoPlantRate.GetValueOnGameThread() : _plantingRate;

				UGameplayTagComponent* gameplayTagComponent = character->GetComponentByClass<UGameplayTagComponent>();
				if (ensure(IsValid(gameplayTagComponent)))
				{
					gameplayTagComponent->Authority_AddTag(Character_Behaviour_State_PotatoPlantingCooldown, effectivePlantRate);
				}
			}
		}
	}
}

