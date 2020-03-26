// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UMG_Base.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API UUMG_Base : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	FText WhitePlayerText;
	FText DarkPlayerText;

	UFUNCTION(BlueprintImplementableEvent)
		void Anim_NextPlayer(const FText& TextToDisplay_out);
	

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* ItemTitle = nullptr;

	UUMG_Base(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NextPlayer();
};
