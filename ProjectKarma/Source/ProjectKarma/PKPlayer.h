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
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshSpirit, meta = (AllowPrivateAccess = "true")) class USkeletalMesh *SpiritMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MesheSpirit, meta = (AllowPrivateAccess = "true")) class UMaterial *SpiritMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshArmadillo, meta = (AllowPrivateAccess = "true")) class USkeletalMesh *ArmadilloMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshArmadillo, meta = (AllowPrivateAccess = "true")) class UMaterial *ArmadilloMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshJaguar, meta = (AllowPrivateAccess = "true")) class USkeletalMesh *JaguarMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshJaguar, meta = (AllowPrivateAccess = "true")) class UMaterial *JaguarMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshBison, meta = (AllowPrivateAccess = "true")) class USkeletalMesh *BisonMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshBison, meta = (AllowPrivateAccess = "true")) class UMaterial *BisonMaterial;
	
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anims) UBlendSpace *BlendSpace;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anims) UAnimSequence *AnimSequence;

	
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	
	//Input functions
	void MoveRight(float AxisValue);
	
	void SpiritPressed();
	void ArmadilloPressed();
	void JaguarPressed();
	void BisonPressed();
	void SwitchToState(State stateParam);
	
	UPROPERTY(EditAnywhere) USpringArmComponent* OurCameraSpringArm;
	UCameraComponent* OurCamera;
	
	UBoxComponent* BoxComponent;
	USkeletalMeshComponent *MeshComponent;
	class UPKPlayerMovementComponent *MovementComponent;
	
	float totalTime;
	State state;
};
