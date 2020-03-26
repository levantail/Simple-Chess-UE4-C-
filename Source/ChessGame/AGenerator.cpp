// Fill out your copyright notice in the Description page of Project Settings.


#include "AGenerator.h"


// Sets default values

AAGenerator::AAGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));

	this->amountX = 8;
	this->amountY = 8;
	this->offset = 0;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Loaded_Board(TEXT("StaticMesh'/Game/Meshes/SM_Board.SM_Board'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Loaded_BoardMI1(TEXT("MaterialInstanceConstant'/Game/Materials/MI_DarkBoard.MI_DarkBoard'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Loaded_BoardMI2(TEXT("MaterialInstanceConstant'/Game/Materials/MI_WhiteBoard.MI_WhiteBoard'"));

	//static ConstructorHelpers::FObjectFinder<UMaterial> Loaded_test1(TEXT("MaterialInstanceConstant'/Game/Materials/MI_DarkBoard.MI_DarkBoard'"));
	//static ConstructorHelpers::FObjectFinder<UMaterial> Loaded_test2(TEXT("MaterialInstanceConstant'/Game/Materials/MI_WhiteBoard.MI_WhiteBoard'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Loaded_ChessMI1(TEXT("MaterialInstanceConstant'/Game/Materials/MI_DarkChess.MI_DarkChess'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Loaded_ChessMI2(TEXT("MaterialInstanceConstant'/Game/Materials/MI_WhiteChess.MI_WhiteChess'"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Loaded_ChessPawn(TEXT("StaticMesh'/Game/Meshes/SM_Pawn.SM_Pawn'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Loaded_ChessKnight(TEXT("StaticMesh'/Game/Meshes/SM_Knight.SM_Knight'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Loaded_ChessRook(TEXT("StaticMesh'/Game/Meshes/SM_Rook.SM_Rook'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Loaded_ChessBishop(TEXT("StaticMesh'/Game/Meshes/SM_Bishop.SM_Bishop'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Loaded_ChessQueen(TEXT("StaticMesh'/Game/Meshes/SM_Queen.SM_Queen'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Loaded_ChessKing(TEXT("StaticMesh'/Game/Meshes/SM_King.SM_King'"));



	
	if (Loaded_Board.Succeeded()) { SM_ChessBoard = Loaded_Board.Object; }

	if (Loaded_BoardMI1.Succeeded()) { MAT_DarkBoard = Loaded_BoardMI1.Object; }
	if (Loaded_BoardMI2.Succeeded()) { MAT_WhiteBoard = Loaded_BoardMI2.Object; }

	if (Loaded_ChessMI1.Succeeded()) { MAT_DarkChess = Loaded_ChessMI1.Object; }
	if (Loaded_ChessMI2.Succeeded()) { MAT_WhiteChess = Loaded_ChessMI2.Object; }


	if (Loaded_ChessPawn.Succeeded()) { SM_ChessPawn = Loaded_ChessPawn.Object; }
	if (Loaded_ChessKnight.Succeeded()) { SM_ChessKnight = Loaded_ChessKnight.Object; }
	if (Loaded_ChessRook.Succeeded()) { SM_ChessRook = Loaded_ChessRook.Object; }
	if (Loaded_ChessBishop.Succeeded()) { SM_ChessBishop = Loaded_ChessBishop.Object; }
	if (Loaded_ChessQueen.Succeeded()) { SM_ChessQueen = Loaded_ChessQueen.Object; }
	if (Loaded_ChessKing.Succeeded()) { SM_ChessKing = Loaded_ChessKing.Object; }


	if (
		SM_ChessBoard == nullptr ||

		MAT_DarkBoard == nullptr ||
		MAT_WhiteBoard == nullptr ||

		MAT_DarkChess == nullptr ||
		MAT_WhiteChess == nullptr ||

		SM_ChessPawn == nullptr ||
		SM_ChessKnight == nullptr ||
		SM_ChessRook == nullptr ||
		SM_ChessBishop == nullptr ||
		SM_ChessQueen == nullptr ||
		SM_ChessKing == nullptr
		)
	{
#if WITH_EDITOR
		DEBUGMESSAGE("Something wrong with chess meshes paths. Check LOG for details.");
#endif

		UE_LOG(LogTemp, Warning, TEXT("Something wrong with some default chess meshes path."));
		UE_LOG(LogTemp, Warning, TEXT("Default path is: '/Game/Meshes/SM_Rook.SM_Rook'."));
		UE_LOG(LogTemp, Warning, TEXT("You need to pass proper meshes to AGenerator manualy to generate board whith chess."));
	}

	CharacterPos.SetLocation(FVector(0.f, 0.f, 30.f));
	CharacterPos.SetRotation(FRotator(320.f, 0.f, 0.f).Quaternion());
	CharacterPos.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
}
// Called when the game starts or when spawned
void AAGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateChessBoard();
	GenerateChesses();
	SpawnCharacter();

}

