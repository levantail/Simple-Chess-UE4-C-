// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessMaster.h"
#include "ChessKing.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessKing : public AChessMaster
{
	GENERATED_BODY()
private:

	void TraceInDirectionAndSpawnVFX(FVector DirectionVector);
public:
	AChessKing();
	void OnSelected();
};
