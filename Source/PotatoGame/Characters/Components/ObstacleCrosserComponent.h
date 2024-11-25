#pragma once

#include "PotatoGame/Characters/Components/ObstacleCrosserBaseComponent.h"

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Containers/EnumAsByte.h"
#include "NativeGameplayTags.h"

#include "ObstacleCrosserComponent.generated.h"

class APotato;
class UInputComponent;

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UObstacleCrosserComponent : public UObstacleCrosserBaseComponent
{
	GENERATED_BODY()

public:	
	UObstacleCrosserComponent();

protected:
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

private:
	void OnGameplayTagChanged(FGameplayTag tag, bool added);
	void UpdateObastacleCrossing(bool enabled);

	FDelegateHandle _tagChangedHandle;
};
