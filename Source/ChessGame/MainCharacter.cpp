// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(30.0f, 30.f);
	//GetCapsuleComponent()->SetHiddenInGame(false);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseZoomRate = 15.f;
	BaseRotationRate = 1.f;
	
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bDoCollisionTest = false;

	SpringArmLenghtMax = 450.f;
	SpringArmLenghtMin = 100.f;
	PitchCameraMax = -70.f;
	PitchCameraMin = 0.f;

	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 0.0f;
	
}

void AMainCharacter::SetNewMouseCursor(ECharacterMouseType NewMouseCursor)
{
	APlayerController_Mouse* PController = Cast<APlayerController_Mouse>(GetController());
	if (PController == nullptr) {
		return;
	}

	switch (NewMouseCursor)
	{
	case ECharacterMouseType::DEFAULT:
		PController->SetDefaultCursor();
		break;
	case ECharacterMouseType::CAMERA_MOVEMENT:
		PController->SetCardinalCrossCursor();
		break;
	case ECharacterMouseType::LOADING:
		PController->SetLoadingCursor();
		break;
	case ECharacterMouseType::HOVERING:
		PController->SetHandCursor();
		break;
	case ECharacterMouseType::SELECTION:
		PController->SetSelectCursor();
		break;
	case ECharacterMouseType::DENIED:
		PController->SetSlashedCircleCursor();
		break;
	}

	CurrentMouseCursor = NewMouseCursor;
}

void AMainCharacter::NextTurn()
{
	if (CurrentPlayerColor == EChessColor::CE_DARK) {
		CurrentPlayerColor = EChessColor::CE_WHITE;
		//UE_LOG(LogTemp, Warning, TEXT("WHITE Chesses Turn"));
	}
	else if (CurrentPlayerColor == EChessColor::CE_WHITE)
	{
		CurrentPlayerColor = EChessColor::CE_DARK;
		//UE_LOG(LogTemp, Warning, TEXT("DARK Chesses Turn"));
	}

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTickTrigger2x && bIsCameraRotatingActive != true) {//TODO check
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		FHitResult HitResult(EForceInit::ForceInit);
		if (PlayerController != nullptr) {
			PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult);

			if (HitResult.GetActor() != nullptr) {
				AChessMaster* CHMaster = Cast<AChessMaster>(HitResult.GetActor());
				AChessPlaceToMove* VFX = Cast<AChessPlaceToMove>(HitResult.GetActor());
				
				TraceForChess(CHMaster);
				TraceForVFX(VFX);
			}
			else
			{
				SetNewMouseCursor(ECharacterMouseType::DEFAULT);
			}
		}
	}
	else {
		bTickTrigger2x = !bTickTrigger2x;
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMainCharacter::Event_LMB);
	PlayerInputComponent->BindAction("CameraRotationActivation", IE_Pressed, this, &AMainCharacter::Event_CameraRotationStart);
	PlayerInputComponent->BindAction("CameraRotationActivation", IE_Released, this, &AMainCharacter::Event_CameraRotationEnd);
	PlayerInputComponent->BindAxis("CameraRotationAxisX", this, &AMainCharacter::Event_CameraRotationX);
	PlayerInputComponent->BindAxis("CameraRotationAxisY", this, &AMainCharacter::Event_CameraRotationY);

	PlayerInputComponent->BindAxis("CameraZoom", this, &AMainCharacter::Event_CameraZoom);


}

void AMainCharacter::Event_LMB()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	FHitResult HitResult(EForceInit::ForceInit);

	//APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController != nullptr) {
		PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult);
		
		if (HitResult.GetActor() != nullptr) {
			AChessMaster* CHMaster = Cast<AChessMaster>(HitResult.GetActor());

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitResult.GetActor()->GetName());


			if (CHMaster != nullptr) { // on new selection, deselect old object and select new
				if (CHTemp == CHMaster) // in case if selection is the same as before
					return;

				if (CHTemp != CHMaster && CHTemp != nullptr) {
					CHTemp->OnUnSelected();
					CHTemp = CHMaster;
				}
				else { // in case if this is a new selection
					CHTemp = CHMaster;
				}

				// Only owner/current player could access his chesses
				if (CHMaster->GetChessColor() == CurrentPlayerColor) {
					CHMaster->OnSelected();
				}
				else {
					CHMaster->OnUnSelected();
				}
			}
			else { // in case if Cast failed and selection is not a child of AChessMaster 
				AChessPlaceToMove* ChessToMove = Cast<AChessPlaceToMove>(HitResult.GetActor());
				if (ChessToMove != nullptr && CHTemp != nullptr) {
					FVector LocationToMove = ChessToMove->GetActorLocation();
					
					CHTemp->OnUnSelected();
					
					UWorld* World = GetWorld();
					if (World == nullptr) {
						DEBUGMESSAGE("Problem with retrieving UWORLD for tracing.");
						return;
					}

					FHitResult HitResultEnemy(EForceInit::ForceInit);

					World->LineTraceSingleByChannel(HitResultEnemy, LocationToMove + FVector(0, 0, 200), LocationToMove, ECollisionChannel::ECC_WorldDynamic);
					if (HitResultEnemy.GetActor() != nullptr) {
						AChessMaster* CHMasterEnemy = Cast<AChessMaster>(HitResultEnemy.GetActor());
						if (CHMasterEnemy != nullptr) {
							if (CHMasterEnemy->GetbIsKing()) {
								UE_LOG(LogTemp, Warning, TEXT("GAME IS END!"));
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("NOT A KING!"));
							}
							CHMasterEnemy->DestroyChess();
						}
					}
					CHTemp->MoveToSelectedPlace(LocationToMove);
					CHTemp = nullptr;

					NextTurn();
					// TODO
					GetPlayerHUD()->NextPlayer();
				}
				else if (CHTemp != nullptr) {
					CHTemp->OnUnSelected();
					CHTemp = nullptr;
				}
			}
		} else { // If selected was a skybox etc. - unselect selected object
			if (CHTemp != nullptr) {
				CHTemp->OnUnSelected();
				CHTemp = nullptr;
			}
		}
	}
}

