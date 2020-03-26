// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameGameModeBase.h"

AChessGameGameModeBase::AChessGameGameModeBase()
{
	this->PlayerControllerClass = APlayerController_Mouse::StaticClass();
	this->DefaultPawnClass = AMainCharacter::StaticClass();
}
