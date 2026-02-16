// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS_AbilitySystemComponent.h"

void UGAS_AbilitySystemComponent::StoreActorInfo(TObjectPtr<ACharacter> ThisAblitiyOwner)
{
	//store any custom struct or details related to character on Init in component itself
	AblitiyOwner = ThisAblitiyOwner;
}
