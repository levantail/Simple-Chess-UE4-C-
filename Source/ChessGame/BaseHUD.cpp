// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"

ABaseHUD::ABaseHUD(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	
	static ConstructorHelpers::FClassFinder<UUserWidget> Loaded_Widget(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/PlayerTurnUMG.PlayerTurnUMG_C'"));
	
	if (Loaded_Widget.Succeeded() == true) {
		WidgetClass = Loaded_Widget.Class;
	}


}

void ABaseHUD::NextPlayer()
{
	if (MainUMG != nullptr) {
		MainUMG->NextPlayer();
	}
}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	MainUMG= CreateWidget<UUMG_Base>(GetOwningPlayerController(), WidgetClass);
	if (MainUMG != nullptr) {
		MainUMG->AddToViewport();
	}
}
