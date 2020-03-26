// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_Base.h"

UUMG_Base::UUMG_Base(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	WhitePlayerText = FText::FromString("White Turn");
	DarkPlayerText = FText::FromString("Black Turn");

	//ItemTitle->Text = WhitePlayerText;
}

void UUMG_Base::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemTitle != nullptr) {
		ItemTitle->SetText(WhitePlayerText);
	}
}

void UUMG_Base::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("HELLO FROM CPP"));
}	

void UUMG_Base::NextPlayer()
{
	if (ItemTitle == nullptr)
		return;

	

	if (ItemTitle->GetText().ToString() == WhitePlayerText.ToString()) {
		//ItemTitle->SetText(DarkPlayerText);
		Anim_NextPlayer(DarkPlayerText);
	}
	else {
		Anim_NextPlayer(WhitePlayerText);
		//ItemTitle->SetText(WhitePlayerText);
	}

}


