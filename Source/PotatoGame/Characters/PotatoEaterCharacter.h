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

private:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type reason) override;

	void OnCaloriesEatenChanged();

	void Authority_SetScale(float scale);
	UFUNCTION()
	void OnReplicate_CurrentScale(float oldScale);
	void OnUpdate_CurrentScale(float oldScale);

	UPROPERTY(Transient, Replicated, ReplicatedUsing=OnReplicate_CurrentScale)
	float _currentScale = 1.0f;

	UPROPERTY(Transient)
	int32 _initialSpringArmLenght;

	UPROPERTY(EditAnywhere)
	float _caloryScale;

	UPROPERTY(Transient)
	USpringArmComponent* _springArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UPotatoEatingComponent* _potatoEatingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UPotatoPickUpComponent* _potatoPickUpComponent;
};
