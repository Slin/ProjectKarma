// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectKarma.h"
#include "PKPlayerMovementComponent.h"

void UPKPlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Make sure that everything is still valid, and that we are allowed to move.
	if(!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}
	
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector(0.0f, 0.0f, 20.0f), UpdatedComponent->GetComponentRotation(), true, Hit);
	
	// Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector() * DeltaTime * 150.0f + FVector(bestX-UpdatedComponent->GetComponentLocation().X, 0.0f, 0.0f);
	
	if(!DesiredMovementThisFrame.IsNearlyZero())
	{
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
		
		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
	
	SafeMoveUpdatedComponent(FVector(0.0f, 0.0f, -20.0f), UpdatedComponent->GetComponentRotation(), true, Hit);
	
	_gravity -= 981.0f*DeltaTime;
	FVector DesiredGravityMovement(0.0f, 0.0f, _gravity*DeltaTime);
	
	SafeMoveUpdatedComponent(DesiredGravityMovement, UpdatedComponent->GetComponentRotation(), true, Hit);
	
	// If we bumped into something, try to slide along it
	if(Hit.IsValidBlockingHit())
	{
		if(_gravity < 0.0f)
		{
			_isGrounded = true;
		}
		else
		{
			_isGrounded = false;
		}
		
//		SlideAlongSurface(DesiredGravityMovement, 1.f - Hit.Time, Hit.Normal, Hit);
		_gravity = 0.0f;
	}
	else
	{
		_isGrounded = false;
	}
};

void UPKPlayerMovementComponent::Jump(float strength)
{
	if(_isGrounded)
		_gravity = strength;
}