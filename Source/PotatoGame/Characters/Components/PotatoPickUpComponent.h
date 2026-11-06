#pragma once


#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "PotatoPickUpComponent.generated.h"

class APotato;
class UInputComponent;

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPickUpComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_DropPotato();

	UPotatoPickUpComponent();

	UFUNCTION(BlueprintCallable)
	bool IsHoldingPotato() const;
	bool IsHoldingPotato(APotato* potato) const;

	APotato* Authority_DropPotato();

	void Authority_PickupPotato(APotato* potato);

private:
	virtual void Activate(bool reset) override;
	virtual void Deactivate() override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void Server_DropPotato_Implementation();

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

	UPROPERTY(EditAnywhere)
	FName _heldSocketName = FName("socket_hand_r");
};
