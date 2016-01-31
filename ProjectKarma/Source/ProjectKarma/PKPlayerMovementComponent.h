// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "PKPlayerMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class PROJECTKARMA_API UPKPlayerMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	void Jump(float strength);
	
private:
	float _gravity;
	bool _isGrounded;
};