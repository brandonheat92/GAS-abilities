// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_LeapSlam.h"
#include "Task/AT_LeapToLocation.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "NavigationSystem.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/OverlapResult.h"
#include "AttributeSets/PlayerAttribute/PlayerAttributeSet.h"

UGA_LeapSlam::UGA_LeapSlam()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_LeapSlam::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    //check ability commit status

    //null checks

    //gather location and direction for leap from CalculateLandingLocation()

    // init leap ability task and set necessary parameters
    //bound leap task finish callback and activate
}

FVector UGA_LeapSlam::CalculateLandingLocation()
{

    //Perform landing calculation, from mouse cursor position, 
    //also to make sure player is not landing into the gap or empty space falling below

    return FVector::ZeroVector;
}

void UGA_LeapSlam::OnArcFinished()
{
    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (!Avatar)
    {
        return;
    }

    ApplyAoEDamage(Avatar->GetActorLocation());

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_LeapSlam::ApplyAoEDamage(const FVector& Location)
{
    //spherical cast to obtain enemies in the slam location
    TArray<FOverlapResult> Overlaps;

    GetWorld()->OverlapMultiByChannel(
        Overlaps,
        Location,
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(AoERadius));

    for (const FOverlapResult& Result : Overlaps)
    {
        //apply stun to enemies
    }
}

void UGA_LeapSlam::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(
        Handle,
        ActorInfo,
        ActivationInfo,
        bReplicateEndAbility,
        bWasCancelled);
}

