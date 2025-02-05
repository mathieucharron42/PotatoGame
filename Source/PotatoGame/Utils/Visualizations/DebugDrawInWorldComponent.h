#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DebugDrawInWorldComponent.generated.h"

UENUM()
enum class EDrawRate
{
	Once, EveryFrame
};

USTRUCT()
struct FDrawInWorldCommonProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EDrawRate DrawRate = EDrawRate::Once;

	int8 DepthRenderingPriority = 0;

	UPROPERTY(EditAnywhere)
	FColor Color = FColor::White;

	UPROPERTY(EditAnywhere)
	float Lifetime = -1.f;
};

USTRUCT()
struct FDrawInWorldPointProperties : public FDrawInWorldCommonProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Size = 5;
};

USTRUCT()
struct FDrawInWorldBoxProperties : public FDrawInWorldCommonProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector Size = FVector(50.f, 50.f, 50.f);

	UPROPERTY(EditAnywhere)
	bool InheritRotation;

	UPROPERTY(EditAnywhere);
	float Thickness = 2.0f;
};

USTRUCT()
struct FDrawInWorldSphereProperties : public FDrawInWorldCommonProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Radius = 50.f;

	UPROPERTY(EditAnywhere)
	float NbSegments = 10;

	UPROPERTY(EditAnywhere);
	float Thickness = 2.0f;
};

USTRUCT()
struct FDrawInWorldArrowProperties : public FDrawInWorldCommonProperties
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
};

USTRUCT()
struct FDrawInWorldTextProperties : public FDrawInWorldCommonProperties
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
};

UENUM()
enum class ESelfDestructType
{
	None, Actor, Component
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class POTATOGAME_API UDebugDrawInWorldComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UDebugDrawInWorldComponent(const FObjectInitializer& ObjectInitializer);

	void EnableDrawPoint(const FDrawInWorldPointProperties& properties);
	void EnableDrawSphere(const FDrawInWorldSphereProperties& properties);
	void EnableDrawBox(const FDrawInWorldBoxProperties& properties);
	void EnableDrawArrow(const FDrawInWorldArrowProperties& properties);
	void EnableDrawText(const FDrawInWorldTextProperties& properties);
	
	void EnableSelfDestructActor(float delay);
	void EnableSelfDestructComponent(float delay);
	void ForceSelfDestruct();

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool ShouldDraw(bool enabled, bool hasDrawn, EDrawRate drawRate) const;


	UPROPERTY(EditAnywhere, Category = "Debug Point")
	bool _pointEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Point", meta = (EditCondition = "_pointEnabled"))
	FDrawInWorldPointProperties _pointProperties;

	UPROPERTY(Transient)
	bool _hasDrawPoint = false;



	UPROPERTY(EditAnywhere, Category = "Debug Sphere")
	bool _sphereEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Sphere", meta = (EditCondition = "_sphereEnabled"))
	FDrawInWorldSphereProperties _sphereProperties;

	UPROPERTY(Transient)
	bool _hasDrawnSphere = false;



	UPROPERTY(EditAnywhere, Category = "Debug Box")
	bool _boxEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Box", meta = (EditCondition = "_boxEnabled"))
	FDrawInWorldBoxProperties _boxProperties;

	UPROPERTY(Transient)
	bool _hasDrawnBox = false;



	UPROPERTY(EditAnywhere, Category = "Debug Arrow")
	bool _arrowEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Arrow", meta = (EditCondition = "_arrowEnabled"))
	FDrawInWorldArrowProperties _arrowProperties;

	UPROPERTY(Transient)
	bool _hasDrawnArrow = false;



	UPROPERTY(EditAnywhere, Category = "Debug Text")
	bool _textEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debug Text", meta = (EditCondition = "_textEnabled"))
	FDrawInWorldTextProperties _textProperties;

	UPROPERTY(Transient)
	bool _hasDrawnText = false;



	UPROPERTY(EditAnywhere, Category = "Behaviour")
	ESelfDestructType _selfDestructType = ESelfDestructType::None;

	UPROPERTY(EditAnywhere, Category = "Behaviour")
	float _selfDestructDelay = -1;

	UPROPERTY(Transient)
	TOptional<float> _selfDestructTime;
};
