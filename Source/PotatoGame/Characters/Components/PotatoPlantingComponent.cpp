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
#include "PotatoGame/PotatoLogs.h"
#include "GameFramework/Character.h"
#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"

static TAutoConsoleVariable<float> CVarPotatoAutoPlantPotato (
	TEXT("Potato.AutoPlantPotato"),
	0,
	TEXT("Auto plant potato (0 for false, 1 for true)"),
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
			gameplayTagComponent->Authority_AddTag(GameplayTag_Ability_PotatoPlanting_Capabale);
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
			gameplayTagComponent->Authority_RemoveTag(GameplayTag_Ability_PotatoPlanting_Capabale);
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
		canPlantPotato = !gameplayTagComponent->HasTag(GameplayTag_Ability_PotatoPlanting_Cooldown);
	}

	return canPlantPotato;
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
		owner->OnSetupPlayerInput.AddUObject(this, &UPotatoPlantingComponent::SetupPlayerInput);

		// If input already setup, assign component input now
		if (IsValid(owner->InputComponent))
		{
			SetupPlayerInput(owner->InputComponent);
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

void UPotatoPlantingComponent::SetupPlayerInput(UInputComponent* inputComponent)
{
	inputComponent->BindAction("Fire", IE_Pressed, this, &UPotatoPlantingComponent::Server_PlantPotato);
}

void UPotatoPlantingComponent::Authority_PlantPotato()
{
	if (!ensure(PotatoUtilities::HasAuthority(this)) || !CanPlantPotato())
	{
		return;
	}

	ACharacter* character = GetOwner<ACharacter>();
	if (!ensure(IsValid(character)))
	{
		return;
	}

	UWorld* world = character->GetWorld();
	USkeletalMeshComponent* meshComponent = character->GetMesh();
	if (!ensure(IsValid(world)) || !ensure(IsValid(meshComponent)) || !ensure(meshComponent->DoesSocketExist(_spawnSocketName)))
	{
		return;
	}

	APotatoGameMode* gameMode = world->GetAuthGameMode<APotatoGameMode>();
	if (!ensure(IsValid(gameMode)))
	{
		return;
	}

	FTransform newPotatoTransform = meshComponent->GetSocketTransform(_spawnSocketName);
	newPotatoTransform.SetRotation(UKismetMathLibrary::RandomRotator(true).Quaternion());

	FVector newPotatoVelocity = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(character->GetTransform().GetUnitAxis(EAxis::X), 45.f) * _spawnVelocity;
	newPotatoVelocity.Z = FMath::Abs(newPotatoVelocity.Z);

	APotato* newPotato = gameMode->SpawnPotato(newPotatoTransform, newPotatoVelocity);
	if (!ensure(IsValid(newPotato)))
	{
		return;
	}

	UE_LOG(LogPotato, Log, TEXT("Spawned potato %s by %s at %s"), *newPotato->GetName(), *character->GetName(), *character->GetTransform().ToString());

	UGameplayTagComponent* gameplayTagComponent = character->GetComponentByClass<UGameplayTagComponent>();
	if (ensure(IsValid(gameplayTagComponent)))
	{
		gameplayTagComponent->Authority_AddTag(GameplayTag_Ability_PotatoPlanting_Cooldown, _plantingRate);
	}
}
