// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "PKAnimationInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)
class UPKAnimationInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()
 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Anims) float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Anims) uint8 State;
};