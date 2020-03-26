// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlaceToMove.h"


// Sets default values
AChessPlaceToMove::AChessPlaceToMove()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneComp);
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Loaded_particles(TEXT("ParticleSystem'/Game/Effects/Particles/P_ky_magicCircle1.P_ky_magicCircle1'"));
	PS_fx = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_fx"));
	PS_fx->SetupAttachment(GetRootComponent());
	PS_fx->SetRelativeScale3D(FVector(0.11f, 0.11f, 0.11f));

	if (Loaded_particles.Succeeded()) {
		PS_fx->SetTemplate(Loaded_particles.Object);
		PS_fx->SetVectorParameter(FName("color"), emptyCellColor);
	}

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());
	BoxCollision->SetBoxExtent(FVector(20.0f, 20.0f, 1.0f));
	BoxCollision->SetRelativeLocation(FVector(0, 0, 0));
	BoxCollision->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Block);


	BoxCollisionEnemy = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionEnemy"));
	BoxCollisionEnemy->SetupAttachment(GetRootComponent());
	BoxCollisionEnemy->SetBoxExtent(FVector(13.0f, 13.0f, 40.0f));
	BoxCollisionEnemy->SetRelativeLocation(FVector(0, 0, 0));
	BoxCollisionEnemy->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollisionEnemy->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollisionEnemy->SetCollisionResponseToAllChannels(ECR_Block);
	//BoxCollisionEnemy->SetGenerateOverlapEvents(false);

	//BoxCollision->SetCollisionObjectType(ECC_EngineTraceChannel1);
	//BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &AChessPlaceToMove::Event_OnComponentBeginOverlap);
	
	//BoxCollision->OnBeginCursorOver.AddDynamic(this, &AChessPlaceToMove::Event_OnMouseBeginOver);
	//BoxCollision->OnEndCursorOver.AddDynamic(this, &AChessPlaceToMove::Event_OnMouseEndOver);
	

	//FX_Orig3DScale = PS_fx->RelativeScale3D.X;
	FX_Orig3DScale = PS_fx->GetRelativeScale3D().X;
	TransitionState = PS_fx->GetRelativeScale3D().X;
	//FX Size on Mouse Hover
	FX_Overl3DScale = .17f;

	TransitionDep_MAT = (FX_Overl3DScale - FX_Orig3DScale) / .1f;

}

// Called when the game starts or when spawned
void AChessPlaceToMove::BeginPlay()
{
	Super::BeginPlay();

}
// Called every frame
void AChessPlaceToMove::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMouseOver) {
		if (PS_fx->GetRelativeScale3D().X < FX_Overl3DScale) {
			TransitionState += TransitionDep_MAT * DeltaTime;
			if (TransitionState > FX_Overl3DScale)
				TransitionState = FX_Overl3DScale;

			PS_fx->SetRelativeScale3D(FVector(TransitionState, TransitionState, TransitionState));
		}
	}
	else {
		if (PS_fx->GetRelativeScale3D().X > FX_Orig3DScale) {
			TransitionState -= TransitionDep_MAT * DeltaTime;
			if (TransitionState < FX_Orig3DScale)
				TransitionState = FX_Orig3DScale;

			PS_fx->SetRelativeScale3D(FVector(TransitionState, TransitionState, TransitionState));
		}
	}
}

void AChessPlaceToMove::SetEmptyColor()
{
	PS_fx->SetVectorParameter(FName("color"), emptyCellColor);
}

void AChessPlaceToMove::SetEnemyColor()
{
	PS_fx->SetVectorParameter(FName("color"), enemyCellColor);
	BoxCollisionEnemy->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//BoxCollisionEnemy->SetGenerateOverlapEvents(true);
}

void AChessPlaceToMove::Event_OnMouseBeginOver()//(UPrimitiveComponent* TouchedComponent)
{
	bMouseOver = true;
	//PS_fx->SetRelativeScale3D(FVector(.75f, .75f, .75f));

}

void AChessPlaceToMove::Event_OnMouseEndOver()//(UPrimitiveComponent* TouchedComponent)
{
	bMouseOver = false;
	//PS_fx->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));
}

//void AChessPlaceToMove::SetCHOriginColor(EChessColor NewColor)
//{
//	CHOriginColor = NewColor;
//}

//void AChessPlaceToMove::Event_OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	PS_fx->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));
//
//}


