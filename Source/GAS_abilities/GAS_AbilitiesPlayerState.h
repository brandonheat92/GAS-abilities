// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Abilities/AttributeSets/PlayerAttribute/PlayerAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GAS_AbilitySystemComponent.h"
#include "GAS_AbilitiesPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GAS_ABILITIES_API AGAS_AbilitiesPlayerState : public APlayerState, 
                                            public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
    AGAS_AbilitiesPlayerState();
	
    virtual UGAS_AbilitySystemComponent* GetAbilitySystemComponent() const override;

    UPlayerAttributeSet* GetAttributeSet() const;

protected:

    UPROPERTY()
    TObjectPtr<UGAS_AbilitySystemComponent> GAS_AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UPlayerAttributeSet> AttributeSet;


};
