// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectKarma.h"
#include "PKPlayer.h"
#include "PKPlayerMovementComponent.h"


// Sets default values
APKPlayer::APKPlayer() : totalTime(0.0f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Our root component will be a box that reacts to physics
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxComponent;
	BoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 100.0f));
	BoxComponent->SetCollisionProfileName(TEXT("Pawn"));
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualRepresentation"));
	MeshComponent->AttachTo(RootComponent);
	
	//Create our components
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->AttachTo(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 500.f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->AttachTo(OurCameraSpringArm, USpringArmComponent::SocketName);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	// Create an instance of our movement component, and tell it to update the root.
	MovementComponent = CreateDefaultSubobject<UPKPlayerMovementComponent>(TEXT("CustomMovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
	
	
	// Configure character movement
/*	MovementComponent->bOrientRotationToMovement = true; // Face in the direction we are moving..
	MovementComponent->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	MovementComponent->GravityScale = 2.f;
	MovementComponent->AirControl = 0.80f;
	MovementComponent->JumpZVelocity = 1000.f;
	MovementComponent->GroundFriction = 3.f;
	MovementComponent->MaxWalkSpeed = 600.f;
	MovementComponent->MaxFlySpeed = 600.f;*/
}

// Called when the game starts or when spawned
void APKPlayer::BeginPlay()
{
	Super::BeginPlay();
	
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
			MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, cos(totalTime)*20.0f+70.0f));
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
	
	InputComponent->BindAction("Spirit", IE_Pressed, this, &APKPlayer::SpiritPressed);
	InputComponent->BindAction("Armadillo", IE_Pressed, this, &APKPlayer::ArmadilloPressed);
	InputComponent->BindAction("Jaguar", IE_Pressed, this, &APKPlayer::JaguarPressed);
	InputComponent->BindAction("Bison", IE_Pressed, this, &APKPlayer::BisonPressed);
}

void APKPlayer::MoveRight(float AxisValue)
{
	if(MovementComponent && (MovementComponent->UpdatedComponent == RootComponent))
	{
		MovementComponent->AddInputVector(GetActorRightVector() * -AxisValue);
		
		if(state != State::Spirit && fabsf(AxisValue) > 0.1f)
		{
			MeshComponent->SetWorldRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), (AxisValue < 0.0f)?0.0f:M_PI));
		}
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
	
	MeshComponent->SetWorldScale3D(FVector(1.0f));
	FVector boxSize;
	
	switch(stateParam)
	{
		case State::Spirit:
		{
			MeshComponent->SetSkeletalMesh(SpiritMesh);
			MeshComponent->SetMaterial(0, SpiritMaterial);
			MeshComponent->SetRelativeScale3D(FVector(0.5f));
			boxSize = FVector(50.0f, 50.0f, 100.0f);
			break;
		}
		case State::Armadillo:
		{
			MeshComponent->SetSkeletalMesh(ArmadilloMesh);
			MeshComponent->SetMaterial(0, ArmadilloMaterial);
			boxSize = FVector(40.0f, 20.0f, 13.5f);
			break;
		}
		case State::Jaguar:
		{
			MeshComponent->SetSkeletalMesh(JaguarMesh);
			MeshComponent->SetMaterial(0, JaguarMaterial);
			boxSize = FVector(85.0f, 28.0f, 37.0f);
			break;
		}
		case State::Bison:
		{
			MeshComponent->SetSkeletalMesh(BisonMesh);
			MeshComponent->SetMaterial(0, BisonMaterial);
			boxSize = FVector(125, 50.0f, 75.0f);
			break;
		}
	}
	
	BoxComponent->SetBoxExtent(boxSize);
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -boxSize.Z));
}



