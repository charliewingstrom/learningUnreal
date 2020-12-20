// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TArray<AActor*> tiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATile::StaticClass(), tiles);
	for (AActor* tile : tiles)
		Tiles.push_back(Cast<ATile>(tile));
	
	TArray<AActor*> playerUnits;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), playerUnits);
	for (AActor* player : playerUnits)
		PlayerUnits.push_back(Cast<APlayerUnit>(player));

	Director = Cast<ACameraDirector>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraDirector::StaticClass()));
	MyMovementManager = new MovementManager(Tiles, Director);
	MyCombatManager = new CombatManager();
	HUD = Cast<AInGameHUD>(UGameplayStatics::GetActorOfClass(GetWorld(), AInGameHUD::StaticClass()));
}

APlayerPawn::~APlayerPawn()
{
	delete MyMovementManager;
	delete MyCombatManager;
}
// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	bPlayerTurn = true;
}

void APlayerPawn::TraceForTile(const FVector& Start, const FVector& End)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (HitResult.Actor.IsValid())
	{
		ATile* HitTile = Cast<ATile>(HitResult.Actor.Get());
		if (CurrentTileFocus != HitTile)
		{
			if (CurrentTileFocus)
				CurrentTileFocus->Highlight(false);
			if (HitTile)
				HitTile->Highlight(true);
			CurrentTileFocus = HitTile;
		}
	}
	else if (CurrentTileFocus)
	{
		CurrentTileFocus->Highlight(false);
		CurrentTileFocus = nullptr;
	}
}

void APlayerPawn::SelectActor(AActor* selectedActor)
{
	if (CurrentUnit != nullptr)
	{
		// Unit is selected and set to move to selected Tile
		if (selectedActor->GetClass() == ATile::StaticClass() && Cast<ATile>(selectedActor)->Selectable)
		{
			ATile* currentTile = Cast<ATile>(selectedActor);
			MyMovementManager->StartMoving(currentTile);
			bSelectingAction = true;
		}
	}
	// unit is being selected
	else if (selectedActor->GetClass() == APlayerUnit::StaticClass())
	{
		APlayerUnit* HitUnit = Cast<APlayerUnit>(selectedActor);
		MyMovementManager->ShowPlayerUnitMovementRange(HitUnit);
		MyMovementManager->CurrentUnit = HitUnit;
		CurrentUnit = HitUnit;
		Director->SetActorLocation(CurrentUnit->GetActorLocation());
	}
}
void APlayerPawn::UnitWait()
{
	if (CurrentUnit != nullptr)
	{
		CurrentUnit->FinishAction();
		CurrentUnit = nullptr;
		int ActiveUnits = 0;
		for (APlayerUnit* unit : PlayerUnits)
		{
			if (unit->Active)
				ActiveUnits++;
		}
		if (ActiveUnits == 0)
		{
			EndPlayerTurn();
			StartEnemyTurn();
		}
			
	}
}
// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bPlayerTurn && !MyMovementManager->bUnitMoving)
	{
		// for each enemy unit, find the path to the closest player unit
		// move as far as possible on that path
		if (!EnemyUnits.empty())
		{
			CurrentUnit = EnemyUnits.back();
			EnemyUnits.pop_back();
			MyMovementManager->FindOpponent(CurrentUnit);

			// remove this later...
			CurrentUnit = nullptr;
		}
		else
		{
			EndEnemyTurn();
			StartPlayerTurn();
		}
	}
		
	if (MyMovementManager->bUnitMoving)
		MyMovementManager->FollowHeading();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FVector Start, Dir, End;
		PC->DeprojectMousePositionToWorld(Start, Dir);
		End = Start + (Dir * 8000.0f);
		TraceForTile(Start, End);
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerPawn::StartPlayerTurn()
{
	bPlayerTurn = true;
	HUD->ShowPlayerPhaseText();
	for (AUnit* unit : PlayerUnits)
		unit->Active = true;
}

void APlayerPawn::EndPlayerTurn()
{
	HUD->ShowPlayerPhaseText();
}

void APlayerPawn::StartEnemyTurn()
{
	bPlayerTurn = false;
	TArray<AActor*> enemyUnits;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyUnit::StaticClass(), enemyUnits);
	for (AActor* enemy : enemyUnits)
		EnemyUnits.push_back(Cast<AEnemyUnit>(enemy));

	HUD->ShowEnemyPhaseText();
}

void APlayerPawn::EndEnemyTurn()
{
	HUD->ShowEnemyPhaseText();
}

void APlayerPawn::StartCombat()
{
	if (bSelectingTarget)
	{
		MyCombatManager->InitiateAttack();
		bSelectingAction = false;
		bSelectingTarget = false;
		UnitWait();
	}
	else if (CurrentUnit != nullptr && bSelectingAction && !bUnitMoving)
	{
		MyCombatManager->StartCombat(CurrentUnit);
		bSelectingTarget = true;
	}
}

void APlayerPawn::ResetCurrentUnit()
{
	if (CurrentUnit != nullptr)
	{
		ATile* prev = CurrentUnit->GetPreviousTile();
		if (prev != nullptr)
		{
			FVector prevLocation = prev->GetActorLocation();
			CurrentUnit->SetActorLocation(FVector(prevLocation[0], prevLocation[1], prevLocation[2] + 10.0f));
			CurrentUnit->FindCurrentTile();
			CurrentUnit = nullptr;
			bSelectingAction = false;
			bSelectingTarget = false;
		}
	}
}


