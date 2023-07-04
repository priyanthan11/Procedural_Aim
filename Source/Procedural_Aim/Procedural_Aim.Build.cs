// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Procedural_Aim : ModuleRules
{
	public Procedural_Aim(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
