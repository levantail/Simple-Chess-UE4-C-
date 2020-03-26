// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Components/SizeBox.h"
#include "Components/TextBlock.h"


#include "ChessGame/UMG_Base.h"
#include "ConstructorHelpers.h"

#include "BaseHUD.generated.h"


/**
 * 
 */
UCLASS()
class CHESSGAME_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ABaseHUD(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY()
	UUMG_Base* MainUMG;


	void NextPlayer();

	void BeginPlay();
};
