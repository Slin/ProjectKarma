// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "ProjectKarmaCharacter.generated.h"

UCLASS(config=Game)
class AProjectKarmaCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) class USpringArmComponent* CameraBoom;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MesheSpirit, meta = (AllowPrivateAccess = "true")) class USkeletalMesh *SpiritMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MesheSpirit, meta = (AllowPrivateAccess = "true")) class UMaterial *SpiritMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MeshArmadillo, meta = (AllowPrivateAccess = "true")) class USkeletalMesh *ArmadilloMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MeshArmadillo, meta = (AllowPrivateAccess = "true")) class UMaterial *ArmadilloMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MeshJaguar, meta = (AllowPrivateAccess = "true")) class USkeletalMesh *JaguarMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MeshJaguar, meta = (AllowPrivateAccess = "true")) class UMaterial *JaguarMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MeshBison, meta = (AllowPrivateAccess = "true")) class USkeletalMesh *BisonMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MeshBison, meta = (AllowPrivateAccess = "true")) class UMaterial *BisonMaterial;

protected:

	enum State
	{
		Spirit,
		Armadillo,
		Jaguar,
		Bison
	};
	
	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	
	void SpiritPressed();
	void ArmadilloPressed();
	void JaguarPressed();
	void BisonPressed();
	void SwitchToState(State state);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
	
	virtual void Tick(float DeltaSeconds) override;
	
	float totalTime;
	
	State state;


public:
	AProjectKarmaCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