void AMainCharacter::Event_CameraRotationStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("Camera Rotator Start !"));
	bIsCameraRotatingActive = true;
	
	APlayerController_Mouse* PController = Cast<APlayerController_Mouse>(GetController());
	if (PController != nullptr) {
		PController->SetCardinalCrossCursor();
	}
}

void AMainCharacter::Event_CameraRotationEnd()
{
	//UE_LOG(LogTemp, Warning, TEXT("Camera Rotator END !"));

	bIsCameraRotatingActive = false;

	APlayerController_Mouse* PController = Cast<APlayerController_Mouse>(GetController());
	if (PController != nullptr) {
		PController->SetDefaultCursor();
	}
}


void AMainCharacter::Event_CameraZoom(float AxisValue)
{
	if (AxisValue != 0.0f) {
		//UE_LOG(LogTemp, Warning, TEXT("Camera zoom Axis Value: %f!"), AxisValue);
		float newVar = CameraBoom->TargetArmLength + BaseZoomRate * (-1.f * AxisValue);
		if (newVar <= SpringArmLenghtMax && newVar >= SpringArmLenghtMin) {
			CameraBoom->TargetArmLength = newVar;
		}
	}
}

void AMainCharacter::Event_CameraRotationX(float AxisValue)
{
	if (AxisValue != 0.0f && bIsCameraRotatingActive == true) {
		//UE_LOG(LogTemp, Warning, TEXT("Camera Rotation X: %f"), AxisValue);
		AddControllerYawInput(AxisValue * BaseRotationRate);
		
	}

}

void AMainCharacter::Event_CameraRotationY(float AxisValue)
{
	if (AxisValue != 0.0f && bIsCameraRotatingActive == true) {
		//UE_LOG(LogTemp, Warning, TEXT("Camera Rotation Y: %f"), AxisValue);
		float newPitch = FMath::Clamp(GetController()->GetControlRotation().Pitch + AxisValue * BaseRotationRate * 2, 280.f, 359.f);
		//float newVar = FMath::Abs() + AxisValue * BaseRotationRate + 1;
		//if (newVar >= PitchCameraMin && newVar <= PitchCameraMax) {
			//AddControllerPitchInput(AxisValue * BaseRotationRate);
		//UE_LOG(LogTemp, Warning, TEXT("New Pitch: %f"),newPitch);
		//UE_LOG(LogTemp, Warning, TEXT("Controller Pitch: %f"), GetController()->GetControlRotation().Pitch);

		FRotator Rotator(newPitch, GetController()->GetControlRotation().Yaw, GetController()->GetControlRotation().Roll);
		GetController()->SetControlRotation(Rotator);
		//AddControllerPitchInput(AxisValue);
		//}
	}
}

ABaseHUD* AMainCharacter::GetPlayerHUD()
{
	if (PlayerHUD != nullptr) {
		return PlayerHUD;
	}

	APlayerController* PController = Cast<APlayerController>(GetController());
	if (PController != nullptr) {
		ABaseHUD* CurrentHUD = Cast<ABaseHUD>(PController->GetHUD());
		if (CurrentHUD != nullptr) {
			PlayerHUD = CurrentHUD;
			return PlayerHUD;
		}
	}

	return nullptr;
}

void AMainCharacter::TraceForChess(AChessMaster* TracedChess)
{
	if (TracedChess == nullptr) {
		if (CHTickTemp != nullptr) {
			CHTickTemp->OnHoverEnd();
		}
		CHTickTemp = nullptr;
		if (CurrentMouseCursor == ECharacterMouseType::HOVERING) {
			SetNewMouseCursor(ECharacterMouseType::DEFAULT);
		}
		return;
	}

	if (TracedChess == CHTickTemp)
		return;

	if (CHTickTemp != nullptr) {
		CHTickTemp->OnHoverEnd();
	}

	// Only Owner could move his chesses
	if (TracedChess->GetChessColor() == CurrentPlayerColor) {
		TracedChess->OnHoverBegin();
		CHTickTemp = TracedChess;
		SetNewMouseCursor(ECharacterMouseType::HOVERING);
	}
}

void AMainCharacter::TraceForVFX(AChessPlaceToMove* TracedVFX)
{
	if (TracedVFX == nullptr) {
		if (VFXTickTemp != nullptr) {
			VFXTickTemp->Event_OnMouseEndOver();
		}
		VFXTickTemp = nullptr;

		if (CurrentMouseCursor == ECharacterMouseType::SELECTION) {
			SetNewMouseCursor(ECharacterMouseType::DEFAULT);
		}
		return;
	}

	if (TracedVFX == VFXTickTemp)
		return;

	if (VFXTickTemp != nullptr) {
		VFXTickTemp->Event_OnMouseEndOver();
	}
	TracedVFX->Event_OnMouseBeginOver();
	VFXTickTemp = TracedVFX;

	SetNewMouseCursor(ECharacterMouseType::SELECTION);
}
