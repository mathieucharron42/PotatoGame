#include "PotatoPlantAbility.h"

#include "PotatoGame/PotatoLogs.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Gameplay/PotatoGameMode.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarPotatoPlantRate(
	TEXT("Potato.PlantRate"),
	-1,
	TEXT("Overrides plant rate (-1 for default"),
	ECVF_Cheat
);

bool UPotatoPlantAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayTagContainer* SourceTags, 
	const FGameplayTagContainer* TargetTags, 
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(Character_Behaviour_State_PotatoPlantingCooldown))
	{
		return false;
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UPotatoPlantAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Authority_ActivateAbility(Handle, ActorInfo, ActivationInfo);
}

void UPotatoPlantAbility::Authority_ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ensure(HasAuthority(&ActivationInfo)))
	{
		TWeakObjectPtr<ACharacter> owner = Cast<ACharacter>(ActorInfo->AvatarActor);

		bool success = Authority_PlantPotato(owner.Get(), _spawnSocketName, _spawnVelocity);

		if (success)
		{
			const float effectivePlantRate = CVarPotatoPlantRate.GetValueOnGameThread() > 0 ? CVarPotatoPlantRate.GetValueOnGameThread() : _plantingRate;

			FGameplayEffectSpec gameplayEffectSpec(
				_effect.GetDefaultObject(),
				MakeEffectContext(Handle, ActorInfo)
			);

			ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(gameplayEffectSpec);
		}
	}
}

bool UPotatoPlantAbility::Authority_PlantPotato(ACharacter* character, FName spawnSocketName, float spawnVelocity)
{
	bool success = false;
	if (ensure(IsValid(character)))
	{
		UWorld* world = character->GetWorld();
		if (ensure(IsValid(world)))
		{
			USkeletalMeshComponent* meshComponent = character->GetMesh();
			if (ensure(IsValid(meshComponent)) && ensure(meshComponent->DoesSocketExist(spawnSocketName)))
			{
				// Locate socket for location
				FTransform newPotatoTransform = meshComponent->GetSocketTransform(spawnSocketName);

				// Set random rotation on potato
				newPotatoTransform.SetRotation(UKismetMathLibrary::RandomRotator(true).Quaternion());

				// Set random velocity in 45 degree half-cone 
				FVector newPotatoVelocity = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(character->GetTransform().GetUnitAxis(EAxis::X), 45.f) * spawnVelocity;
				newPotatoVelocity.Z = FMath::Abs(newPotatoVelocity.Z);

				APotatoGameMode* gameMode = world->GetAuthGameMode<APotatoGameMode>();
				if (ensure(IsValid(gameMode)))
				{
					APotato* newPotato = gameMode->SpawnPotato(newPotatoTransform, newPotatoVelocity);
					UE_LOG(LogPotato, Log, TEXT("Spawned potato %s by %s at %s"), *newPotato->GetName(), *character->GetName(), *character->GetTransform().ToString());
					success = true;
				}
			}
		}
	}
	return success;
}