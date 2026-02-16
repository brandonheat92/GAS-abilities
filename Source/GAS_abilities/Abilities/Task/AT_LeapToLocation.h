// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_LeapToLocation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArcMoveFinished);

UCLASS()
class GAS_ABILITIES_API UAT_LeapToLocation : public UAbilityTask
{
	GENERATED_BODY()
public:

    UPROPERTY(BlueprintAssignable)
    FArcMoveFinished OnFinished;        //callback to broadcast leap task finish

    static UAT_LeapToLocation* ArcMoveToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector StartLocation, FVector TargetLocation, float Duration, float ArcHeight);

    virtual void Activate() override;
    virtual void TickTask(float DeltaTime) override;
    virtual void OnDestroy(bool bInOwnerFinished) override;

protected:

    FVector Start;
    FVector End;

    float TotalDuration;
    float Height;

    float ElapsedTime;

    TWeakObjectPtr<AActor> AvatarActor;

};
