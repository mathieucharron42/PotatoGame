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
	
protected:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> _allowedCollisionChannel;
};
