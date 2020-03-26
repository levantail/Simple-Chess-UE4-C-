// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "ChessPlaceToMove.generated.h"


UCLASS()
class CHESSGAME_API AChessPlaceToMove : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChessPlaceToMove();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* PS_fx;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* BoxCollision;
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* BoxCollisionEnemy;


	FVector emptyCellColor = FVector(0, 15, 5);
	FVector enemyCellColor = FVector(15, 4, 0);

	// Original 3D scale of FX 
	float FX_Orig3DScale;
	// Max 3D scale of FX  
	float FX_Overl3DScale;

	float TransitionState;

	float TransitionDep_MAT;

	bool bMouseOver = false;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetEmptyColor();
	void SetEnemyColor();
	//void SetCHOriginColor(EChessColor NewColor);

	//UFUNCTION()
	//	void Event_OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	void Event_OnMouseBeginOver();//(UPrimitiveComponent* TouchedComponent);
//UFUNCTION()
	void Event_OnMouseEndOver();//(UPrimitiveComponent* TouchedComponent);
};
