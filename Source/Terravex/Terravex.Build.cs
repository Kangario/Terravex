using UnrealBuildTool;
using System.IO;

public class Terravex : ModuleRules
{
	public Terravex(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"HTTP",
			"Json",
			"JsonUtilities",
			"UMG",
			"AndroidPermission", 
			"OnlineSubsystem",
			"WebSockets",
			"ProceduralMeshComponent"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"WebBrowser",
			"Slate",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"OnlineSubsystem",
			"OnlineSubsystemUtils"
		});
	
	}
}