#include "PotatoPlantingBaseComponent.h"

#include "PotatoGame/AbilitySystem/Abilities/PotatoPlantAbility.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/Gameplay/PotatoGameMode.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "AbilitySystemComponent.h"
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

UPotatoPlantingBaseComponent::UPotatoPlantingBaseComponent()
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UPotatoPlantingBaseComponent::Activate(bool reset)
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

void UPotatoPlantingBaseComponent::Deactivate()
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

void UPotatoPlantingBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

bool UPotatoPlantingBaseComponent::CanPlantPotato() const
{
	bool canPlantPotato = false;

	const UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
	if (IsValid(gameplayTagComponent))
	{
		canPlantPotato = !gameplayTagComponent->HasTag(GameplayTag_Ability_PotatoPlanting_Cooldown);
	}

	return canPlantPotato;
}

void UPotatoPlantingBaseComponent::Server_PlantPotato_Implementation()
{
	Authority_PlantPotato();
}

void UPotatoPlantingBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		// Register to assign component input for next pawn possession
		owner->OnSetupPlayerInput.AddUObject(this, &UPotatoPlantingBaseComponent::SetupPlayerInput);

		// If input already setup, assign component input now
		if (IsValid(owner->InputComponent))
		{
			SetupPlayerInput(owner->InputComponent);
		}
	}
}

void UPotatoPlantingBaseComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (IsValid(owner))
	{
		owner->OnSetupPlayerInput.RemoveAll(this);
	}
}

void UPotatoPlantingBaseComponent::SetupPlayerInput(UInputComponent* inputComponent)
{
	inputComponent->BindAction("Fire", IE_Pressed, this, &UPotatoPlantingBaseComponent::Server_PlantPotato);
}