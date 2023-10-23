#include "PotatoPlantingComponent.h"

#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Gameplay/PotatoGameMode.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "Kismet/KismetMathLibrary.h"

static TAutoConsoleVariable<float> CVarPotatoAutoPlantRate (
	TEXT("Potato.AutoPlantPotatoRate"),
	-1.f,
	TEXT("Rate of potato per second (-1 means unset)"),
	ECVF_Cheat
);

UPotatoPlantingComponent::UPotatoPlantingComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UPotatoPlantingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		if (owner->HasAuthority())
		{
			if (CVarPotatoAutoPlantRate.GetValueOnGameThread() != -1)
			{
				_timeUntilNextPlant -= DeltaTime;
				if (_timeUntilNextPlant <= 0)
				{
					Authority_PlantPotato();
					_timeUntilNextPlant = CVarPotatoAutoPlantRate.GetValueOnGameThread();
				}
			}
		}
	}
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
		owner->OnSetupPlayerInput.AddUObject(this, &UPotatoPlantingComponent::OnSetupPlayerInput);
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

