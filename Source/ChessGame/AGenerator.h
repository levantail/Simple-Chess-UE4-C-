// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <ChessGame\ABoardCell.h>

#include <ChessGame\ChessRook.h>
#include <ChessGame\ChessKing.h>
#include <ChessGame\ChessBishop.h>
#include <ChessGame\ChessPawn.h>
#include <ChessGame\ChessQueen.h>
#include <ChessGame\ChessKnight.h>

//#include <ChessGame\MainCharacter.h>
#include "AGenerator.generated.h"

#define DEBUGMESSAGE(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, TEXT(x));}


UCLASS()
class CHESSGAME_API AAGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAGenerator();

		int amountX;
		int amountY;
		float offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* SM_ChessBoard UMETA(DisplayName = "Chess Board Mesh");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* MAT_DarkBoard UMETA(DisplayName = "Dark Board Material");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* MAT_WhiteBoard UMETA(DisplayName = "White Board Material");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* MAT_DarkChess UMETA(DisplayName = "Dark Chess Material");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* MAT_WhiteChess UMETA(DisplayName = "White Chess Material");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* SM_ChessPawn UMETA(DisplayName = "Pawh Mesh");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* SM_ChessRook UMETA(DisplayName = "Rook Mesh");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* SM_ChessKnight UMETA(DisplayName = "Knight Mesh");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* SM_ChessBishop UMETA(DisplayName = "Bishop Mesh");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* SM_ChessQueen UMETA(DisplayName = "Queen Mesh");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* SM_ChessKing UMETA(DisplayName = "King Mesh");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform CharacterPos;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GenerateChessBoard();
	void GenerateChesses();
	void SpawnCharacter();
	//GenerateSingleChess
	void GenerateChessByType(int ChessPosX, int ChessPosY, EChessType ChessType, EChessColor ChessColor);

	FVector V_BottomLeft;
	FVector V_TopRight;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
