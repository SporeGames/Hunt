// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "UHuntAttributeSet.h"
#include "GameFramework/Character.h"
#include "AHuntCharacter.generated.h"

UCLASS()
class HUNT_API AAHuntCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAHuntCharacter();
	UPROPERTY()
	UHuntAttributeSet* AttributeSet = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	

};
