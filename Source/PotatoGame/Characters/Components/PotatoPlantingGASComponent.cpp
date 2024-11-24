#include "PotatoPlantingGASComponent.h"

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

UPotatoPlantingGASComponent::UPotatoPlantingGASComponent()
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UPotatoPlantingGASComponent::Activate(bool reset)
{
	Super::Activate(reset);
	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(gameplayTagComponent)))
		{
			gameplayTagComponent->Authority_AddTag(Character_Behaviour_PotatoPlantingCapabale);
		}

		UAbilitySystemComponent* abilitySystemComponent = PotatoUtilities::GetComponentByClass<UAbilitySystemComponent>(this);
		if (ensure(IsValid(abilitySystemComponent)))
		{
			FGameplayAbilitySpec abilitySpec(_plantingAbility);
			_abilityHandle = abilitySystemComponent->GiveAbility(abilitySpec);
			ensure(_abilityHandle.IsValid());
		}
	}
}

void UPotatoPlantingGASComponent::Deactivate()
{
	Super::Deactivate();
	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(gameplayTagComponent)))
		{
			gameplayTagComponent->Authority_RemoveTag(Character_Behaviour_PotatoPlantingCapabale);
		}

		UAbilitySystemComponent* abilitySystemComponent = PotatoUtilities::GetComponentByClass<UAbilitySystemComponent>(this);
		if (ensure(IsValid(abilitySystemComponent)))
		{
			abilitySystemComponent->ClearAbility(_abilityHandle);
		}
	}
}

void UPotatoPlantingGASComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

bool UPotatoPlantingGASComponent::CanPlantPotato() const
{
	bool canPlantPotato = false;

	const UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
	if (IsValid(gameplayTagComponent))
	{
		canPlantPotato = !gameplayTagComponent->HasTag(Character_Behaviour_State_PotatoPlantingCooldown);
	}

	return canPlantPotato;
}

void UPotatoPlantingGASComponent::Authority_PlantPotato()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		if (ensure(owner->HasAuthority()))
		{
			UAbilitySystemComponent* abilitySystemComponent = owner->GetComponentByClass<UAbilitySystemComponent>();
			if (ensure(IsValid(abilitySystemComponent)))
			{
				abilitySystemComponent->TryActivateAbility(_abilityHandle);
			}
			
		}
	}
}

void UPotatoPlantingGASComponent::Server_PlantPotato_Implementation()
{
	Authority_PlantPotato();
}

void UPotatoPlantingGASComponent::InitializeComponent()
{
	Super::InitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		// Register to assign component input for next pawn possession
		owner->OnSetupPlayerInput.AddUObject(this, &UPotatoPlantingGASComponent::OnSetupPlayerInput);

		// If input already setup, assign component input now
		if (IsValid(owner->InputComponent))
		{
			OnSetupPlayerInput(owner->InputComponent);
		}
	}
}

void UPotatoPlantingGASComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (IsValid(owner))
	{
		owner->OnSetupPlayerInput.RemoveAll(this);
	}
}

void UPotatoPlantingGASComponent::OnSetupPlayerInput(UInputComponent* inputComponent)
{
	inputComponent->BindAction("Fire", IE_Pressed, this, &UPotatoPlantingGASComponent::Server_PlantPotato);
}

