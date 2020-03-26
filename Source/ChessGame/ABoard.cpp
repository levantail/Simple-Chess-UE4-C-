// Fill out your copyright notice in the Description page of Project Settings.
#include "ABoard.h"

#define DEBUGMESSAGE(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}
// Sets default values
AABoard::AABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->amountX = 5;
	this->amountY = 5;
	this->offset = 100.0f;

	this->amountXold = amountX;
	this->amountYold = amountY;
	SM_ChessBoard = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	//TO DO: set default for creation as cube
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));
}

// Called when the game starts or when spawned
void AABoard::BeginPlay()
{
	DEBUGMESSAGE("Hello from begin Play");


	Super::BeginPlay();
	
}
void AABoard::OnConstruction(const FTransform& Transform)
{
	//DEBUGMESSAGE("Hello from constructor!")
	CDeystroyGeneratedMeshes();

	if (amountX == 0 || amountY == 0 || SM_ChessBoard == nullptr) {
		DEBUGMESSAGE("There is nothing to generate!")
		return;
	}

	bool bTrigger = true;

	for (int i = 0; i < amountX; ++i) {
		if (amountY % 2 == 0) {
			bTrigger = !bTrigger;
		}
		for (int j = 0; j < amountY; ++j) {
			UStaticMeshComponent* NewComponent = NewObject<UStaticMeshComponent>(this);
			NewComponent->SetStaticMesh(SM_ChessBoard);
			NewComponent->SetFlags(RF_Transactional);
			NewComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			NewComponent->SetGenerateOverlapEvents(false);
			NewComponent->CreationMethod = EComponentCreationMethod::SimpleConstructionScript;
			NewComponent->SetRelativeLocation(FVector(i * offset, j * offset, 0) + this->GetActorLocation());
			NewComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
			this->AddInstanceComponent(NewComponent);


			if (MAT_DarkBoard == nullptr || MAT_WhiteBoard == nullptr) {
				NewComponent->SetMaterial(0, SM_ChessBoard->GetMaterial(0));
			}
			else
			{
				if (bTrigger) {
					NewComponent->SetMaterial(0, MAT_DarkBoard);
					bTrigger = false;
				}
				else
				{
					NewComponent->SetMaterial(0, MAT_WhiteBoard);
					bTrigger = true;
				}
			}
			
			NewComponent->RegisterComponent();
			Meshes.Add(NewComponent);
		}
	}

}
void AABoard::CDeystroyGeneratedMeshes()
{
	for (UStaticMeshComponent* object : Meshes) {
		object->DestroyComponent();
	}
	Meshes.Empty();
}

// Called every frame
void AABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

