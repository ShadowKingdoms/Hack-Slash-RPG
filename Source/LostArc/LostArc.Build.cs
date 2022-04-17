// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LostArc : ModuleRules
{
	public LostArc(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "GameplayTasks", "Niagara", "UMG", "ProceduralMeshComponent", "Slate", "SlateCore" });
    }
}
