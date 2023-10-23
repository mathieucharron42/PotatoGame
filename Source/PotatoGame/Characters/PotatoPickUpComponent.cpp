#include "PotatoPickUpComponent.h"

#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"

UPotatoPickUpComponent::UPotatoPickUpComponent()
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UPotatoPickUpComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPotatoPickUpComponent, _heldPotato);
}

void UPotatoPickUpComponent::InitializeComponent()
{
	Super::InitializeComponent();
	APotatoBaseCharacter* owner = Cast<APotatoBaseCharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		_characterMeshComponent = owner->FindComponentByClass<USkeletalMeshComponent>();
		owner->OnActorBeginOverlap.AddDynamic(this, &UPotatoPickUpComponent::OnOwnerOverlap);
		owner->OnActorHit.AddDynamic(this, &UPotatoPickUpComponent::OnOwnerHit);
		owner->OnSetupPlayerInput.AddUObject(this, &UPotatoPickUpComponent::OnSetupPlayerInput);
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
	AActor* owner = GetOwner();
	if (ensure(IsValid(owner)))
	{
		if (owner->HasAuthority())
		{
			if (otherActor->IsA<APotato>())
			{
				APotato* potato = Cast<APotato>(otherActor);
				Authority_PickupPotato(potato);
			}
		}
	}
}

void UPotatoPickUpComponent::OnOwnerHit(AActor* owningActor, AActor* otherActor, FVector normalImpulse, const FHitResult& hit)
{
	AActor* owner = GetOwner();
	if (ensure(IsValid(owner)))
	{
		if (owner->HasAuthority())
		{
			if (otherActor->IsA<APotato>())
			{
				APotato* potato = Cast<APotato>(otherActor);
				Authority_PickupPotato(potato);
			}
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
		if (ensure(owner->HasAuthority()))
		{
			if (!IsHoldingPotato())
			{
				SetHeldPotato(potato);
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
	if (ensure(IsValid(owner)))
	{
		if (ensure(owner->HasAuthority()))
		{
			if (IsHoldingPotato())
			{
				potato = _heldPotato;
				SetHeldPotato(nullptr);
				UE_LOG(LogPotato, Log, TEXT("Pick up potato %s by %s"), *potato->GetName(), *owner->GetName());
			}
		}
	}

	return potato;
}

bool UPotatoPickUpComponent::IsHoldingPotato() const
{
	return IsValid(_heldPotato);
}

bool UPotatoPickUpComponent::IsHoldingPotato(APotato* potato) const
{
	return IsHoldingPotato() && _heldPotato == potato;
}

void UPotatoPickUpComponent::OnRep_HeldPotato(APotato* old)
{
	OnUpdate_HeldPotato(old);
}

void UPotatoPickUpComponent::SetHeldPotato(APotato* potato)
{
	APotato* previous = _heldPotato;
	_heldPotato = potato;
	OnUpdate_HeldPotato(previous);
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
		targetComponent->AttachToComponent(_characterMeshComponent, attachementRules, _heldSocketName);
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