
#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"

#pragma once

namespace PotatoUtilities
{
	void DoSomething(float value);
	void DoSomethingElse(int32 value);

	bool HasAuthority(const UActorComponent* actorComponent);
	bool HasAuthority(const AActor* actor);

	template<class ComponentType>
	const ComponentType* GetComponentByClass(const AActor* actor)
	{
		return GetComponentByClass<ComponentType>(const_cast<AActor*>(actor));
	}

	template<class ComponentType>
	ComponentType* GetComponentByClass(AActor* actor)
	{
		ComponentType* component = nullptr;
		if (ensure(IsValid(actor)))
		{
			component = actor->FindComponentByClass<ComponentType>();
		}
		return component;
	}

	template<class ComponentType>
	ComponentType* GetComponentByClass(UActorComponent* actorComponent)
	{
		ComponentType* component = nullptr;
		if (ensure(IsValid(actorComponent)))
		{
			component = GetComponentByClass<ComponentType>(actorComponent->GetOwner());
		}
		return component;
	}

	template<class ComponentType>
	const ComponentType* GetComponentByClass(const UActorComponent* actorComponent)
	{
		return GetComponentByClass<ComponentType>(const_cast<UActorComponent*>(actorComponent));
	}
};
