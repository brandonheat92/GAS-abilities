// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_LeapToLocation.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbility.h"

UAT_LeapToLocation* UAT_LeapToLocation::ArcMoveToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector StartLocation, FVector TargetLocation, float Duration, float ArcHeight)
{
    UAT_LeapToLocation* MyObj = NewAbilityTask<UAT_LeapToLocation>(OwningAbility, TaskInstanceName);

    MyObj->Start = StartLocation;
    MyObj->End = TargetLocation;
    MyObj->TotalDuration = FMath::Max(Duration, 0.01f);
    MyObj->Height = ArcHeight;
    MyObj->ElapsedTime = 0.f;

    return MyObj;
}

void UAT_LeapToLocation::Activate()
{
    Super::Activate();

    if (!Ability)
    {
        EndTask();
        return;
    }

    AvatarActor = Ability->GetAvatarActorFromActorInfo();

    if (!AvatarActor.IsValid())
    {
        EndTask();
        return;
    }

    bTickingTask = true;
}

void UAT_LeapToLocation::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    if (!AvatarActor.IsValid())
    {
        EndTask();
        return;
    }

    ElapsedTime += DeltaTime;

    float Alpha = FMath::Clamp(ElapsedTime / TotalDuration, 0.f, 1.f);

    FVector NewLocation = FMath::Lerp(Start, End, Alpha);

    float ZOffset = 4.f * Height * Alpha * (1.f - Alpha);
    NewLocation.Z += ZOffset;

    AvatarActor->SetActorLocation(NewLocation, false);

    if (Alpha >= 1.f)
    {
        OnFinished.Broadcast();
        EndTask();
    }
}

void UAT_LeapToLocation::OnDestroy(bool bInOwnerFinished)
{
    bTickingTask = false;
    Super::OnDestroy(bInOwnerFinished);
}
