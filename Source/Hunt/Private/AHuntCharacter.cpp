// Fill out your copyright notice in the Description page of Project Settings.


#include "AHuntCharacter.h"

// Sets default values
AAHuntCharacter::AAHuntCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttributeSet = CreateDefaultSubobject<UHuntAttributeSet>(TEXT("AttributeSet")); // Add our AttributeSet to the character

}

// Called when the game starts or when spawned
void AAHuntCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAHuntCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAHuntCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Fix for abilities not being able to find the owner actor, crashing the game
void AAHuntCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UAbilitySystemComponent* ASC = FindComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
		ASC->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	}
}

void AAHuntCharacter::UnPossessed()
{	
	Super::UnPossessed();
	FindComponentByClass<UAbilitySystemComponent>()->RefreshAbilityActorInfo();
}