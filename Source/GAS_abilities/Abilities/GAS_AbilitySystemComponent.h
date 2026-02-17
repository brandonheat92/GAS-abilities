// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAS_ABILITIES_API UGAS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void StoreActorInfo(TObjectPtr<ACharacter> AblitiyOwner);

private:
	TObjectPtr<ACharacter> AblitiyOwner;
};
