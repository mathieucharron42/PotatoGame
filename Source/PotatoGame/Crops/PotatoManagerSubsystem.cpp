#include "PotatoManagerSubsystem.h"

#include "PotatoGame/Utils/PotatoUtilities.h"

void UPotatoManagerSubsystem::RegisterPotato(APotato* potato)
{
	_potatoes.Add(potato);
	PotatoUtilities::DoSomethingElse(5*1024*1024);
}

void UPotatoManagerSubsystem::UnregisterPotato(APotato* potato)
{
	_potatoes.Remove(potato);
}

const TArray<APotato*> UPotatoManagerSubsystem::GetPotatoes() const
{
	return _potatoes;
}
