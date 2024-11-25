#pragma once

#include "PotatoGame/Characters/Components/PotatoPlantingBaseComponent.h"
#include "PotatoGame/Characters/Components/PotatoPickUpBaseComponent.h"

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "PotatoPickUpComponent.generated.h"

class APotato;
class UInputComponent;

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPickUpComponent : public UPotatoPickUpBaseComponent
{
	GENERATED_BODY()

public:	
	UPotatoPickUpComponent();

	virtual bool IsHoldingPotato() const override;
	virtual bool IsHoldingPotato(APotato* potato) const override;

	virtual APotato* Authority_DropPotato() override;

	virtual void Authority_PickupPotato(APotato* potato) override;

private:
	virtual void Activate(bool reset) override;
	virtual void Deactivate() override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void Server_DropPotato_Implementation() override;

	UFUNCTION()
	void OnOwnerOverlap(AActor* owningActor, AActor* otherActor);
	
	UFUNCTION()
	void OnOwnerHit(AActor* owningActor, AActor* otherActor, FVector normalImpulse, const FHitResult& hit);

	void OnSetupPlayerInput(UInputComponent* inputComponent);

	UFUNCTION()
	void OnReplicate_HeldPotato(APotato* old);

	void Authority_SetHeldPotato(APotato* potato);

	void OnUpdate_HeldPotato(APotato* old);

	UPROPERTY(Transient, Replicated, ReplicatedUsing = OnReplicate_HeldPotato)
	APotato* _heldPotato = nullptr;
};
