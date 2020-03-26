// Fill out your copyright notice in the Description page of Project Settings.


#include "ABoardCell.h"
#include "Engine.h"
#include "CoreMinimal.h"

// Sets default values
AABoardCell::AABoardCell()
{
	PrimaryActorTick.bCanEverTick = true;
	BoardMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardCell"));
	SetRootComponent(BoardMeshComp);
	//BoardMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

}

// Called when the game starts or when spawned
void AABoardCell::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AABoardCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABoardCell::SetBoardMesh(UStaticMesh* NewBoardMesh) {
	if (NewBoardMesh != nullptr) {
		BoardMeshComp->SetStaticMesh(NewBoardMesh);
	}
}

void AABoardCell::SetBoardMI(UMaterialInterface* NewBoardMI)
{
	if (NewBoardMI != nullptr ) {
		BoardMeshComp->SetMaterial(0, NewBoardMI);
	}
}

