#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "PotatoPickUpBaseComponent.generated.h"

class APotato;
class UInputComponent;

UCLASS(Abstract, ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPickUpBaseComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	virtual bool IsHoldingPotato() const PURE_VIRTUAL(UPotatoPickUpBaseComponent::IsHoldingPotato, return false;);
	virtual bool IsHoldingPotato(APotato* potato) const PURE_VIRTUAL(UPotatoPickUpBaseComponent::IsHoldingPotato, return false;);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void Server_DropPotato();

	virtual APotato* Authority_DropPotato() PURE_VIRTUAL(UPotatoPickUpBaseComponent::Authority_DropPotato, return nullptr;);
	virtual void Authority_PickupPotato(APotato* potato) PURE_VIRTUAL(UPotatoPickUpBaseComponent::Authority_PickupPotato);

protected:
	virtual void Server_DropPotato_Implementation() PURE_VIRTUAL(UPotatoPickUpBaseComponent::Server_DropPotato_Implementation);

	UPROPERTY(EditAnywhere)
	FName _heldSocketName = FName("socket_hand_r");
};
