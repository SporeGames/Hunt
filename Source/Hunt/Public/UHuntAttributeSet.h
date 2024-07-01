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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData Health = 100.0f;
	ATTRIBUTE_ACCESSORS(UHuntAttributeSet, Health)
	UFUNCTION(BlueprintCallable, Category = "HuntAttributeSet")
	void SetMaxHealthFloat(float newValue);

 
	// Max health that the owner can have
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData MaxHealth= 500.0f;
	ATTRIBUTE_ACCESSORS(UHuntAttributeSet, MaxHealth)
	UFUNCTION(BlueprintCallable, Category = "HuntAttributeSet")
	void SetHealthFloat(float newValue);

	// Damage the owner can deal with one attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData Damage = 90.0f;
	ATTRIBUTE_ACCESSORS(UHuntAttributeSet, Damage)
	UFUNCTION(BlueprintCallable, Category = "HuntAttributeSet")
	void SetDamageFloat(float newValue);

	// Movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData MovementSpeed = 600.0f;
	ATTRIBUTE_ACCESSORS(UHuntAttributeSet, MovementSpeed)
	
	// Mana
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData Mana = 100.0f;
	ATTRIBUTE_ACCESSORS(UHuntAttributeSet, Mana)
	UFUNCTION(BlueprintCallable, Category = "HuntAttributeSet")
	void SetManaFloat(float newValue);
};