// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessRook.h"
#include "Engine.h"
#include "CoreMinimal.h"

void AChessRook::TraceInDirectionAndSpawnVFX(FVector DirectionVector)
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

void AChessRook::OnSelected()
{
	Super::OnSelected();
	//Trace 12h dir
	TraceInDirectionAndSpawnVFX(GetActorForwardVector());

	//Trace 3h dir
	TraceInDirectionAndSpawnVFX(GetActorRightVector());

	//trace 6h dir
	TraceInDirectionAndSpawnVFX(-GetActorForwardVector());

	//trace 9h dir
	TraceInDirectionAndSpawnVFX(-GetActorRightVector());
}
