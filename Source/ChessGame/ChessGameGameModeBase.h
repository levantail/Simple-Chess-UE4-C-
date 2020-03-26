// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <ChessGame/PlayerController_Mouse.h>
#include <ChessGame/MainCharacter.h>
#include "ChessGameGameModeBase.generated.h"
/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChessGameGameModeBase();
};
