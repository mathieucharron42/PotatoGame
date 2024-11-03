#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"

#include "GameplayTagComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UGameplayTagComponent : public UActorComponent, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:	
	UGameplayTagComponent();

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	FGameplayTagContainer& GetContainer() { return _gameplayTagContainer; }

	const FGameplayTagContainer& GetContainer() const { return _gameplayTagContainer; }

protected:
	UPROPERTY(BlueprintReadWrite, Transient)
	FGameplayTagContainer _gameplayTagContainer;
};
