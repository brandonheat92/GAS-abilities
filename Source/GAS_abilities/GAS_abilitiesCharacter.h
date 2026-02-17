// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Abilities/GAS_AbilitySystemComponent.h"
#include "GAS_abilitiesCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(Blueprintable)
class AGAS_abilitiesCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGAS_abilitiesCharacter();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MovementAction;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

protected:

	void MovementInput(const FInputActionValue& Value);
	void InitializeAttributes();
	void GiveAbilities();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UGAS_AbilitySystemComponent* AbilitySystem;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

};

