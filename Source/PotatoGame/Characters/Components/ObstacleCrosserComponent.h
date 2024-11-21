#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Containers/EnumAsByte.h"
#include "NativeGameplayTags.h"

#include "ObstacleCrosserComponent.generated.h"

class APotato;
class UInputComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UObstacleCrosserComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UObstacleCrosserComponent();

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

private:
	void OnGameplayTagChanged(FGameplayTag tag, bool added);
	void UpdateObastacleCrossing(bool enabled);

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> _allowedCollisionChannel;
};
