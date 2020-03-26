// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessMaster.h"
#include "ChessQueen.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessQueen : public AChessMaster
{
	GENERATED_BODY()
protected:
	void TraceInDirectionAndSpawnVFX(FVector DirectionVector);
public:
	void OnSelected();
};
