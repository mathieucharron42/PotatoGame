#pragma once

#include "PotatoGame/Characters/Components/ObstacleCrosserBaseComponent.h"

#include "CoreMinimal.h"

#include "ObstacleCrosserComponent.generated.h"

class APotato;
class UInputComponent;

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UObstacleCrosserComponent : public UObstacleCrosserBaseComponent
{
	GENERATED_BODY()

public:
	
protected:
	virtual void Authority_ActivateObstacleCrossing() override;
};
