// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ChroniclesOfTheForgottenTarget : TargetRules
{
	public ChroniclesOfTheForgottenTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		ExtraModuleNames.Add("ChroniclesOfTheForgotten");
	}
}
