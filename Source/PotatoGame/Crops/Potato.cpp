#include "Potato.h"

#include "PotatoGame/PotatoLogs.h"
#include "PotatoGame/Crops/PotatoManagerSubsystem.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

APotato::APotato()
{
	bReplicates = true;
	SetReplicateMovement(true);
}

void APotato::BeginPlay()
{
	TRACE_BOOKMARK(TEXT("Potato spawn"));
	Super::BeginPlay();

	UPotatoManagerSubsystem* potatoManager = GetGameInstance()->GetSubsystem<UPotatoManagerSubsystem>();
	potatoManager->RegisterPotato(this);


	UE_LOG(LogPotato, Log, TEXT("Spawned potato %s of type %s at %s"), *GetName(), *GetClass()->GetName(), *GetTransform().ToString())
}

void APotato::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	TRACE_BOOKMARK(TEXT("Potato unspawn"));
	Super::EndPlay(endPlayReason);

	UPotatoManagerSubsystem* potatoManager = GetGameInstance()->GetSubsystem<UPotatoManagerSubsystem>();
	potatoManager->UnregisterPotato(this);

	UE_LOG(LogPotato, Log, TEXT("Unspawned potato %s at %s"), *GetName(), *GetActorLocation().ToString())
}

void APotato::Cheat_Scale(float scale)
{
	SetActorScale3D(FVector(scale, scale, scale));
}

const FNutritionalInformations& APotato::GetNutritionalInformations() const
{
	return _nutritionalInformations;
}

