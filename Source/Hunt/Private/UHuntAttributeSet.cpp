// Fill out your copyright notice in the Description page of Project Settings.


#include "UHuntAttributeSet.h"

void UHuntAttributeSet::SetMaxHealthFloat(float NewVal)
{
	NewVal = FMath::Max(NewVal, 0.0f);
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (ensure(ASC))
	{
		ASC->SetNumericAttributeBase(GetMaxHealthAttribute(), NewVal);
	}
}

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

void UHuntAttributeSet::SetDamageFloat(float NewVal)
{
	NewVal = FMath::Max(NewVal, 0.0f);
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (ensure(ASC))
	{
		ASC->SetNumericAttributeBase(GetDamageAttribute(), NewVal);
	}
}


