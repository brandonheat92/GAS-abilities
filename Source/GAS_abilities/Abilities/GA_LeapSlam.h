// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_LeapSlam.generated.h"

/**
 * 
 */
UCLASS()
class GAS_ABILITIES_API UGA_LeapSlam : public UGameplayAbility
{
	GENERATED_BODY()
public:

    UGA_LeapSlam();

protected:

    UPROPERTY(EditDefaultsOnly, Category = "Leap")
    float MaxDistance = 800.f;

    UPROPERTY(EditDefaultsOnly, Category = "Leap")
    float BaseDuration = 0.8f;

    UPROPERTY(EditDefaultsOnly, Category = "Leap")
    float ArcHeight = 300.f;

    UPROPERTY(EditDefaultsOnly, Category = "Leap")
    float AoERadius = 300.f;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    
    UPROPERTY(EditDefaultsOnly, Category = "Leap")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

private:

    FVector CalculateLandingLocation();
    void ApplyAoEDamage(const FVector& Location);

    UFUNCTION()
    void OnArcFinished();
};
