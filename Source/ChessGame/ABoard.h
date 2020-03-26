// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABoard.generated.h"

UCLASS()
class CHESSGAME_API AABoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABoard();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int amountX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int amountY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* SM_ChessBoard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* MAT_DarkBoard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* MAT_WhiteBoard;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	//Constructor: destroy all generated meshes
	virtual void CDeystroyGeneratedMeshes();

	TArray <UStaticMeshComponent *> Meshes;

	int amountXold;
	int amountYold;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
