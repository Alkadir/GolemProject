// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GOLEMPROJECT_GolemProjectCharacter_generated_h
#error "GolemProjectCharacter.generated.h already included, missing '#pragma once' in GolemProjectCharacter.h"
#endif
#define GOLEMPROJECT_GolemProjectCharacter_generated_h

#define GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_RPC_WRAPPERS
#define GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS
#define GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAGolemProjectCharacter(); \
	friend struct Z_Construct_UClass_AGolemProjectCharacter_Statics; \
public: \
	DECLARE_CLASS(AGolemProjectCharacter, ACharacter, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GolemProject"), NO_API) \
	DECLARE_SERIALIZER(AGolemProjectCharacter)


#define GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_INCLASS \
private: \
	static void StaticRegisterNativesAGolemProjectCharacter(); \
	friend struct Z_Construct_UClass_AGolemProjectCharacter_Statics; \
public: \
	DECLARE_CLASS(AGolemProjectCharacter, ACharacter, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GolemProject"), NO_API) \
	DECLARE_SERIALIZER(AGolemProjectCharacter)


#define GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AGolemProjectCharacter(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AGolemProjectCharacter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AGolemProjectCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AGolemProjectCharacter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AGolemProjectCharacter(AGolemProjectCharacter&&); \
	NO_API AGolemProjectCharacter(const AGolemProjectCharacter&); \
public:


#define GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AGolemProjectCharacter(AGolemProjectCharacter&&); \
	NO_API AGolemProjectCharacter(const AGolemProjectCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AGolemProjectCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AGolemProjectCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AGolemProjectCharacter)


#define GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CameraBoom() { return STRUCT_OFFSET(AGolemProjectCharacter, CameraBoom); } \
	FORCEINLINE static uint32 __PPO__FollowCamera() { return STRUCT_OFFSET(AGolemProjectCharacter, FollowCamera); }


#define GolemProject_Source_GolemProject_GolemProjectCharacter_h_9_PROLOG
#define GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_RPC_WRAPPERS \
	GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_INCLASS \
	GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_INCLASS_NO_PURE_DECLS \
	GolemProject_Source_GolemProject_GolemProjectCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GOLEMPROJECT_API UClass* StaticClass<class AGolemProjectCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID GolemProject_Source_GolemProject_GolemProjectCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
