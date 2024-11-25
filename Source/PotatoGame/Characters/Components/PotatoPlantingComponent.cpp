#include "PotatoPlantingComponent.h"

#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/Gameplay/PotatoGameMode.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

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

UPotatoPlantingComponent::UPotatoPlantingComponent()
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UPotatoPlantingComponent::Activate(bool reset)
{
	Super::Activate(reset);
	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(gameplayTagComponent)))
		{
			gameplayTagComponent->Authority_AddTag(Character_Behaviour_PotatoPlantingCapabale);
		}
	}
}

void UPotatoPlantingComponent::Deactivate()
{
	Super::Deactivate();
	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(gameplayTagComponent)))
		{
			gameplayTagComponent->Authority_RemoveTag(Character_Behaviour_PotatoPlantingCapabale);
		}
	}
}

void UPotatoPlantingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		if (owner->IsLocallyControlled())
		{
			if (CVarPotatoAutoPlantPotato.GetValueOnAnyThread())
			{
				Server_PlantPotato();
			}
		}
	}
}

bool UPotatoPlantingComponent::CanPlantPotato() const
{
	bool canPlantPotato = false;

	const UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
	if (IsValid(gameplayTagComponent))
	{
		canPlantPotato = !gameplayTagComponent->HasTag(Character_Behaviour_State_PotatoPlantingCooldown);
	}

	return canPlantPotato;
}

void UPotatoPlantingComponent::Authority_PlantPotato()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		if (ensure(owner->HasAuthority()))
		{
			UWorld* world = GetWorld();
			if (ensure(IsValid(world)))
			{
				USkeletalMeshComponent* meshComponent = owner->GetMesh();
				if (ensure(IsValid(meshComponent)) && ensure(meshComponent->DoesSocketExist(_spawnSocketName)))
				{
					if (CanPlantPotato())
					{
						// Locate socket for location
						FTransform newPotatoTransform = meshComponent->GetSocketTransform(_spawnSocketName);

						// Set random rotation on potato
						newPotatoTransform.SetRotation(UKismetMathLibrary::RandomRotator(true).Quaternion());

						// Set random velocity in 45 degree half-cone 
						FVector newPotatoVelocity = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(owner->GetTransform().GetUnitAxis(EAxis::X), 45.f) * _spawnVelocity;
						newPotatoVelocity.Z = FMath::Abs(newPotatoVelocity.Z);

						APotatoGameMode* gameMode = world->GetAuthGameMode<APotatoGameMode>();
						if (ensure(IsValid(gameMode)))
						{
							APotato* newPotato = gameMode->SpawnPotato(newPotatoTransform, newPotatoVelocity);
							UE_LOG(LogPotato, Log, TEXT("Spawned potato %s by %s at %s"), *newPotato->GetName(), *owner->GetName(), *owner->GetTransform().ToString());

							const float effectivePlantRate = CVarPotatoPlantRate.GetValueOnGameThread() > 0? CVarPotatoPlantRate.GetValueOnGameThread() : _plantingRate;

							UGameplayTagComponent* gameplayTagComponent = owner->GetComponentByClass<UGameplayTagComponent>();
							if (ensure(IsValid(gameplayTagComponent)))
							{
								gameplayTagComponent->Authority_AddTag(Character_Behaviour_State_PotatoPlantingCooldown, effectivePlantRate);
							}
						}
					}
				} 
			}
		}
	}
}

void UPotatoPlantingComponent::Server_PlantPotato_Implementation()
{
	Authority_PlantPotato();
}

void UPotatoPlantingComponent::InitializeComponent()
{
	Super::InitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		// Register to assign component input for next pawn possession
		owner->OnSetupPlayerInput.AddUObject(this, &UPotatoPlantingComponent::OnSetupPlayerInput);

		// If input already setup, assign component input now
		if (IsValid(owner->InputComponent))
		{
			OnSetupPlayerInput(owner->InputComponent);
		}
	}
}

void UPotatoPlantingComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (IsValid(owner))
	{
		owner->OnSetupPlayerInput.RemoveAll(this);
	}
}

void UPotatoPlantingComponent::OnSetupPlayerInput(UInputComponent* inputComponent)
{
	inputComponent->BindAction("Fire", IE_Pressed, this, &UPotatoPlantingComponent::Server_PlantPotato);
}

