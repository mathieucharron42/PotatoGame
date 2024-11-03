#include "PotatoGame.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, PotatoGame, "PotatoGame" );
 
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Behaviour_PotatoPickupCapabale, "Character.Behaviour.PotatoPickupCapabale", "Able to pick up potato");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Behaviour_PotatoPlantingCapabale, "Character.Behaviour.PotatoPlantingCapabale", "Able to plant potato");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Behaviour_PotatoHoldingCapabale, "Character.Behaviour.PotatoHoldingCapabale", "Able to hold potato");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Behaviour_PotatoEatingCapabale, "Character.Behaviour.PotatoEatingCapabale", "Able to eat potato");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Behaviour_State_HoldingPotato, "Character.Behaviour.State.HoldingPotato", "Whether is currently holding potato");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_Behaviour_PotatoRainable, "Cheat.Behaviour.PotatoRainable", "Able to rain potato");