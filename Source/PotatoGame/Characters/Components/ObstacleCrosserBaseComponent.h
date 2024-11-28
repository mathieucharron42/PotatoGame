#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Containers/EnumAsByte.h"
#include "NativeGameplayTags.h"

#include "ObstacleCrosserBaseComponent.generated.h"

class APotato;
class UInputComponent;

UCLASS(Abstract, ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UObstacleCrosserBaseComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UObstacleCrosserBaseComponent();

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

	virtual void BeginPlay() override;

protected:
	virtual void Authority_ActivateObstacleCrossing() PURE_VIRTUAL(UObstacleCrosserBaseComponent::ActivateObstacleCrossing);

protected:	
	void OnGameplayTagChanged(FGameplayTag tag, bool added);
	void OnGameplayTagObstacleCrossing(bool enabled);

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> _allowedCollisionChannel;

	UPROPERTY(EditAnywhere)
	FGameplayTag _activationTag;

	FDelegateHandle _tagChangedHandle;
};
