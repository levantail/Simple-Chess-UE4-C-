// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "ABoardCell.generated.h"

UCLASS()
class CHESSGAME_API AABoardCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABoardCell();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BoardMeshComp;

	UMaterialInterface* BoardMI;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetBoardMesh(UStaticMesh* NewBoardMesh);
	void SetBoardMI(UMaterialInterface* NewBoardMI);
};
