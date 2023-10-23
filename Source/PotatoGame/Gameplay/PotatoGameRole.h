#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"

#include "CoreMinimal.h"

#include "PotatoGameRole.generated.h"

UENUM()
enum class EGameRoleType
{
	Planter,
	Gatherer,
	Eater
};

USTRUCT()
struct POTATOGAME_API FPotatoGameRole
{
	GENERATED_BODY()
public:
	EGameRoleType GetRoleType() const { return _roleType; }

	TSubclassOf<APotatoBaseCharacter> GetCharacterType() const { return _characterType; }

	bool operator==(const FPotatoGameRole& other) const
	{
		return _roleType == other._roleType
			&& _characterType == other._characterType;
	}

	bool operator!=(const FPotatoGameRole& other) const
	{
		return !(operator==(other));
	}

private:
	UPROPERTY(EditAnywhere)
	EGameRoleType _roleType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APotatoBaseCharacter> _characterType;
};
