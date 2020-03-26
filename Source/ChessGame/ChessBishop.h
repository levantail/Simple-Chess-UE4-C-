// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessMaster.h"
#include "ChessBishop.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessBishop : public AChessMaster
{
	GENERATED_BODY()
protected:
	void TraceInDirectionAndSpawnVFX(FVector DirectionVector);
public:
	void OnSelected();
};
