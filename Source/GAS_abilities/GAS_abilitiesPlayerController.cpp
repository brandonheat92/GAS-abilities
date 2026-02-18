// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_abilitiesPlayerController.h"
#include "GameFramework/Pawn.h"			 
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GAS_abilitiesCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/AttributeSets/PlayerAttribute/PlayerAttributeSet.h"
#include "GAS_AbilitiesPlayerState.h"
#include "Engine/LocalPlayer.h"

AGAS_abilitiesPlayerController::AGAS_abilitiesPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AGAS_abilitiesPlayerController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

    InitAbilitySystemAndAttributeCallbacks();
}

void AGAS_abilitiesPlayerController::OnUnPossess()
{
    OnPlayerAttributesChangedCB.Clear();
    AbilitySystem->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetHealthAttribute()).Clear();
    AbilitySystem->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetManaAttribute()).Clear();
}

void AGAS_abilitiesPlayerController::InitAbilitySystemAndAttributeCallbacks()
{
    AbilitySystem = nullptr;
    PlayerAttributeSet = nullptr;

    AGAS_AbilitiesPlayerState* PS = GetPlayerState<AGAS_AbilitiesPlayerState>();
    if (!PS)
    {
        return;
    }

    AbilitySystem = PS->GetAbilitySystemComponent();
    if (!AbilitySystem)
    {
        return;
    }

    PlayerAttributeSet = PS->GetAttributeSet();
    if (!PlayerAttributeSet)
    {
        return;
    }

    BindASCDelegates(UPlayerAttributeSet::GetHealthAttribute());
    BindASCDelegates(UPlayerAttributeSet::GetManaAttribute());
}

void AGAS_abilitiesPlayerController::BindASCDelegates(const FGameplayAttribute& Data)
{
    AbilitySystem->GetGameplayAttributeValueChangeDelegate(Data).AddUObject(this, &AGAS_abilitiesPlayerController::OnPlayerAttributeChanged);    
}

void AGAS_abilitiesPlayerController::OnPlayerAttributeChanged(const FOnAttributeChangeData& Data)
{
    OnPlayerAttributesChangedCB.Broadcast(Data.Attribute);
}
