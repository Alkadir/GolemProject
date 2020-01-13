// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GolemProject/GolemProjectGameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGolemProjectGameMode() {}
// Cross Module References
	GOLEMPROJECT_API UClass* Z_Construct_UClass_AGolemProjectGameMode_NoRegister();
	GOLEMPROJECT_API UClass* Z_Construct_UClass_AGolemProjectGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_GolemProject();
// End Cross Module References
	void AGolemProjectGameMode::StaticRegisterNativesAGolemProjectGameMode()
	{
	}
	UClass* Z_Construct_UClass_AGolemProjectGameMode_NoRegister()
	{
		return AGolemProjectGameMode::StaticClass();
	}
	struct Z_Construct_UClass_AGolemProjectGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AGolemProjectGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_GolemProject,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGolemProjectGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "GolemProjectGameMode.h" },
		{ "ModuleRelativePath", "GolemProjectGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AGolemProjectGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AGolemProjectGameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AGolemProjectGameMode_Statics::ClassParams = {
		&AGolemProjectGameMode::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802A8u,
		METADATA_PARAMS(Z_Construct_UClass_AGolemProjectGameMode_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_AGolemProjectGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AGolemProjectGameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AGolemProjectGameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AGolemProjectGameMode, 2513500505);
	template<> GOLEMPROJECT_API UClass* StaticClass<AGolemProjectGameMode>()
	{
		return AGolemProjectGameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AGolemProjectGameMode(Z_Construct_UClass_AGolemProjectGameMode, &AGolemProjectGameMode::StaticClass, TEXT("/Script/GolemProject"), TEXT("AGolemProjectGameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AGolemProjectGameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
