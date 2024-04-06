#pragma once
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UHuntAttributeSet.generated.h"
 
// Helpers
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UHuntAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
 
public:
 
	// Current health of the owner
	UPROPERTY()
	FGameplayAttributeData Health = 100.0f;
	ATTRIBUTE_ACCESSORS(UHuntAttributeSet, Health)
 
	// Max health that the owner can have
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth= 100.0f;
	ATTRIBUTE_ACCESSORS(UHuntAttributeSet, MaxHealth)

	// Damage the owner can deal with one attack
	UPROPERTY()
	FGameplayAttributeData Damage = 90.0f;
	ATTRIBUTE_ACCESSORS(UHuntAttributeSet, Damage)
};