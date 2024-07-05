// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Hunt : ModuleRules
{
	public Hunt(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "MoviePlayer", "GameAnalytics", "AdvancedSessions", "GameplayAbilities"});

		PrivateDependencyModuleNames.AddRange(new string[] {"AIModule", "GameAnalytics"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		PrivateDependencyModuleNames.Add("OnlineSubsystem");
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
