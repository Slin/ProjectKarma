// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PKPlayer.generated.h"

UCLASS()
class PROJECTKARMA_API APKPlayer : public APawn
{
	GENERATED_BODY()

public:
	
	enum State
	{
		Spirit,
		Armadillo,
		Jaguar,
		Bison
	};
	
	// Sets default values for this pawn's properties
	APKPlayer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meshes, meta = (AllowPrivateAccess = "true")) USkeletalMeshComponent *SpiritMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meshes, meta = (AllowPrivateAccess = "true")) USkeletalMeshComponent *ArmadilloMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meshes, meta = (AllowPrivateAccess = "true")) USkeletalMeshComponent *PumaMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meshes, meta = (AllowPrivateAccess = "true")) USkeletalMeshComponent *BisonMeshComponent;
	

	
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	
	//Input functions
	void MoveRight(float AxisValue);
	void JumpPressed();
	
	void SpiritPressed();
	void ArmadilloPressed();
	void JaguarPressed();
	void BisonPressed();
	void SwitchToState(State stateParam);
	
	UPROPERTY(EditAnywhere) USpringArmComponent* OurCameraSpringArm;
	UCameraComponent* OurCamera;
	
	UBoxComponent* BoxComponent;
	class UPKPlayerMovementComponent *MovementComponent;
	
	float totalTime;
	State state;
	FVector currentCollisionBoxSize;
	float movementSpeed;
};
