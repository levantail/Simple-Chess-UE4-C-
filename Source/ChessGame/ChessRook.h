// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessMaster.h"
#include "ChessRook.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessRook : public AChessMaster
{
	GENERATED_BODY()

private:
	// Help fn only for rook.
	void TraceInDirectionAndSpawnVFX(FVector DirectionVector);
public:
	void OnSelected();

};
