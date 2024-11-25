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
	UGameplayTagComponent();

	FDelegateHandle RegisterTagChange(const FGameplayTagChanged::FDelegate& delegate);
	FDelegateHandle RegisterAndNotifyTagChange(const FGameplayTagChanged::FDelegate& delegate);
	void UnregisterTagChange(FDelegateHandle handle);

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type reason) override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	const FGameplayTagContainer& GetContainer() const { return _gameplayTagContainer; }

	void Authority_AddTag(FGameplayTag tag);

	void Authority_AddTag(FGameplayTag tag, float expiration);

	void Authority_RemoveTag(FGameplayTag tag);

	bool HasTag(FGameplayTag tag) const;

private:
	void NotifyTags(bool added);
	void Authority_OnTagExpired(FGameplayTag tag);

	UFUNCTION()
	void OnReplication_GameplayTagContainer(FGameplayTagContainer old);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, ReplicatedUsing=OnReplication_GameplayTagContainer, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer _gameplayTagContainer;

	FGameplayTagChanged GameplayTagChanged;

	TMap<FGameplayTag, FTimerHandle> _tagExpirations;
};
