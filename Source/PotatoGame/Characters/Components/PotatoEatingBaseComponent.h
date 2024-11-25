#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PotatoEatingBaseComponent.generated.h"

class APotato;

UCLASS(Abstract, ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoEatingBaseComponent : public USceneComponent
{
	GENERATED_BODY()

public:		
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_EatHeldPotato();

	virtual bool IsHungry() const PURE_VIRTUAL(UPotatoEatingBaseComponent::IsHungry, return false;);

	virtual float GetCaloriesNeeded() const PURE_VIRTUAL(UPotatoEatingBaseComponent::GetCaloriesNeeded, return 0.f;);

	virtual float GetCaloriesEaten() const PURE_VIRTUAL(UPotatoEatingBaseComponent::GetCaloriesEaten, return 0.f;);

protected:
	virtual void Server_EatHeldPotato_Implementation() PURE_VIRTUAL(UPotatoEatingBaseComponent::Server_EatHeldPotato_Implementation);

	UPROPERTY(EditAnywhere)
	float _caloriesNeeded;

	UPROPERTY(EditAnywhere)
	float _caloryScale;
};
