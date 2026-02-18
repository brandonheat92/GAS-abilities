// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/PlayerController.h"
#include "GAS_abilitiesPlayerController.generated.h"

class UAbilitySystemComponent;
class UPlayerAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerAttributeSignature, FGameplayAttribute, AttrData);

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AGAS_abilitiesPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGAS_abilitiesPlayerController();

	virtual void OnPossess(APawn* pawn) override;
    virtual void OnUnPossess() override;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Attributes")
    FOnPlayerAttributeSignature OnPlayerAttributesChangedCB;

private:

    void InitAbilitySystemAndAttributeCallbacks();
    void BindASCDelegates(const FGameplayAttribute& Data);
    void OnPlayerAttributeChanged(const FOnAttributeChangeData& Data);

    UPROPERTY()
    UAbilitySystemComponent* AbilitySystem;

    UPROPERTY()
    const UPlayerAttributeSet* PlayerAttributeSet;
};


