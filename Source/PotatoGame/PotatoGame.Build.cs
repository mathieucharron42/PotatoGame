// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PotatoGame : ModuleRules
{
	public PotatoGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "Core", "CoreUObject", "Engine", "GameplayAbilities", "GameplayTags", "GameplayTasks", "InputCore", "HeadMountedDisplay", "NavigationSystem" });

        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
        }
    }
}
