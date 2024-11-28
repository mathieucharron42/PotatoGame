#include "PotatoEatingBaseComponent.h"

#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/PotatoLogs.h"
#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

UPotatoEatingBaseComponent::UPotatoEatingBaseComponent()
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	SetIsReplicatedByDefault(true);
}

void UPotatoEatingBaseComponent::Activate(bool reset)
{
	Super::Activate(reset);
	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* tagsComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(tagsComponent)))
		{
			tagsComponent->Authority_AddTag(GameplayTag_Ability_PotatoEating_Capabale);
		}
	}
}

void UPotatoEatingBaseComponent::Deactivate()
{
	Super::Deactivate();
	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* tagsComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(tagsComponent)))
		{
			tagsComponent->Authority_RemoveTag(GameplayTag_Ability_PotatoEating_Capabale);
		}
	}
}

void UPotatoEatingBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPotatoEatingBaseComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPotatoEatingBaseComponent, _caloriesEaten);
}

void UPotatoEatingBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		owner->OnSetupPlayerInput.AddUObject(this, &UPotatoEatingBaseComponent::OnSetupPlayerInput);
	}
}

void UPotatoEatingBaseComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (IsValid(owner))
	{
		owner->OnSetupPlayerInput.RemoveAll(this);
	}
}

void UPotatoEatingBaseComponent::OnSetupPlayerInput(UInputComponent* inputComponent)
{
	if (ensure(IsValid(inputComponent)))
	{
		inputComponent->BindAction("Fire", IE_Pressed, this, &UPotatoEatingBaseComponent::Server_EatHeldPotato);
	}
}

void UPotatoEatingBaseComponent::IncrementCaloriesEaten(float calories)
{
	SetCaloriesEaten(GetCaloriesEaten() + calories);
}

void UPotatoEatingBaseComponent::SetCaloriesEaten(float calories)
{
	float old = _caloriesEaten;
	_caloriesEaten = calories;
}

bool UPotatoEatingBaseComponent::IsHungry() const
{
	return _caloriesEaten < _caloriesNeeded;
}

float UPotatoEatingBaseComponent::GetCaloriesNeeded() const
{
	return _caloriesNeeded;
}

float UPotatoEatingBaseComponent::GetCaloriesEaten() const
{
	return _caloriesEaten;
}

void UPotatoEatingBaseComponent::Server_EatHeldPotato_Implementation()
{
	Authority_EatHeldPotato();
}

void UPotatoEatingBaseComponent::Authority_EatHeldPotato()
{
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		UPotatoPickUpBaseComponent* _potatoPickUpComponent = PotatoUtilities::GetComponentByClass<UPotatoPickUpBaseComponent>(this);
		if (ensure(IsValid(_potatoPickUpComponent)))
		{
			if (_potatoPickUpComponent->IsHoldingPotato())
			{
				APotato* potato = _potatoPickUpComponent->Authority_DropPotato();
				Authority_EatPotato(potato);
			}
		}
	}
}