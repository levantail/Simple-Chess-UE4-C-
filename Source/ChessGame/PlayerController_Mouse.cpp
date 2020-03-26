// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController_Mouse.h"


APlayerController_Mouse::APlayerController_Mouse()
{
	this->bShowMouseCursor = true;
	this->bEnableMouseOverEvents = true;
	this->bEnableClickEvents = true;
	
}

void APlayerController_Mouse::SetDefaultCursor()
{
	this->CurrentMouseCursor = EMouseCursor::Default;
}

void APlayerController_Mouse::SetCardinalCrossCursor()
{
	this->CurrentMouseCursor = EMouseCursor::CardinalCross;
}

void APlayerController_Mouse::SetHandCursor()
{
	this->CurrentMouseCursor = EMouseCursor::Hand;

}

void APlayerController_Mouse::SetSelectCursor()
{
	this->CurrentMouseCursor = EMouseCursor::TextEditBeam;

}

void APlayerController_Mouse::SetLoadingCursor()
{
	this->CurrentMouseCursor = EMouseCursor::Crosshairs;

}

void APlayerController_Mouse::SetSlashedCircleCursor()
{
	this->CurrentMouseCursor = EMouseCursor::SlashedCircle;

}