void AAGenerator::SpawnCharacter()
{
	UWorld* World = GetWorld();

	if (World) {
		FTransform SpawnTransform;

		SpawnTransform.SetLocation((V_BottomLeft + (V_TopRight - V_BottomLeft)/2) + CharacterPos.GetLocation());
		SpawnTransform.SetRotation(CharacterPos.GetRotation());
		SpawnTransform.SetScale3D(CharacterPos.GetScale3D());

		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		PController->GetPawn()->SetActorTransform(SpawnTransform);
		PController->SetControlRotation(SpawnTransform.GetRotation().Rotator().Clamp());
		//AMainCharacter* NewActor = World->SpawnActorDeferred<AMainCharacter>(AMainCharacter::StaticClass(), SpawnTransform);
		//NewActor->SetV_BottomLeft(V_BottomLeft);
		//NewActor->SetV_TopRight(V_TopRight);
		//NewActor->FinishSpawning(SpawnTransform);

		//World->GetFirstPlayerController()->UnPossess();
		//World->GetFirstPlayerController()->Possess(NewActor);
		//pawn->Destroy();
	}
}

void AAGenerator::GenerateChessBoard() {
	UWorld* World = GetWorld();
	FTransform SpawnTransform;
	FVector BoundBox = SM_ChessBoard->GetBoundingBox().GetSize();

	bool bTrigger = false;
	for (int x = 0; x < amountX; ++x) {
		if (amountX % 2 == 0) {
			bTrigger = !bTrigger;
		}
		for (int y = 0; y < amountY; ++y) {
			SpawnTransform.SetLocation(FVector(BoundBox.X * x + offset, BoundBox.Y * y + offset, 0.0) + GetActorLocation());
			SpawnTransform.SetRotation(FQuat(0, 0, 0, 0));
			SpawnTransform.SetScale3D(FVector(1, 1, 1));
			if (y == 0 && x == 0) {
				V_BottomLeft = FVector(SpawnTransform.GetLocation().X - BoundBox.X / 2, SpawnTransform.GetLocation().Y - BoundBox.X / 2, SpawnTransform.GetLocation().Z + BoundBox.Z);
			}else if (y == amountY - 1 && x == amountX - 1)
			{
				V_TopRight = FVector(SpawnTransform.GetLocation().X + BoundBox.X/2, SpawnTransform.GetLocation().Y + BoundBox.Y/2, SpawnTransform.GetLocation().Z + BoundBox.Z);
			}

			AABoardCell* NewActor = World->SpawnActorDeferred<AABoardCell>(AABoardCell::StaticClass(), SpawnTransform);
			NewActor->SetBoardMesh(SM_ChessBoard);
			if (bTrigger == true){
				NewActor->SetBoardMI(MAT_DarkBoard);
				bTrigger = !bTrigger;
			}
			else {
				NewActor->SetBoardMI(MAT_WhiteBoard);
				bTrigger = !bTrigger;
			}

			NewActor->FinishSpawning(SpawnTransform);
			NewActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void AAGenerator::GenerateChesses() {

	int y;

	//BLACK
	y = 0;
	GenerateChessByType(0, y, EChessType::CE_ROOK, EChessColor::CE_WHITE);
	GenerateChessByType(1, y, EChessType::CE_KNIGHT, EChessColor::CE_WHITE);
	GenerateChessByType(2, y, EChessType::CE_BISHOP, EChessColor::CE_WHITE);
	GenerateChessByType(3, y, EChessType::CE_QUEEN, EChessColor::CE_WHITE);
	GenerateChessByType(4, y, EChessType::CE_KING, EChessColor::CE_WHITE);
	GenerateChessByType(5, y, EChessType::CE_BISHOP, EChessColor::CE_WHITE);
	GenerateChessByType(6, y, EChessType::CE_KNIGHT, EChessColor::CE_WHITE);
	GenerateChessByType(7, y, EChessType::CE_ROOK, EChessColor::CE_WHITE);

	y = 1;
	GenerateChessByType(0, y, EChessType::CE_PAWN, EChessColor::CE_WHITE);
	GenerateChessByType(1, y, EChessType::CE_PAWN, EChessColor::CE_WHITE);
	GenerateChessByType(2, y, EChessType::CE_PAWN, EChessColor::CE_WHITE);
	GenerateChessByType(3, y, EChessType::CE_PAWN, EChessColor::CE_WHITE);
	GenerateChessByType(4, y, EChessType::CE_PAWN, EChessColor::CE_WHITE);
	GenerateChessByType(5, y, EChessType::CE_PAWN, EChessColor::CE_WHITE);
	GenerateChessByType(6, y, EChessType::CE_PAWN, EChessColor::CE_WHITE);
	GenerateChessByType(7, y, EChessType::CE_PAWN, EChessColor::CE_WHITE);

	//WHITE
	y = 7;
	GenerateChessByType(0, y, EChessType::CE_ROOK, EChessColor::CE_DARK);
	GenerateChessByType(1, y, EChessType::CE_KNIGHT, EChessColor::CE_DARK);
	GenerateChessByType(2, y, EChessType::CE_BISHOP, EChessColor::CE_DARK);
	GenerateChessByType(3, y, EChessType::CE_QUEEN, EChessColor::CE_DARK);
	GenerateChessByType(4, y, EChessType::CE_KING, EChessColor::CE_DARK);
	GenerateChessByType(5, y, EChessType::CE_BISHOP, EChessColor::CE_DARK);
	GenerateChessByType(6, y, EChessType::CE_KNIGHT, EChessColor::CE_DARK);
	GenerateChessByType(7, y, EChessType::CE_ROOK, EChessColor::CE_DARK);

	y = 6;
	GenerateChessByType(0, y, EChessType::CE_PAWN, EChessColor::CE_DARK);
	GenerateChessByType(1, y, EChessType::CE_PAWN, EChessColor::CE_DARK);
	GenerateChessByType(2, y, EChessType::CE_PAWN, EChessColor::CE_DARK);
	GenerateChessByType(3, y, EChessType::CE_PAWN, EChessColor::CE_DARK);
	GenerateChessByType(4, y, EChessType::CE_PAWN, EChessColor::CE_DARK);
	GenerateChessByType(5, y, EChessType::CE_PAWN, EChessColor::CE_DARK);
	GenerateChessByType(6, y, EChessType::CE_PAWN, EChessColor::CE_DARK);
	GenerateChessByType(7, y, EChessType::CE_PAWN, EChessColor::CE_DARK);

}



void AAGenerator::GenerateChessByType(int ChessPosX, int ChessPosY, EChessType ChessType, EChessColor ChessColor)
{
	UWorld* World = GetWorld();

	FVector BoundBox = SM_ChessBoard->GetBoundingBox().GetSize();
	FTransform SpawnTransform;
	UMaterialInterface* ChessMI;

	SpawnTransform.SetLocation(FVector(BoundBox.X * ChessPosY + offset, BoundBox.Y * ChessPosX + offset, BoundBox.Z) + GetActorLocation());
	SpawnTransform.SetScale3D(FVector(1, 1, 1));

	if (EChessColor::CE_DARK == ChessColor) {
		SpawnTransform.SetRotation(FRotator(0, 180, 0).Quaternion());
		ChessMI = MAT_DarkChess;
	}
	else {
		SpawnTransform.SetRotation(FRotator(0, 0, 0).Quaternion());
		ChessMI = MAT_WhiteChess;
	}

	if (ChessType == EChessType::CE_KNIGHT) {
		AChessKnight* NewChess = World->SpawnActorDeferred<AChessKnight>(AChessKnight::StaticClass(), SpawnTransform);
		NewChess->SetChessMeshCH(SM_ChessKnight);
		NewChess->SetChessMICH(ChessMI);
		NewChess->SetChessType(ChessType);
		//NewChess->SetXPos(ChessPosX);
		//NewChess->SetYPos(ChessPosY);
		NewChess->SetChessColor(ChessColor);
		NewChess->SetBoardStep(BoundBox.X);
		NewChess->FinishSpawning(SpawnTransform);
		NewChess->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
	else if (ChessType == EChessType::CE_ROOK) {
		AChessRook* NewChess = World->SpawnActorDeferred<AChessRook>(AChessRook::StaticClass(), SpawnTransform);
		NewChess->SetChessMeshCH(SM_ChessRook);
		NewChess->SetChessMICH(ChessMI);
		NewChess->SetChessType(ChessType);
		NewChess->SetChessColor(ChessColor);
		//NewChess->SetXPos(ChessPosX);
		//NewChess->SetYPos(ChessPosY);
		NewChess->SetBoardStep(BoundBox.X);
		NewChess->FinishSpawning(SpawnTransform);
		NewChess->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	}
	else if (ChessType == EChessType::CE_BISHOP) {
		AChessBishop* NewChess = World->SpawnActorDeferred<AChessBishop>(AChessBishop::StaticClass(), SpawnTransform);
		NewChess->SetChessMeshCH(SM_ChessBishop);
		NewChess->SetChessMICH(ChessMI);
		NewChess->SetChessType(ChessType);
		NewChess->SetChessColor(ChessColor);
		//NewChess->SetXPos(ChessPosX);
		//NewChess->SetYPos(ChessPosY);
		NewChess->SetBoardStep(BoundBox.X);
		NewChess->FinishSpawning(SpawnTransform);
		NewChess->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	}
	else if (ChessType == EChessType::CE_QUEEN) {
		AChessQueen* NewChess = World->SpawnActorDeferred<AChessQueen>(AChessQueen::StaticClass(), SpawnTransform);
		NewChess->SetChessMeshCH(SM_ChessQueen);
		NewChess->SetChessMICH(ChessMI);
		NewChess->SetChessType(ChessType);
		NewChess->SetChessColor(ChessColor);
		//NewChess->SetXPos(ChessPosX);
		//NewChess->SetYPos(ChessPosY);
		NewChess->SetBoardStep(BoundBox.X);
		NewChess->FinishSpawning(SpawnTransform);
		NewChess->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	}
	else if (ChessType == EChessType::CE_KING) {
		AChessKing* NewChess = World->SpawnActorDeferred<AChessKing>(AChessKing::StaticClass(), SpawnTransform);
		NewChess->SetChessMeshCH(SM_ChessKing);
		NewChess->SetChessMICH(ChessMI);
		NewChess->SetChessType(ChessType);
		NewChess->SetChessColor(ChessColor);
		//NewChess->SetXPos(ChessPosX);
		//NewChess->SetYPos(ChessPosY);
		NewChess->SetBoardStep(BoundBox.X);
		NewChess->FinishSpawning(SpawnTransform);
		NewChess->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	}
	else {
		AChessPawn* NewChess = World->SpawnActorDeferred<AChessPawn>(AChessPawn::StaticClass(), SpawnTransform);
		NewChess->SetChessMeshCH(SM_ChessPawn);
		NewChess->SetQueenProps(SM_ChessQueen);
		NewChess->SetChessMICH(ChessMI);
		NewChess->SetChessType(ChessType);
		NewChess->SetChessColor(ChessColor);
		//NewChess->SetXPos(ChessPosX);
		//NewChess->SetYPos(ChessPosY);
		NewChess->SetBoardStep(BoundBox.X);
		NewChess->FinishSpawning(SpawnTransform);
		NewChess->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	}
}

// Called every frame
void AAGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

