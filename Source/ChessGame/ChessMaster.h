// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "ChessGame/ChessPlaceToMove.h"
#include "ChessGame/ABoardCell.h"
#include "Containers/Array.h"
//#include "Math/UnrealMathVectorCommon.h"
#include "ComponentInstanceDataCache.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

//#include "NiagaraSystem.h"
//#include "Niagara/Public/NiagaraFunctionLibrary.h"
//#include "Niagara/Classes/NiagaraSystem.h"
//#include "Niagara/Classes/NiagaraFunctionLibrary.h"
//#include "Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
//#include "/Program Files/Epic Games/UE_4.23/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
//#include "/Program Files/Epic Games/UE_4.23/Engine/Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"

#include "ChessMaster.generated.h"

#define DEBUGMESSAGE(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, TEXT(x));}

UENUM(BlueprintType)
enum class EChessColor : uint8
{
	CE_WHITE UMETA(DisplayName = "WHITE"),
	CE_DARK UMETA(DisplayName = "DARK")
};

UENUM(BlueprintType)
enum class EChessType : uint8
{
	CE_PAWN UMETA(DisplayName = "PAWN"),
	CE_KNIGHT UMETA(DisplayName = "KNIGHT"),
	CE_ROOK UMETA(DisplayName = "ROOK"),
	CE_BISHOP UMETA(DisplayName = "BISHOP"),
	CE_QUEEN UMETA(DisplayName = "QUEEN"),
	CE_KING UMETA(DisplayName = "KING")
};

UCLASS()
class CHESSGAME_API AChessMaster : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChessMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ChessComp;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* NSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enum)
		EChessColor ChessColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enum)
		EChessType ChessType;

	UFUNCTION()
		void DelayedDestroy();


	virtual void TimeToMoveFinished();

	FTimerHandle DestoryTimerHandle;

	UStaticMesh* ChessMeshCH;
	UMaterialInterface* ChessMICH;
	UMaterialInstanceDynamic* BlendMID;

	//int8 xPos;
	//int8 yPos;
	bool bIsHovering = false;
	bool bIsTransitionEnabled = true;
	bool bIsSelected = false;
	float BlendState;

	int BoardStep;
	TArray<AChessPlaceToMove*> LPositionsToMove;

	// Max Blend Value 
	float TransitionDistance_MAT = 1.0f;
	// Transition Blending Speed Between two materials in seconds 
	float TransitionTime_MAT = 0.1f;
	float TransitionDep_MAT = TransitionDistance_MAT / TransitionTime_MAT;

	bool bIsTimeToMoveDoOnce = true;
	bool bIsTimeToMove = false;
	bool bIsFlyMoving = false;
	FVector FMoveFrom;
	FVector FMoveTo;

	float FlyHeight = 30;
	bool bFlyLerpTrigger = true;


	float Alpha_MoveTo = 0;
	float MovingDurationSec = 0.39f;
	bool bIsKing = false;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UFUNCTION()
	virtual void OnHoverBegin();//(UPrimitiveComponent* TouchedComponent);
	//UFUNCTION()
	virtual void OnHoverEnd();//(UPrimitiveComponent* TouchedComponent);

	virtual void OnSelected();
	virtual void OnUnSelected();

	virtual void DestroyChess();
	
	virtual void MoveToSelectedPlace(FVector NewLocation);
	virtual bool GetbIsKing();
	virtual void SetBoardStep(int NewValue) { BoardStep = NewValue;	}

	//virtual int8 GetXPos() { return xPos; }
	//virtual void SetXPos(int8 newXPos) { xPos = newXPos; }

	//virtual int8 GetYPos() { return yPos; }
	//virtual void SetYPos(int8 newYPos) { yPos = newYPos; }

	virtual EChessType GetChessType() { return ChessType; }
	virtual void SetChessType(EChessType NewChessType) { ChessType = NewChessType; }

	virtual EChessColor GetChessColor() { return ChessColor; }
	virtual void SetChessColor(EChessColor NewChessColor) { ChessColor = NewChessColor; }

	virtual void SetChessMeshCH(UStaticMesh* NewChessMeshCH) { ChessMeshCH = NewChessMeshCH; }
	virtual void SetChessMICH(UMaterialInterface* NewChessMICH) { ChessMICH = NewChessMICH; }

};