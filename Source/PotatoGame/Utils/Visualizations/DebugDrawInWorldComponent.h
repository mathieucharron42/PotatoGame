#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DebugDrawInWorldComponent.generated.h"

USTRUCT()
struct FDrawInWorldPointProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Size = 5;

	UPROPERTY(EditAnywhere)
	FColor Color = FColor::White;

	UPROPERTY(EditAnywhere)
	float Lifetime = -1.f;
};

USTRUCT()
struct FDrawInWorldBoxProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector Size = FVector(50.f, 50.f, 50.f);

	UPROPERTY(EditAnywhere)
	bool InheritRotation;

	UPROPERTY(EditAnywhere)
	FColor Color = FColor::White;

	UPROPERTY(EditAnywhere);
	float Thickness = 2.0f;

	UPROPERTY(EditAnywhere)
	float Lifetime = -1.f;
};

USTRUCT()
struct FDrawInWorldSphereProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Radius = 50.f;

	UPROPERTY(EditAnywhere)
	float NbSegments = 10;

	UPROPERTY(EditAnywhere)
	FColor Color = FColor::White;

	UPROPERTY(EditAnywhere);
	float Thickness = 2.0f;

	UPROPERTY(EditAnywhere)
	float Lifetime = 0.f;
};

USTRUCT()
struct FDrawInWorldArrowProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector Direction = FVector::XAxisVector;

	UPROPERTY(EditAnywhere)
	bool InheritRotation = true;

	UPROPERTY(EditAnywhere);
	float Lenght = 100.0f;

	UPROPERTY(EditAnywhere);
	float Thickness = 2.0f;

	UPROPERTY(EditAnywhere);
	float Size = 20.0f;

	UPROPERTY(EditAnywhere)
	FColor Color = FColor::White;

	UPROPERTY(EditAnywhere)
	float Lifetime = 0.f;
};

USTRUCT()
struct FDrawInWorldTextProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector Position;

	UPROPERTY(EditAnywhere)
	bool InheritRotation;

	UPROPERTY(EditAnywhere);
	float Size = 1.0f;

	UPROPERTY(EditAnywhere)
	FString Text;

	UPROPERTY(EditAnywhere)
	FColor Color = FColor::White;

	UPROPERTY(EditAnywhere)
	float Lifetime = 0.f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class POTATOGAME_API UDebugDrawInWorldComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UDebugDrawInWorldComponent(const FObjectInitializer& ObjectInitializer);

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Debug Point")
	bool _pointEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Point", meta = (EditCondition = "_pointEnabled"))
	FDrawInWorldPointProperties _pointProperties;

	UPROPERTY(EditAnywhere, Category = "Debug Sphere")
	bool _sphereEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Sphere", meta = (EditCondition = "_sphereEnabled"))
	FDrawInWorldSphereProperties _sphereProperties;

	UPROPERTY(EditAnywhere, Category = "Debug Box")
	bool _boxEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Box", meta = (EditCondition = "_boxEnabled"))
	FDrawInWorldBoxProperties _boxProperties;

	UPROPERTY(EditAnywhere, Category = "Debug Arrow")
	bool _arrowEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Arrow", meta = (EditCondition = "_arrowEnabled"))
	FDrawInWorldArrowProperties _arrowProperties;

	UPROPERTY(EditAnywhere, Category = "Debug Text")
	bool _textEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Text", meta = (EditCondition = "_textEnabled"))
	FDrawInWorldTextProperties _textProperties;
};
