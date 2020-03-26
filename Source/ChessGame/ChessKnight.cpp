// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessKnight.h"


void AChessKnight::TraceInDirectionAndSpawnVFX(FVector DirectionVector)
{
	UWorld* World = GetWorld();
	if (World == nullptr) {
		return;
	}

	FHitResult HitResult(EForceInit::ForceInit);
	FVector TraceStart;
	FVector TraceEnd;
	FTransform SpawnTransform;



	TraceStart = (GetActorLocation() + DirectionVector) + FVector(0, 0, 200);
	TraceEnd = TraceStart - FVector(0, 0, 300);

	World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldDynamic);
	//DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 3.f);
	//DrawDebugSphere(World, TraceStart, 3.f, 10, FColor::Blue, false, 3.f);
	//DrawDebugSphere(World, TraceEnd, 3.f, 10, FColor::Black, false, 3.f);

	//If we out of board
	if (HitResult.GetActor() == nullptr) {
		return;
	}

	AABoardCell* HitBoard = Cast<AABoardCell>(HitResult.GetActor());
	AChessMaster* HitChess = Cast<AChessMaster>(HitResult.GetActor());

	SpawnTransform.SetLocation(GetActorLocation() + DirectionVector );
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
		//LPositionsToMove.Add(spawnedActorDefferred);
		LPositionsToMove.Push(spawnedActorDefferred);
		return;
	}
	//Hit is a part of game board
	else if (HitBoard != nullptr) {
		AChessPlaceToMove* spawnedActorDefferred = World->SpawnActorDeferred<AChessPlaceToMove>(AChessPlaceToMove::StaticClass(), SpawnTransform);
		spawnedActorDefferred->SetEmptyColor();
		spawnedActorDefferred->FinishSpawning(SpawnTransform);

		spawnedActorDefferred->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		LPositionsToMove.Push(spawnedActorDefferred);
		//LPositionsToMove.Add(spawnedActorDefferred);
	}
	// if smth else
	else
	{
		return;
	}


}

void AChessKnight::OnSelected()
{
	Super::OnSelected();
	//Trace 1h dir
	TraceInDirectionAndSpawnVFX(FVector(
		(GetActorForwardVector() * BoardStep * 2).X,
		(GetActorRightVector() * BoardStep * 1).Y,
		GetActorForwardVector().Z
	));
	//Trace 2h dir
	TraceInDirectionAndSpawnVFX(FVector(
		(GetActorForwardVector() * BoardStep * 1).X,
		(GetActorRightVector() * BoardStep * 2).Y,
		GetActorForwardVector().Z
	));
	//Trace 11h dir 
	TraceInDirectionAndSpawnVFX(FVector(
		(GetActorForwardVector() * BoardStep * 2).X,
		(-GetActorRightVector() * BoardStep * 1).Y,
		GetActorForwardVector().Z
	));
	//Trace 10h dir
	TraceInDirectionAndSpawnVFX(FVector(
		(GetActorForwardVector() * BoardStep * 1).X,
		(-GetActorRightVector() * BoardStep * 2).Y,
		GetActorForwardVector().Z
	));

	//Trace 4h dir
	TraceInDirectionAndSpawnVFX(FVector(
		(-GetActorForwardVector() * BoardStep * 2).X,
		(GetActorRightVector() * BoardStep * 1).Y,
		GetActorForwardVector().Z
	));
	//Trace 5h dir
	TraceInDirectionAndSpawnVFX(FVector(
		(-GetActorForwardVector() * BoardStep * 1).X,
		(GetActorRightVector() * BoardStep * 2).Y,
		GetActorForwardVector().Z
	));

	//Trace 7h dir
	TraceInDirectionAndSpawnVFX(FVector(
		(-GetActorForwardVector() * BoardStep * 2).X,
		(-GetActorRightVector() * BoardStep * 1).Y,
		GetActorForwardVector().Z
	));
	//Trace 8h dir
	TraceInDirectionAndSpawnVFX(FVector(
		(-GetActorForwardVector() * BoardStep * 1).X,
		(-GetActorRightVector() * BoardStep * 2).Y,
		GetActorForwardVector().Z
	));

}