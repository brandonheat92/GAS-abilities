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

    AActor* Avatar = GetAvatarActor();
    if (!Avatar)
    {
        EndTask();
        return;
    }
    AvatarActor = Ability->GetAvatarActorFromActorInfo();

    // Create spline
    MovementSpline = NewObject<USplineComponent>(Avatar);
    MovementSpline->RegisterComponent();
    MovementSpline->SetMobility(EComponentMobility::Movable);

    // Clear any old points
    MovementSpline->ClearSplinePoints();

    // Start
    MovementSpline->AddSplinePoint(Start, ESplineCoordinateSpace::World);

    // Mid point (arc)
    FVector MidPoint = (Start + End) * 0.5f;
    MidPoint.Z += Height;

    MovementSpline->AddSplinePoint(MidPoint, ESplineCoordinateSpace::World);

    // End
    MovementSpline->AddSplinePoint(End, ESplineCoordinateSpace::World);

    MovementSpline->SetSplinePointType(0, ESplinePointType::Curve);
    MovementSpline->SetSplinePointType(1, ESplinePointType::Curve);
    MovementSpline->SetSplinePointType(2, ESplinePointType::Curve);

    MovementSpline->UpdateSpline();

    TotalDistance = MovementSpline->GetSplineLength();
    ElapsedTime = 0.f;

    bTickingTask = true;
}

void UAT_LeapToLocation::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    if (!MovementSpline || !AvatarActor.IsValid())
    {
        EndTask();
        return;
    }

    ElapsedTime += DeltaTime;

    float Alpha = FMath::Clamp(ElapsedTime / TotalDuration, 0.f, 1.f);

    float Distance = Alpha * TotalDistance;
    UE_LOG(LogTemp, Warning, TEXT("Elapsed: %f / %f"), ElapsedTime, TotalDuration);

    FVector NewLocation =
        MovementSpline->GetLocationAtDistanceAlongSpline(
            Distance,
            ESplineCoordinateSpace::World
        );

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
