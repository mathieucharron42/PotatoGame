#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

#include "GameplayTagComponent.generated.h"

enum class TagOperation { Added, Removed };
DECLARE_MULTICAST_DELEGATE_TwoParams(FGameplayTagChanged, FGameplayTag /*tag*/, bool /*added*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UGameplayTagComponent : public UActorComponent, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	FGameplayTagChanged GameplayTagChanged;

	UGameplayTagComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type reason) override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	const FGameplayTagContainer& GetContainer() const { return _gameplayTagContainer; }

	void AddTag(FGameplayTag tag);

	void RemoveTag(FGameplayTag tag);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer _gameplayTagContainer;
};
