#include "PotatoEatingComponent.h"

#include "PotatoGame/PotatoGame.h"
#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Characters/PotatoPickUpComponent.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"

UPotatoEatingComponent::UPotatoEatingComponent()
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UPotatoEatingComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* owner = Cast<AActor>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		UGameplayTagComponent* tagsComponent = owner->GetComponentByClass<UGameplayTagComponent>();
		if (ensure(IsValid(tagsComponent)))
		{
			tagsComponent->GetContainer().AddTag(Character_Behaviour_PotatoEatingCapabale);
		}
	}
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

void UPotatoEatingComponent::SetCaloriesEaten(float calories)
{
	float old = _caloriesEaten;
	_caloriesEaten = calories;
	if (old != _caloriesEaten)
	{
		OnCaloriesEatenChanged.Broadcast();
	}
}

void UPotatoEatingComponent::Server_EatPotato_Implementation(APotato* potato)
{
	Authority_EatPotato(potato);
}

void UPotatoEatingComponent::Authority_EatPotato(APotato* potato)
{
	AActor* owner = GetOwner();
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		const FNutritionalInformations& nutritionalInformations = potato->GetNutritionalInformations();
		float newCalories = nutritionalInformations.GetCalories(potato->GetWeight());
		SetCaloriesEaten(GetCaloriesEaten() + newCalories);
		potato->Destroy();

		UE_LOG(LogPotato, Log, TEXT("Ate potato %s by %s at %s"), *potato->GetName(), *owner->GetName(), *owner->GetTransform().ToString());
	}
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

void UPotatoEatingComponent::Server_EatHeldPotato_Implementation()
{
	Authority_EatHeldPotato();
}
