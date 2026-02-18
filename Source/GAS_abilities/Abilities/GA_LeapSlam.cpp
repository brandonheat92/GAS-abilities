// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_LeapSlam.h"
#include "Task/AT_LeapToLocation.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "NavigationSystem.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/OverlapResult.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AttributeSets/PlayerAttribute/PlayerAttributeSet.h"

UGA_LeapSlam::UGA_LeapSlam()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_LeapSlam::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    AActor* Avatar = ActorInfo->AvatarActor.Get();
    if (!Avatar)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    FVector StartLocation = Avatar->GetActorLocation();
    FVector TargetLocation = CalculateLandingLocation();

    const UPlayerAttributeSet* AttributeSet = ActorInfo->AbilitySystemComponent->GetSet<UPlayerAttributeSet>();

    float AttackSpeed = 1.f;
    if (AttributeSet)
    {
        AttackSpeed = FMath::Max(AttributeSet->GetAttackSpeed(), 0.1f);
    }

    float Duration = BaseDuration / AttackSpeed;

    UAT_LeapToLocation* ArcTask = UAT_LeapToLocation::ArcMoveToLocation(this, FName("LeapArcMove"), StartLocation, TargetLocation, Duration, ArcHeight);

    ArcTask->OnFinished.AddDynamic(this, &UGA_LeapSlam::OnArcFinished);
    ArcTask->ReadyForActivation();
}

FVector UGA_LeapSlam::CalculateLandingLocation()
{
    AActor* Avatar = GetAvatarActorFromActorInfo();
    APlayerController* PC = GetActorInfo().PlayerController.Get();

    if (!PC)
    {
        return Avatar->GetActorLocation();
    }

    FHitResult CursorHit;
    PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

    FVector Start = Avatar->GetActorLocation();
    FVector Direction = (CursorHit.Location - Start).GetSafeNormal();

    float Distance = FVector::Distance(Start, CursorHit.Location);

    float ClampedDistance = FMath::Min(Distance, MaxDistance);

    FVector Desired = Start + Direction * ClampedDistance;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

    if (!NavSys)
    {
        return Start;
    }

    FNavLocation Projected;

    if (NavSys->ProjectPointToNavigation(Desired, Projected))
    {
        return Projected.Location;
    }

    for (float Step = ClampedDistance; Step > 0.f; Step -= 50.f)
    {
        FVector Test = Start + Direction * Step;

        if (NavSys->ProjectPointToNavigation(Test, Projected))
        {
            return Projected.Location;
        }
    }

    return Start;
}

void UGA_LeapSlam::OnArcFinished()
{
    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (!Avatar)
    {
        return;
    }

    if (LandingCameraShake)
    {
        APlayerController* PC = GetActorInfo().PlayerController.Get();
        if (PC)
        {
            PC->ClientStartCameraShake(LandingCameraShake);
        }
    }

    if (SlamMontage)
    {
        UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SlamMontage, 1.f);

        MontageTask->ReadyForActivation();
    }
    
    //moved to Cue
    //if (LandingVFX)
    //{
    //    FVector SpawnLocation = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();
    //    FRotator SpawnRotation = FRotator::ZeroRotator;

    //    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LandingVFX.Get(), SpawnLocation, SpawnRotation, FVector::OneVector,
    //        true,   // auto destroy
    //        true, ENCPoolMethod::AutoRelease, true);
    //}

    FGameplayCueParameters CueParams;
    CueParams.Location = Avatar->GetActorLocation();
    CueParams.Instigator = Avatar;

    GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(LandingVFXCueTag, CueParams);

    ApplyAoEDamage(Avatar->GetActorLocation());

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_LeapSlam::ApplyAoEDamage(const FVector& Location)
{
    if (!DamageEffectClass)
        return;

    TArray<FOverlapResult> Overlaps;

    GetWorld()->OverlapMultiByChannel(
        Overlaps,
        Location,
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(AoERadius));

    for (const FOverlapResult& Result : Overlaps)
    {
        AActor* TargetActor = Result.GetActor();
        if (!TargetActor)
            continue;

        UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);

        if (!TargetASC)
            continue;

        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

        if (SpecHandle.IsValid())
        {
            TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}

void UGA_LeapSlam::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}