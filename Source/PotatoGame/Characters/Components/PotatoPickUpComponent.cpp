#include "PotatoPickUpComponent.h"

#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/PotatoLogs.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "GameplayTagContainer.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

UPotatoPickUpComponent::UPotatoPickUpComponent()
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	SetIsReplicatedByDefault(true);
}

void UPotatoPickUpComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPotatoPickUpComponent, _heldPotato);
}

void UPotatoPickUpComponent::Activate(bool reset)
{
	Super::Activate();

	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (IsValid(gameplayTagComponent))
		{
			gameplayTagComponent->Authority_AddTag(Character_Behaviour_PotatoPickupCapabale);
		}
	}
}

void UPotatoPickUpComponent::Deactivate()
{
	Super::Deactivate();

	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (IsValid(gameplayTagComponent))
		{
			gameplayTagComponent->Authority_RemoveTag(Character_Behaviour_PotatoPickupCapabale);
		}
	}
}

void UPotatoPickUpComponent::InitializeComponent()
{
	Super::InitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		owner->OnActorBeginOverlap.AddDynamic(this, &UPotatoPickUpComponent::OnOwnerOverlap);
		owner->OnActorHit.AddDynamic(this, &UPotatoPickUpComponent::OnOwnerHit);

		// Register to assign component input for next pawn possession
		owner->OnSetupPlayerInput.AddUObject(this, &UPotatoPickUpComponent::OnSetupPlayerInput);

		// If input already setup, assign component input now
		if (IsValid(owner->InputComponent))
		{
			OnSetupPlayerInput(owner->InputComponent);
		}
	}
}

void UPotatoPickUpComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (IsValid(owner))
	{
		owner->OnActorBeginOverlap.RemoveDynamic(this, &UPotatoPickUpComponent::OnOwnerOverlap);
		owner->OnActorHit.RemoveDynamic(this, &UPotatoPickUpComponent::OnOwnerHit);
		owner->OnSetupPlayerInput.RemoveAll(this);
	}
}

void UPotatoPickUpComponent::OnOwnerOverlap(AActor* owningActor, AActor* otherActor)
{
	if (PotatoUtilities::HasAuthority(this))
	{
		if (otherActor->IsA<APotato>())
		{
			APotato* potato = Cast<APotato>(otherActor);
			Authority_PickupPotato(potato);
		}
	}
}

void UPotatoPickUpComponent::OnOwnerHit(AActor* owningActor, AActor* otherActor, FVector normalImpulse, const FHitResult& hit)
{
	if (PotatoUtilities::HasAuthority(this))
	{
		if (otherActor->IsA<APotato>())
		{
			APotato* potato = Cast<APotato>(otherActor);
			Authority_PickupPotato(potato);
		}
	}
}

void UPotatoPickUpComponent::OnSetupPlayerInput(UInputComponent* inputComponent)
{
	inputComponent->BindAction("Release", IE_Pressed, this, &UPotatoPickUpComponent::Server_DropPotato);
}

void UPotatoPickUpComponent::Authority_PickupPotato(APotato* potato)
{
	TRACE_BOOKMARK(TEXT("Potato pickup"));
	TRACE_CPUPROFILER_EVENT_SCOPE("UPotatoPickUpComponent::Authority_PickupPotato")

	AActor* owner = GetOwner();
	if (ensure(IsValid(owner)))
	{
		if (ensure(PotatoUtilities::HasAuthority(owner)))
		{
			if (!IsHoldingPotato())
			{
				Authority_SetHeldPotato(potato);
				UE_LOG(LogPotato, Log, TEXT("Pick up potato %s by %s at %s"), *potato->GetName(), *owner->GetName(), *owner->GetTransform().ToString());
			}
		}
	}
}

void UPotatoPickUpComponent::Server_DropPotato_Implementation()
{
	Authority_DropPotato();
}

APotato* UPotatoPickUpComponent::Authority_DropPotato()
{
	APotato* potato = nullptr;

	AActor* owner = GetOwner();
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		if (IsHoldingPotato())
		{
			potato = _heldPotato;
			Authority_SetHeldPotato(nullptr);
			UE_LOG(LogPotato, Log, TEXT("Pick up potato %s by %s"), *potato->GetName(), *owner->GetName());
		}
	}

	return potato;
}

bool UPotatoPickUpComponent::IsHoldingPotato() const
{
	bool isHoldingPotato = false;
	if (IsValid(_heldPotato))
	{
		const UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(gameplayTagComponent)))
		{
			if (gameplayTagComponent->GetOwnedGameplayTags().HasTag(Character_Behaviour_State_HoldingPotato))
			{
				isHoldingPotato = true;
			}
		}
	}
	return isHoldingPotato;
}

bool UPotatoPickUpComponent::IsHoldingPotato(APotato* potato) const
{
	return IsHoldingPotato() && _heldPotato == potato;
}

void UPotatoPickUpComponent::OnReplicate_HeldPotato(APotato* old)
{
	OnUpdate_HeldPotato(old);
}

void UPotatoPickUpComponent::Authority_SetHeldPotato(APotato* potato)
{
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		APotato* previous = _heldPotato;
		_heldPotato = potato;

		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (IsValid(gameplayTagComponent))
		{
			if (_heldPotato)
			{
				gameplayTagComponent->Authority_AddTag(Character_Behaviour_State_HoldingPotato);
			}
			else
			{
				gameplayTagComponent->Authority_RemoveTag(Character_Behaviour_State_HoldingPotato);
			}
		}

		OnUpdate_HeldPotato(previous);
	}
}

void UPotatoPickUpComponent::OnUpdate_HeldPotato(APotato* old)
{
	if (IsValid(_heldPotato))
	{
		UPrimitiveComponent* targetComponent = Cast<UPrimitiveComponent>(_heldPotato->GetRootComponent());

		targetComponent->SetSimulatePhysics(false);
		targetComponent->SetEnableGravity(false);
		targetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FAttachmentTransformRules attachementRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
		attachementRules.bWeldSimulatedBodies = true;
		attachementRules.ScaleRule = EAttachmentRule::KeepRelative;
		USkeletalMeshComponent* characterMeshComponent = PotatoUtilities::GetComponentByClass<USkeletalMeshComponent>(this);
		targetComponent->AttachToComponent(characterMeshComponent, attachementRules, _heldSocketName);
	}
	else if (IsValid(old))
	{
		UPrimitiveComponent* targetComponent = Cast<UPrimitiveComponent>(old->GetRootComponent());

		FDetachmentTransformRules detachementRules = FDetachmentTransformRules::KeepWorldTransform;
		detachementRules.ScaleRule = EDetachmentRule::KeepRelative;
		old->DetachFromActor(detachementRules);

		targetComponent->SetSimulatePhysics(true);
		targetComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		targetComponent->SetEnableGravity(true);
	}
	PotatoUtilities::DoSomething(0.8f);
}