// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "AI/Navigation/NavQueryFilter.h"

#include "PotatoBaseCharacter.generated.h"

class UAbilitySystemComponent;
struct FNavigationQueryFilte;

UCLASS(Abstract, config=Game)
class APotatoBaseCharacter : public ACharacter, public IGameplayTagAssetInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UGameplayTagComponent* GameplayTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UNavigationQueryFilter> _navigationQueryFilter;
public:
	APotatoBaseCharacter();

	DECLARE_MULTICAST_DELEGATE_OneParam(FSetupPlayerInput, UInputComponent*);
	FSetupPlayerInput OnSetupPlayerInput;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	TSubclassOf<UNavigationQueryFilter> GetNavigationQueryFilter() { return _navigationQueryFilter; }

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	template<typename ComponentType>
	ComponentType* SpawnComponent(UClass* componentClass)
	{
		ComponentType* component = NewObject<ComponentType>(this, componentClass ? componentClass : ComponentType::StaticClass());
		if (ensure(IsValid(component)))
		{
			component->RegisterComponent();
			component->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
		return component;
	}

private:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

