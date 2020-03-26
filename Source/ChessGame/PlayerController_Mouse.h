// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_Mouse.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API APlayerController_Mouse : public APlayerController
{
	GENERATED_BODY()
	
	APlayerController_Mouse();

public:
	void SetDefaultCursor();
	//When Rotating Camera
	void SetCardinalCrossCursor();
	//When Hovering a Chess
	void SetHandCursor();
	//When Picking Up a Place to Move
	void SetSelectCursor();
	//When Chess is Moving
	void SetLoadingCursor();
	//When Pointing Something where we can't Move a Chess
	void SetSlashedCircleCursor();
};
