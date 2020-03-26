// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessQueen.h"

void AChessQueen::TraceInDirectionAndSpawnVFX(FVector DirectionVector)
{
	UWorld* World = GetWorld();
	if (World == nullptr) {
		return;
	}

	FHitResult HitResult(EForceInit::ForceInit);
	FVector TraceStart;
	FVector TraceEnd;
	FTransform SpawnTransform;

	int i = 0;

	while (true)
	{
		++i;

		TraceStart = (GetActorLocation() + (DirectionVector * BoardStep * i)) + FVector(0, 0, 200);
		TraceEnd = TraceStart - FVector(0, 0, 300);

		World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldDynamic);
		//DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 20.f);
		//DrawDebugSphere(World, TraceStart, 100.f, 10, FColor::Red, false, 2.f);

		//If we out of board
		if (HitResult.GetActor() == nullptr) {
			return;
		}

		AABoardCell* HitBoard = Cast<AABoardCell>(HitResult.GetActor());
		AChessMaster* HitChess = Cast<AChessMaster>(HitResult.GetActor());

		SpawnTransform.SetLocation(GetActorLocation() + (DirectionVector * BoardStep * i));
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());
		SpawnTransform.SetScale3D(FVector(1, 1, 1));

		//Hit is a chess
		if (HitChess != nullptr) {

			EChessColor TracedChessColor = HitChess->GetChessColor();
			// Do nothing if traced chess have the same color
			if (TracedChessColor == ChessColor)
				return;

			AChessPlaceToMove* spawnedActorDefferred = World->SpawnActorDeferred<AChessPlaceToMove>(AChessPlaceToMove::StaticClass(), SpawnTransform);
			spawnedActorDefferred->SetEnemyColor();
			spawnedActorDefferred->FinishSpawning(SpawnTransform);

			spawnedActorDefferred->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			LPositionsToMove.Add(spawnedActorDefferred);
			return;
		}
		//Hit is a part of game board
		else if (HitBoard != nullptr) {
			AChessPlaceToMove* spawnedActorDefferred = World->SpawnActorDeferred<AChessPlaceToMove>(AChessPlaceToMove::StaticClass(), SpawnTransform);
			spawnedActorDefferred->SetEmptyColor();
			spawnedActorDefferred->FinishSpawning(SpawnTransform);

			spawnedActorDefferred->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			LPositionsToMove.Add(spawnedActorDefferred);
		}
		// if smth else
		else
		{
			return;
		}
	}
}

void AChessQueen::OnSelected()
{
	Super::OnSelected();
	//Trace Up
	TraceInDirectionAndSpawnVFX(GetActorForwardVector());
	//Trace Up Right
	TraceInDirectionAndSpawnVFX(
		FVector(
			GetActorForwardVector().X,
			GetActorRightVector().Y,
			GetActorForwardVector().Z
		)
	);
	//Trace Right 
	TraceInDirectionAndSpawnVFX(GetActorRightVector());
	//Trace Backward Right
	TraceInDirectionAndSpawnVFX(
		FVector(
			-GetActorForwardVector().X,
			GetActorRightVector().Y,
			GetActorForwardVector().Z
		)
	);

	//Trace Backward
	TraceInDirectionAndSpawnVFX(-GetActorForwardVector());
	//Trace Backward Left
	TraceInDirectionAndSpawnVFX(
		FVector(
			-GetActorForwardVector().X,
			-GetActorRightVector().Y,
			GetActorForwardVector().Z
		)
	);
	//Trace Left
	TraceInDirectionAndSpawnVFX(-GetActorRightVector());
	//Trace Up Left
	TraceInDirectionAndSpawnVFX(
		FVector(
			GetActorForwardVector().X,
			-GetActorRightVector().Y,
			GetActorForwardVector().Y
		)
	);
}
