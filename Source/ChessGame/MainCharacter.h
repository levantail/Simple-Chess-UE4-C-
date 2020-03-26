// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"


#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

#include "ChessGame/BaseHUD.h"
#include "ChessGame/ChessMaster.h"
#include "ChessGame/PlayerController_Mouse.h"

#include "MainCharacter.generated.h"

#define DEBUGMESSAGE(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, TEXT(x));}

UENUM()
enum class ECharacterMouseType : uint8
{
	DEFAULT,
	CAMERA_MOVEMENT,
	LOADING,
	HOVERING,
	SELECTION,
	DENIED
};

UCLASS()
class CHESSGAME_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseZoomRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseRotationRate;

protected:
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	void SetNewMouseCursor(ECharacterMouseType NewMouseCursor);
	ECharacterMouseType CurrentMouseCursor = ECharacterMouseType::DEFAULT;
	// Does Player trying to rotate camera right now ?
	bool bIsCameraRotatingActive = false;

	// Called when the game starts or when spawned
	FVector V_BottomLeft;
	FVector V_TopRight;

	EChessColor CurrentPlayerColor;

	float SpringArmLenghtMax;
	float SpringArmLenghtMin;
	float PitchCameraMax;
	float PitchCameraMin;
	void NextTurn();

	virtual void BeginPlay() override;

	virtual void Event_LMB();
	virtual void Event_CameraRotationStart();
	virtual void Event_CameraRotationEnd();
	virtual void Event_CameraZoom(float AxisValue);
	virtual void Event_CameraRotationX(float AxisValue);
	virtual void Event_CameraRotationY(float AxisValue);

	class ABaseHUD* PlayerHUD;
	virtual ABaseHUD* GetPlayerHUD();

	void TraceForChess(AChessMaster* TracedChess);
	void TraceForVFX(AChessPlaceToMove* TracedVFX);

	AChessMaster* CHTemp;
	AChessMaster* CHTickTemp;
	AChessPlaceToMove* VFXTickTemp;
	bool bTickTrigger2x = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void SetV_BottomLeft(FVector NewVector) { V_BottomLeft = NewVector; }
	void SetV_TopRight(FVector NewVector) { V_TopRight = NewVector; }
};
