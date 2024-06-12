// Fill out your copyright notice in the Description page of Project Settings.


#include "UHuntAttributeSet.h"

void UHuntAttributeSet::SetHealthFloat(float NewVal)
{
	NewVal = FMath::Max(NewVal, 0.0f);
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (ensure(ASC))
	{
		ASC->SetNumericAttributeBase(GetHealthAttribute(), NewVal);
	}
}

void UHuntAttributeSet::SetManaFloat(float NewVal)
{
	NewVal = FMath::Max(NewVal, 0.0f);
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (ensure(ASC))
	{
		ASC->SetNumericAttributeBase(GetManaAttribute(), NewVal);
	}
}

