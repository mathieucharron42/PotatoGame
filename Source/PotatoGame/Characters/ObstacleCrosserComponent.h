#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Containers/EnumAsByte.h"

#include "ObstacleCrosserComponent.generated.h"

class APotato;
class UInputComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UObstacleCrosserComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UObstacleCrosserComponent();

	virtual void BeginPlay() override;

private:
	void AllowObastacleCrossing();

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> _allowedCollisionChannel;
};
