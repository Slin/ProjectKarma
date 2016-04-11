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
	
	FHitResult movementHit;
	SafeMoveUpdatedComponent(FVector(0.0f, 0.0f, 20.0f), UpdatedComponent->GetComponentRotation(), true, movementHit);
	
	// Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector() * DeltaTime * 150.0f + FVector(bestX-UpdatedComponent->GetComponentLocation().X, 0.0f, 0.0f);
	
	if(!DesiredMovementThisFrame.IsNearlyZero())
	{
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, movementHit);
		
		// If we bumped into something, try to slide along it
		if (movementHit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - movementHit.Time, movementHit.Normal, movementHit);
		}
	}

	const FVector StartTrace = UpdatedComponent->GetComponentLocation();
	const FVector EndTrace = StartTrace + FVector(0.0f, 0.0f, _isGrounded ? -100.0f : -22.0f);

	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(FName(TEXT("IsGroundedTrace")), true, PawnOwner);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	_gravity -= 1100.0f*DeltaTime;

	FHitResult traceHit;
	float downCorrection = 20.0f;
	if(GetWorld()->LineTraceSingleByChannel(traceHit, StartTrace, EndTrace, ECC_GameTraceChannel1, TraceParams) && _gravity < 0.0f && _isGrounded)
	{
		downCorrection = traceHit.Distance;
	}

	SafeMoveUpdatedComponent(FVector(0.0f, 0.0f, -downCorrection+_gravity*DeltaTime), UpdatedComponent->GetComponentRotation(), true, movementHit);
	
	// If we bumped into something, try to slide along it
	if(traceHit.IsValidBlockingHit() || movementHit.IsValidBlockingHit())
	{
		if(_gravity < 0.0f)
		{
			_isGrounded = true;
			_gravity = 0.0f;
		}
		else
		{
			_isGrounded = false;
		}
	}
	else
	{
		_isGrounded = false;
	}
	
/*	if(_isGrounded)
	{
		FVector normal = traceHit.Normal;

//		normal.X = 1.0f;
		float tempX = normal.X;
		normal.X = 0.5f;// normal.Z;
		normal.Z = 0.5f;// -tempX;
		normal.Y = 0.0f;
//		normal.Z = 0.0f;
		normal.Normalize();
		FRotator rotation = normal.Rotation();
		FRotator compRotation = UpdatedComponent->GetComponentRotation();
		UpdatedComponent->SetWorldRotation(rotation);
	}*/
};

void UPKPlayerMovementComponent::Jump(float strength)
{
	if(_isGrounded)
	{
		_gravity = strength;
	}
}