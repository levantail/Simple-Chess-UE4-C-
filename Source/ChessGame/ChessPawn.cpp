// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPawn.h"
#include "DrawDebugHelpers.h"
#include <ChessGame/ChessQueen.h>

void AChessPawn::OnSelected()
{
	Super::OnSelected();

	UWorld* World = GetWorld();
	if (World == nullptr) {
		return;
	}

	FHitResult HitResult(EForceInit::ForceInit);
	FVector TraceStart;
	FVector TraceEnd;
	FTransform SpawnTransform;

	//Pawn 2x forward steps
	for (int i = 1; i < 2 + 1 - rangeFix; ++i)
	{
		TraceStart = (GetActorLocation() + (GetActorForwardVector() * BoardStep * i)) + FVector(0, 0, 200);
		TraceEnd = TraceStart - FVector(0, 0, 300);

		World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldDynamic);


		//DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 3.f);
		//DrawDebugSphere(World, TraceStart, 3.f, 1, FColor::Red, false, 3.f);
		//DrawDebugSphere(World, TraceEnd, 3.f, 1, FColor::Red, false, 3.f);
		//If we out of board
		if (HitResult.GetActor() == nullptr) {
			DEBUGMESSAGE("OUT OF BOARD ?");
			return;
		}

		AABoardCell* HitBoard = Cast<AABoardCell>(HitResult.GetActor());
		AChessMaster* HitChess = Cast<AChessMaster>(HitResult.GetActor());

		SpawnTransform.SetLocation(GetActorLocation() + (GetActorForwardVector() * BoardStep * i));
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());
		SpawnTransform.SetScale3D(FVector(1, 1, 1));

		if (HitChess != nullptr) {
			break;
			//EChessColor TracedChessColor = HitChess->GetChessColor();
			//// Do nothing if traced chess have the same color
			//if (TracedChessColor == ChessColor)
			//	return;

			//AChessPlaceToMove* spawnedActorDefferred = World->SpawnActorDeferred<AChessPlaceToMove>(AChessPlaceToMove::StaticClass(), SpawnTransform);
			//spawnedActorDefferred->SetEnemyColor();
			//spawnedActorDefferred->FinishSpawning(SpawnTransform);

			//spawnedActorDefferred->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			//LPositionsToMove.Add(spawnedActorDefferred);
		}
		else if (HitBoard != nullptr) {
			AChessPlaceToMove* spawnedActorDefferred = World->SpawnActorDeferred<AChessPlaceToMove>(AChessPlaceToMove::StaticClass(), SpawnTransform);
			spawnedActorDefferred->SetEmptyColor();
			spawnedActorDefferred->FinishSpawning(SpawnTransform);

			spawnedActorDefferred->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			LPositionsToMove.Add(spawnedActorDefferred);
		}
		else
		{
			break;
		}
	}

	//Pawn looking for enemy
	int trigger = -1;
	for (int i = 0; i < 2; ++i) {
		if (i == 1)
			trigger = -trigger;

		TraceStart = (GetActorLocation() + FVector(0, trigger * BoardStep, 0) + (GetActorForwardVector()* BoardStep)) + FVector(0, 0, 200);
		TraceEnd = TraceStart - FVector(0, 0, 300);

		World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldDynamic);

		//If we out of board
		if (HitResult.GetActor() == nullptr)
			continue;

		AChessMaster* HitChess = Cast<AChessMaster>(HitResult.GetActor());

		if (HitChess == nullptr) {
			continue;
		}

		EChessColor TracedChessColor = HitChess->GetChessColor();
		// Do nothing if traced chess have the same color
		if (TracedChessColor == ChessColor)
			continue;

		SpawnTransform.SetLocation(GetActorLocation() + FVector(0, trigger * BoardStep, 0) + (GetActorForwardVector()* BoardStep));
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());
		SpawnTransform.SetScale3D(FVector(1, 1, 1));

		AChessPlaceToMove* spawnedActorDefferred = World->SpawnActorDeferred<AChessPlaceToMove>(AChessPlaceToMove::StaticClass(), SpawnTransform);
		spawnedActorDefferred->SetEnemyColor();
		spawnedActorDefferred->FinishSpawning(SpawnTransform);

		spawnedActorDefferred->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		LPositionsToMove.Add(spawnedActorDefferred);
	}
}



void AChessPawn::SetQueenProps(UStaticMesh* NewQueenMesh)
{
	QueenMesh = NewQueenMesh;
}

void AChessPawn::PawnToQueen()
{
	UWorld* World = GetWorld();
	
	if (World) {
		FTransform SpawnTransform;
		SpawnTransform = GetActorTransform();

		AChessQueen * NewChess = World->SpawnActorDeferred<AChessQueen>(AChessQueen::StaticClass(), SpawnTransform);
		NewChess->SetChessMeshCH(QueenMesh);
		NewChess->SetChessMICH(ChessMICH);
		NewChess->SetChessType(ChessType);
		//NewChess->SetXPos(xPos);
		//NewChess->SetYPos(yPos);
		NewChess->SetChessColor(ChessColor);
		NewChess->SetBoardStep(BoardStep);
		NewChess->FinishSpawning(SpawnTransform);
	}
	Destroy();
}

void AChessPawn::TimeToMoveFinished()
{
	Super::TimeToMoveFinished();

	UWorld* World = GetWorld();
	if (World == nullptr) {
		return;
	}
	if (rangeFix == 0) {
		rangeFix = 1;
	}
	FHitResult HitResult(EForceInit::ForceInit);
	FVector TraceStart;
	FVector TraceEnd;
	FTransform SpawnTransform;

	//Pawn 2x forward steps
	TraceStart = (GetActorLocation() + (GetActorForwardVector() * BoardStep)) + FVector(0, 0, 200);
	TraceEnd = TraceStart - FVector(0, 0, 300);

	World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldDynamic);
	//DrawDebugLine(World, TraceStart, TraceEnd, FColor::Blue, false, 3.f);
	//DrawDebugSphere(World, TraceStart, 3.f, 1, FColor::Blue, false, 3.f);
	//DrawDebugSphere(World, TraceEnd, 3.f, 1, FColor::Blue, false, 3.f);

	//If we out of board
	if (HitResult.GetActor() == nullptr) {
		DEBUGMESSAGE("Attempt transform to queen");
		PawnToQueen();
	}
}
