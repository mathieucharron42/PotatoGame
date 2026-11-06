#include "PotatoEatingComponent.h"
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

UPotatoEatingComponent::UPotatoEatingComponent()
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	SetIsReplicatedByDefault(true);
}

void UPotatoEatingComponent::Activate(bool reset)
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

void UPotatoEatingComponent::Deactivate()
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

void UPotatoEatingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPotatoEatingComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPotatoEatingComponent, _caloriesEaten);
}

void UPotatoEatingComponent::InitializeComponent()
{
	Super::InitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		owner->OnSetupPlayerInput.AddUObject(this, &UPotatoEatingComponent::OnSetupPlayerInput);
	}
}

void UPotatoEatingComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (IsValid(owner))
	{
		owner->OnSetupPlayerInput.RemoveAll(this);
	}
}

void UPotatoEatingComponent::OnSetupPlayerInput(UInputComponent* inputComponent)
{
	if (ensure(IsValid(inputComponent)))
	{
		inputComponent->BindAction("Fire", IE_Pressed, this, &UPotatoEatingComponent::Server_EatHeldPotato);
	}
}

void UPotatoEatingComponent::IncrementCaloriesEaten(float calories)
{
	SetCaloriesEaten(GetCaloriesEaten() + calories);
}

void UPotatoEatingComponent::SetCaloriesEaten(float calories)
{
	float old = _caloriesEaten;
	_caloriesEaten = calories;
}

bool UPotatoEatingComponent::IsHungry() const
{
	return _caloriesEaten < _caloriesNeeded;
}

float UPotatoEatingComponent::GetCaloriesNeeded() const
{
	return _caloriesNeeded;
}

float UPotatoEatingComponent::GetCaloriesEaten() const
{
	return _caloriesEaten;
}

void UPotatoEatingComponent::Server_EatHeldPotato_Implementation()
{
	Authority_EatHeldPotato();
}

void UPotatoEatingComponent::Authority_EatHeldPotato()
{
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		UPotatoPickUpComponent* _potatoPickUpComponent = PotatoUtilities::GetComponentByClass<UPotatoPickUpComponent>(this);
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

void UPotatoEatingComponent::Authority_EatPotato(APotato* potato)
{
	AActor* owner = GetOwner();
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		if (ensure(IsValid(potato)))
		{
			const FNutritionalInformations& nutritionalInformations = potato->GetNutritionalInformations();
			const float newCalories = nutritionalInformations.GetCalories(potato->GetWeight());
			potato->Destroy();
			IncrementCaloriesEaten(newCalories);

			UE_LOG(LogPotato, Log, TEXT("Ate potato %s by %s at %s"), *potato->GetName(), *owner->GetName(), *owner->GetTransform().ToString());
		}
	}
}
