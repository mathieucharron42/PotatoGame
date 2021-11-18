#pragma once


#include "Potato.h"
#include "PotatoBaseCharacter.h"

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

#include "PotatoGathererCharacter.generated.h"

UCLASS(Abstract)
class POTATOGATHEREREATER_API APotatoGathererCharacter : public APotatoBaseCharacter
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float dt) override;
	
	void PickupPotato(APotato* potato);
	void DropPotato();
	bool IsHoldingPotato() const;

	UPROPERTY(Transient)
	APotato* _heldPotato = nullptr;

	UPROPERTY(Transient)
	USkeletalMeshComponent* _characterMeshComponent = nullptr;
};
