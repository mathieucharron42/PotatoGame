#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"

#include "CoreMinimal.h"

#include "PotatoEaterCharacter.generated.h"

class UPotatoEatingComponent;
class UPotatoPickUpComponent;
class USpringArmComponent;

UCLASS()
class POTATOGAME_API APotatoEaterCharacter : public APotatoBaseCharacter
{
	GENERATED_BODY()

public:
	APotatoEaterCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type reason) override;
	
private:

	UPROPERTY(Transient, Replicated)
	float _currentScale = 1.0f;

	UPROPERTY(Transient)
	int32 _initialSpringArmLenght;

	UPROPERTY(EditAnywhere)
	float _caloryScale;

	UPROPERTY(Transient)
	USpringArmComponent* _springArmComponent;
};
