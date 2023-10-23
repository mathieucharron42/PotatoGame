#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DebugDrawOnScreenComponent.generated.h"

USTRUCT()
struct FDrawOnScreenLineProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector2D Start;

	UPROPERTY(EditAnywhere)
	FVector2D End;

	UPROPERTY(EditAnywhere)
	FLinearColor Color;

	UPROPERTY(EditAnywhere)
	float Thickness = 1.f;
};

USTRUCT()
struct FDrawOnScreenBoxProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBox2D Box;

	UPROPERTY(EditAnywhere)
	FLinearColor Color = FColor::Black;

	UPROPERTY(EditAnywhere)
	float Thickness = 1.f;
};

USTRUCT()
struct FDrawOnScreenCircleProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector2D Center;

	UPROPERTY(EditAnywhere)
	float Radius = 5;

	UPROPERTY(EditAnywhere)
	int32 NbSide = 10;

	UPROPERTY(EditAnywhere)
	FLinearColor Color = FColor::Black;

	UPROPERTY(EditAnywhere)
	float Thickness = 1.f;
};

USTRUCT()
struct FDrawOnScreenTextProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FColor Color = FColor::Black;

	UPROPERTY(EditAnywhere)
	FText Message;

	UPROPERTY(EditAnywhere)
	FVector2D Position = FVector2D(0, 0);

	UPROPERTY(EditAnywhere)
	FVector2D Scale = FVector2D(1, 1);
};

class UCanvas;
class AHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class POTATOGAME_API UDebugDrawOnScreenComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called when the game starts
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	void OnPostRender(AHUD* HUD, UCanvas* Canvas);

	UPROPERTY(EditAnywhere, Category = "Debug Line")
	bool _lineEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Line", meta = (EditCondition = "_lineEnabled"))
	FDrawOnScreenLineProperties _lineProperties;

	UPROPERTY(EditAnywhere, Category = "Debug Box")
	bool _boxEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Box", meta = (EditCondition = "_boxEnabled"))
	FDrawOnScreenBoxProperties _boxProperties;

	UPROPERTY(EditAnywhere, Category = "Debug Circle")
	bool _circleEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Circle", meta = (EditCondition = "_circleEnabled"))
	FDrawOnScreenCircleProperties _circleProperties;

	UPROPERTY(EditAnywhere, Category = "Debug Text")
	bool _textEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Text", meta = (EditCondition = "_textEnabled"))
	FDrawOnScreenTextProperties _textProperties;
};