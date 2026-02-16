// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_LeapToLocation.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbility.h"

UAT_LeapToLocation* UAT_LeapToLocation::ArcMoveToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector StartLocation, FVector TargetLocation, float Duration, float ArcHeight)
{
    UAT_LeapToLocation* MyObj = NewAbilityTask<UAT_LeapToLocation>(OwningAbility, TaskInstanceName);

    //init and get necessary parameters and return task object

    return MyObj;
}

void UAT_LeapToLocation::Activate()
{
    Super::Activate();

    bTickingTask = true;
}

void UAT_LeapToLocation::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    //todo linear interpolate to leap location
}

void UAT_LeapToLocation::OnDestroy(bool bInOwnerFinished)
{
    bTickingTask = false;
    Super::OnDestroy(bInOwnerFinished);
}
