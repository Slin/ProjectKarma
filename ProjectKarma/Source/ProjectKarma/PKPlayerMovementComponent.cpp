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
	
	// Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
	
	if(!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
		
		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
	
	_gravity -= 981.0f*DeltaTime;
	FVector DesiredGravityMovement(0.0f, 0.0f, _gravity*DeltaTime);
	
	FHitResult GravityHit;
	SafeMoveUpdatedComponent(DesiredGravityMovement, UpdatedComponent->GetComponentRotation(), true, GravityHit);
	
	// If we bumped into something, try to slide along it
	if (GravityHit.IsValidBlockingHit())
	{
		SlideAlongSurface(DesiredGravityMovement, 1.f - GravityHit.Time, GravityHit.Normal, GravityHit);
		_gravity = 0.0f;
	}
};