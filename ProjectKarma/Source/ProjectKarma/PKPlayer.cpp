// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectKarma.h"
#include "PKPlayer.h"
#include "PKPlayerMovementComponent.h"
#include "PKAnimationInstance.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Animation/AnimBlueprint.h"
#include "Runtime/Engine/Classes/Animation/AnimBlueprintGeneratedClass.h"

// Sets default values
APKPlayer::APKPlayer() : totalTime(0.0f), movementSpeed(2.0f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Our root component will be a box that reacts to physics
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxComponent;
	BoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 100.0f));
	BoxComponent->SetCollisionProfileName(TEXT("Pawn"));
	
	SpiritMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SpiritRepresentation"));
	SpiritMeshComponent->AttachTo(RootComponent);
	SpiritMeshComponent->SetRelativeScale3D(FVector(0.5f));
	
	ArmadilloMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmadilloRepresentation"));
	ArmadilloMeshComponent->AttachTo(RootComponent);
	
	PumaMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PumaRepresentation"));
	PumaMeshComponent->AttachTo(RootComponent);
	
	BisonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BisonRepresentation"));
	BisonMeshComponent->AttachTo(RootComponent);
	
	//Create our components
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->AttachTo(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 500.f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 5.0f;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->AttachTo(OurCameraSpringArm, USpringArmComponent::SocketName);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	// Create an instance of our movement component, and tell it to update the root.
	MovementComponent = CreateDefaultSubobject<UPKPlayerMovementComponent>(TEXT("CustomMovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void APKPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	MovementComponent->bestX = GetActorLocation(GetRootComponent()).X;
	SwitchToState(State::Spirit);
}

// Called every frame
void APKPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	switch(state)
	{
		case State::Spirit:
		{
			totalTime += DeltaTime*4.0f;
			SpiritMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, cos(totalTime)*20.0f+0.0f));
			break;
		}
	}
}

UPawnMovementComponent* APKPlayer::GetMovementComponent() const
{
	return MovementComponent;
}

// Called to bind functionality to input
void APKPlayer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	
	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveRight", this, &APKPlayer::MoveRight);
	InputComponent->BindAction("Jump", IE_Pressed, this, &APKPlayer::JumpPressed);
	
	InputComponent->BindAction("Spirit", IE_Pressed, this, &APKPlayer::SpiritPressed);
	InputComponent->BindAction("Armadillo", IE_Pressed, this, &APKPlayer::ArmadilloPressed);
	InputComponent->BindAction("Jaguar", IE_Pressed, this, &APKPlayer::JaguarPressed);
	InputComponent->BindAction("Bison", IE_Pressed, this, &APKPlayer::BisonPressed);
}

void APKPlayer::MoveRight(float AxisValue)
{
	if(MovementComponent && (MovementComponent->UpdatedComponent == RootComponent))
	{
		MovementComponent->AddInputVector(GetActorRightVector() * -AxisValue * movementSpeed);
		
		UPKAnimationInstance *pumaAnimInstance = Cast<UPKAnimationInstance>(PumaMeshComponent->GetAnimInstance());
		
		UPKAnimationInstance *bisonAnimInstance = Cast<UPKAnimationInstance>(BisonMeshComponent->GetAnimInstance());
		
		UPKAnimationInstance *armadilloAnimInstance = Cast<UPKAnimationInstance>(ArmadilloMeshComponent->GetAnimInstance());
		
		if(fabsf(AxisValue) > 0.1f)
		{
			float angle = (AxisValue < 0.0f)?0.0f:PI;
			ArmadilloMeshComponent->SetWorldRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), angle));
			PumaMeshComponent->SetWorldRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), angle));
			BisonMeshComponent->SetWorldRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), angle));
			
			pumaAnimInstance->Speed = fabsf(AxisValue)*movementSpeed;
			pumaAnimInstance->State = 1;
			
			bisonAnimInstance->Speed = fabsf(AxisValue)*movementSpeed*0.5f;
			bisonAnimInstance->State = 1;
			
			armadilloAnimInstance->Speed = fabsf(AxisValue)*movementSpeed*0.2f;
			armadilloAnimInstance->State = 1;
		}
		else
		{
			pumaAnimInstance->State = 0;
			bisonAnimInstance->State = 0;
			armadilloAnimInstance->State = 0;
		}
	}
}

void APKPlayer::JumpPressed()
{
	switch(state)
	{
		case State::Spirit:
			break;
			MovementComponent->Jump(600.0f);
			break;
		case State::Armadillo:
			MovementComponent->Jump(500.0f);
			break;
		case State::Jaguar:
			MovementComponent->Jump(750.0f);
			break;
		case State::Bison:
			break;
	}
}

void APKPlayer::SpiritPressed()
{
	SwitchToState(State::Spirit);
}

void APKPlayer::ArmadilloPressed()
{
	SwitchToState(State::Armadillo);
}

void APKPlayer::JaguarPressed()
{
	SwitchToState(State::Jaguar);
}

void APKPlayer::BisonPressed()
{
	SwitchToState(State::Bison);
}

void APKPlayer::SwitchToState(State stateParam)
{
	state = stateParam;
	
	FVector boxSize;
	
	SpiritMeshComponent->SetVisibility(stateParam == State::Spirit);
	ArmadilloMeshComponent->SetVisibility(stateParam == State::Armadillo);
	PumaMeshComponent->SetVisibility(stateParam == State::Jaguar);
	BisonMeshComponent->SetVisibility(stateParam == State::Bison);
	
	switch(stateParam)
	{
		case State::Spirit:
		{
			boxSize = FVector(50.0f, 50.0f, 100.0f);
			movementSpeed = 1.5f;
			break;
		}
		case State::Armadillo:
		{
//			boxSize = FVector(40.0f, 20.0f, 13.5f);
			boxSize = FVector(15.0f, 15.0f, 13.5f);
			ArmadilloMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -boxSize.Z));
			movementSpeed = 2.0f;
			break;
		}
		case State::Jaguar:
		{
			boxSize = FVector(85.0f, 50.0f, 35.0f);
			PumaMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -boxSize.Z));
			movementSpeed = 4.0f;
			break;
		}
		case State::Bison:
		{
			boxSize = FVector(125, 50.0f, 75.0f);
			BisonMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -boxSize.Z));
			movementSpeed = 2.0f;
			break;
		}
	}
	
	float heightDiff = boxSize.Z - currentCollisionBoxSize.Z;
	if(heightDiff > 0)
		GetRootComponent()->AddWorldOffset(FVector(0.0f, 0.0f, heightDiff));
	
	BoxComponent->SetBoxExtent(boxSize);
	currentCollisionBoxSize = boxSize;
}



