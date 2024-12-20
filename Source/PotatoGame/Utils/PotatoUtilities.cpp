#include "PotatoUtilities.h"

#include "PotatoGame/PotatoLogs.h"

#include "UObject/NoExportTypes.h"

static TAutoConsoleVariable<int32> CVarWasteTime(
	TEXT("potato.WasteTime"),
	0,
	TEXT("Whether program will purposely waste time"),
	ECVF_Cheat);

static TAutoConsoleVariable<int32> CVarWasteMemory(
	TEXT("potato.WasteMemory"),
	0,
	TEXT("Whether program will purposely waste memory"),
	ECVF_Cheat);

void PotatoUtilities::DoSomething(float seconds)
{
	if (CVarWasteTime.GetValueOnGameThread())
	{
		UE_LOG(LogGlobal, Log, TEXT("Wasting %.4fs seconds"), seconds);
		const FDateTime start = FDateTime::Now();
		FTimespan span(0);
		while (span.GetTotalSeconds() <= seconds)
		{
			span = FDateTime::Now() - start;
		}
		UE_LOG(LogGlobal, Log, TEXT("Wasted %.4fs seconds"), span.GetTotalSeconds());
	}
}

void PotatoUtilities::DoSomethingElse(int32 value)
{
	if (CVarWasteMemory.GetValueOnGameThread())
	{
		static TArray<TArray<uint8>> leak;

		UE_LOG(LogGlobal, Log, TEXT("Wasting %d bytes"), value);
		TArray<uint8>& wastedMemory = leak.AddDefaulted_GetRef();

		wastedMemory.SetNumUninitialized(value);
	}
}

bool PotatoUtilities::HasAuthority(const UActorComponent* actorComponent)
{
	bool hasAuthority = false;
	if (ensure(IsValid(actorComponent)))
	{
		hasAuthority = HasAuthority(actorComponent->GetOwner());
	}
	return hasAuthority;
}

bool PotatoUtilities::HasAuthority(const AActor* actor)
{
	bool hasAuthority = false;
	if (ensure(IsValid(actor)))
	{
		hasAuthority = actor->HasAuthority();
	}
	return hasAuthority;
}