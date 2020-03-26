// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessMaster.h"
#include "Engine.h"
#include "CoreMinimal.h"


// Sets default values
AChessMaster::AChessMaster()
{
	PrimaryActorTick.bCanEverTick = true;
	ChessComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Component"));
	SetRootComponent(ChessComp);


	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Loaded_NSystem(TEXT("NiagaraSystem'/Game/Effects/NiagaraYT/DeanNiagaraSystem3.DeanNiagaraSystem3'"));

	if (Loaded_NSystem.Succeeded()) {
		NSystem = Loaded_NSystem.Object;
	}

	//this->OnClicked.AddUniqueDynamic(this, &AChessMaster::OnSelected);
	//ChessComp->OnBeginCursorOver.AddDynamic(this, &AChessMaster::OnHoverBegin);
	//ChessComp->OnEndCursorOver.AddDynamic(this, &AChessMaster::OnHoverEnd);

	
	//MeshComp->OnClicked.AddDynamic(this, &AChessMaster::OnSelected);
}

// Called when the game starts or when spawned
void AChessMaster::BeginPlay()
{
	Super::BeginPlay();

	ChessComp->SetStaticMesh(ChessMeshCH);
	ChessComp->SetMaterial(0, ChessMICH);

	BlendMID = ChessComp->CreateDynamicMaterialInstance(0);

	BlendMID->GetScalarParameterValue(FMaterialParameterInfo("Blend", EMaterialParameterAssociation::GlobalParameter, 0), BlendState, false);
}

void AChessMaster::DelayedDestroy()
{
	Destroy();
}


void AChessMaster::TimeToMoveFinished()
{
	Alpha_MoveTo = 0;
}

// Called every frame
void AChessMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Color Blending
	if (bIsHovering == true && bIsTransitionEnabled == true) {
		if (BlendState < 1.0f) {
			BlendState += TransitionDep_MAT * DeltaTime;
			if (BlendState > 1.0f)
				BlendState = 1.0f;
			if (BlendMID != nullptr) {
				BlendMID->SetScalarParameterValue(FName("Blend"), BlendState);
			}
		}
	}
	else if (bIsHovering == false && bIsTransitionEnabled == true && bIsSelected == false)
	{
		if (BlendState > 0.0f) {
			BlendState -= TransitionDep_MAT * DeltaTime;
			if (BlendState < 0.0f)
				BlendState = 0.0f;
			if (BlendMID != nullptr) {
				BlendMID->SetScalarParameterValue(FName("Blend"), BlendState);
			}
		}
	}
	else if (bIsSelected == true)
	{
		if (BlendState < 1.0f) {
			BlendState += TransitionDep_MAT * DeltaTime;
			if (BlendState > 1.0f)
				BlendState = 1.0f;
			if (BlendMID != nullptr) {
				BlendMID->SetScalarParameterValue(FName("Blend"), BlendState);
			}
		}
	}

	//Moving transformation
	if (bIsTimeToMove) {
		if (bIsTimeToMoveDoOnce) {
			FMoveFrom = GetActorLocation();
			bIsTimeToMoveDoOnce = false;
		}
		
		Alpha_MoveTo += (1.0f/MovingDurationSec) * DeltaTime;

		if (Alpha_MoveTo > 1) {
			Alpha_MoveTo = 1;
			bIsTimeToMove = false;
			bIsTimeToMoveDoOnce = true;
			bFlyLerpTrigger = !bFlyLerpTrigger;
		}

		FVector NewLocation = FMath::Lerp(FMoveFrom, FMoveTo, Alpha_MoveTo);

		if (bIsFlyMoving) {

			// Arc curve 
			// @Alpha_MoveTo - x;
			// @FlyHeight - h (y max value);
			// @return flyLerp - interpolated height
			float flyLerp = FlyHeight * sin(PI * Alpha_MoveTo);

			SetActorLocation(NewLocation + FVector(0,0, flyLerp));
		}
		else {
			SetActorLocation(NewLocation);
		}

 		if (bIsTimeToMoveDoOnce) {
			TimeToMoveFinished();
		}
	}
}

