// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_AbilitiesPlayerState.h"

AGAS_AbilitiesPlayerState::AGAS_AbilitiesPlayerState()
{
    GAS_AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>("ASC");

    //if needed for multiplayer
    /*AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);*/

    AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>("AttributeSet");

}

UGAS_AbilitySystemComponent* AGAS_AbilitiesPlayerState::GetAbilitySystemComponent() const
{
	return GAS_AbilitySystemComponent;
}

UPlayerAttributeSet* AGAS_AbilitiesPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
