// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessMaster.h"
#include "ChessPawn.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessPawn : public AChessMaster
{
	GENERATED_BODY()

protected:

	void PawnToQueen();
	UStaticMesh* QueenMesh;

	int8 rangeFix = 0;
	virtual void TimeToMoveFinished();
public:
	void OnSelected();

	void SetQueenProps(UStaticMesh* NewQueenMesh);

};
