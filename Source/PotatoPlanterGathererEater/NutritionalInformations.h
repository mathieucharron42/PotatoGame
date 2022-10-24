#pragma once

#include "CoreMinimal.h"

#include "NutritionalInformations.generated.h"

USTRUCT()
struct POTATOPLANTERGATHEREREATER_API FNutritionalInformations
{
	GENERATED_BODY()

public:
	int32 GetServingSize() const 
	{ 
		return _servingSize; 
	}

	float GetCalories(float size) const 
	{ 
		return _calories / (float) _servingSize * size;
	}
	
	float GetCarbs(float size) const
	{ 
		return _carbs / (float)_servingSize * size;
	}
	
	float GetFat(float size) const
	{ 
		return _fat / (float)_servingSize * size;
	}
	
	float GetPoteins(float size) const
	{ 
		return _proteins / (float)_servingSize * size;
	}

private:
	UPROPERTY(EditAnywhere)
	int32 _servingSize;

	UPROPERTY(EditAnywhere)
	float _calories;

	UPROPERTY(EditAnywhere)
	float _carbs;

	UPROPERTY(EditAnywhere)
	float _fat;

	UPROPERTY(EditAnywhere)
	float _proteins;
};