void AChessMaster::DestroyChess() {
	if (NSystem) {
		//UNiagaraFunctionLibrary::SpawnSystemAttached(NSystem, ChessComp, FName("None"), GetActorLocation(), GetActorRotation(), GetActorScale3D(), EAttachLocation::KeepWorldPosition, false, EPSCPoolMethod::None);
		//UNiagaraFunctionLibrary::SpawnSystemAttached(NSystem, ChessComp, FName("None"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false, true,EPSCPoolMethod::None);
		UNiagaraFunctionLibrary::SpawnSystemAttached(NSystem, ChessComp, FName("None"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
	}
	//UNiagaraComponent* UNiagaraFunctionLibrary::SpawnSystemAttached(UNiagaraSystem* SystemTemplate, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy, bool bAutoActivate, ENCPoolMethod PoolingMethod)
	ChessComp->SetHiddenInGame(true);
	ChessComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//TODO: update score;
	UWorld* World = GetWorld();
	if (World) {
		World->GetTimerManager().SetTimer(DestoryTimerHandle,this,&AChessMaster::DelayedDestroy, 10, false);
	}
	//Destroy();
}

void AChessMaster::MoveToSelectedPlace(FVector NewLocation) {
	UWorld* World = GetWorld();
	
	if (!World)
		return ;

	FMoveTo = NewLocation;

	FHitResult HitResult1(EForceInit::ForceInit);
	FHitResult HitResult2(EForceInit::ForceInit);
	FHitResult HitResult3(EForceInit::ForceInit);
	FVector Start = GetActorLocation() + FVector(0, 0, 5);
	FVector End = FMoveTo + FVector(0, 0, 5);
	World->LineTraceSingleByChannel(HitResult1, Start, End, ECollisionChannel::ECC_WorldDynamic);
	World->LineTraceSingleByChannel(HitResult1, Start, End, ECollisionChannel::ECC_WorldDynamic);
	World->LineTraceSingleByChannel(HitResult1, Start, End, ECollisionChannel::ECC_WorldDynamic);
	//
	//DrawDebugLine(World, Start, End, FColor::Purple, false, 3.f);
	//DrawDebugSphere(World, Start, 3.f, 1, FColor::Purple, false, 3.f);
	//DrawDebugSphere(World, End, 3.f, 1, FColor::Purple, false, 3.f);

	FVector testStart;
	FVector testEnd;
	
	FVector norm = testEnd - testStart;
	norm.Normalize();

	testStart = GetActorLocation() +
		norm * 15.0f + FVector(0, 0, 5);
	testEnd = FMoveTo +
		norm* 15.0f + FVector(0, 0, 5);

	//DrawDebugLine(World, testStart, testEnd, FColor::Yellow, false, 10.f);
	//DrawDebugSphere(World, testStart, 3.f, 1, FColor::Cyan, false, 10.f);
	//DrawDebugSphere(World, testEnd, 3.f, 1, FColor::Red, false, 10.f);


	testStart = GetActorLocation() -
		norm * 15.0f + FVector(0, 0, 5);
	testEnd = FMoveTo -
		norm * 15.0f + FVector(0, 0, 5);

	//DrawDebugLine(World, testStart, testEnd, FColor::Yellow, false, 10.f);
	//DrawDebugSphere(World, testStart, 3.f, 1, FColor::Cyan, false, 10.f);
	//DrawDebugSphere(World, testEnd, 3.f, 1, FColor::Red, false, 10.f);


	//DEBUGMESSAGE("HELLx5 ?");
	//if nothing were hit - proceed normal moving
	if (HitResult1.GetActor() == nullptr && HitResult2.GetActor() == nullptr && HitResult3.GetActor() == nullptr) {
		bIsTimeToMove = true;
		bIsFlyMoving = false;
	}
	else {
		bIsTimeToMove = true;
		bIsFlyMoving = true;
	}

	//SetActorLocation(NewLocation);
}

bool AChessMaster::GetbIsKing()
{
	return bIsKing;
}

// Called to bind functionality to input
void AChessMaster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void AChessMaster::OnSelected() {
	bIsSelected = true;
}

void AChessMaster::OnUnSelected() {
	bIsSelected = false;

	for (AChessPlaceToMove* obj : LPositionsToMove) {
		if (IsValid(obj)) {
			obj->Destroy();
		}
	}
	
	LPositionsToMove.Empty();
}

void AChessMaster::OnHoverBegin(){//(UPrimitiveComponent* TouchedComponent) {
	bIsHovering = true;
}

void AChessMaster::OnHoverEnd(){//(UPrimitiveComponent* TouchedComponent) {
	bIsHovering = false;
}

