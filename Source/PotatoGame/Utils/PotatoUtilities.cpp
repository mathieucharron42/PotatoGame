#include "PotatoGame/Utils/PotatoUtilities.h"

#include "UObject/NoExportTypes.h"

DEFINE_LOG_CATEGORY(LogGlobal);
DEFINE_LOG_CATEGORY(LogPotato);

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