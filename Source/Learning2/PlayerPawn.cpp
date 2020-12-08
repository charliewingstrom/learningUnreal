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
	
	Director = Cast<ACameraDirector>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraDirector::StaticClass()));

	TArray<AActor*> playerUnits;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), playerUnits);
	for (AActor* player : playerUnits)
		PlayerUnits.push_back(Cast<APlayerUnit>(player));
	TArray<AActor*> enemyUnits;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyUnit::StaticClass(), enemyUnits);
	for (AActor* enemy : enemyUnits)
		EnemyUnits.push_back(Cast<AEnemyUnit>(enemy));
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	bPlayerTurn = true;
	UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerUnits.size());
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
		if (selectedActor->GetClass() == ATile::StaticClass() && Cast<ATile>(selectedActor)->Selectable)
		{
			ATile* currentTile = Cast<ATile>(selectedActor);
			while (currentTile != nullptr)
			{
				Path.push_back(currentTile);
				currentTile = currentTile->Parent;
			}
			CalculateHeading();
			bUnitMoving = true;
		}
	}
	else if (selectedActor->GetClass() == APlayerUnit::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("I GOT HERE"));
		APlayerUnit* HitUnit = Cast<APlayerUnit>(selectedActor);
		ShowPlayerUnitMovementRange(HitUnit);
		CurrentUnit = HitUnit;
		Director->SetActorLocation(CurrentUnit->GetActorLocation());
	}
}
// just a helper that finds the largest distance stored in each Tile
bool largestDistance(ATile* tile1, ATile* tile2)
{
	return tile1->Distance > tile2->Distance;
}
// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	if (!bPlayerTurn && !bUnitMoving)
	{

		// for each enemy unit, find the path to the closest player unit
		// move as far as possible on that path
		if (!EnemyUnits.empty())
		{
			UE_LOG(LogTemp, Warning, TEXT("An Enemy is still left"));
			CurrentUnit = EnemyUnits.back();
			EnemyUnits.pop_back();

			// get path to nearest Player Unit
			CurrentUnit->GetCurrentTile()->Distance = 0;
			ATile* currentTile = nullptr;
			std::vector<ATile*> tiles = Tiles;
			while (!tiles.empty())
			{
				sort(tiles.begin(), tiles.end(), largestDistance);
				currentTile = tiles.back();
				tiles.pop_back();
				
				currentTile->Visited = true;
				for (ATile* neighbor : currentTile->GetAdjList())
				{
					if (neighbor != nullptr && !neighbor->Visited)
					{
						uint32_t tmpDistance = currentTile->Distance + neighbor->MovementPenalty;
						if (neighbor->Distance > tmpDistance)
						{
							neighbor->Distance = tmpDistance;
							neighbor->Parent = currentTile;
						}
						if (neighbor->PlayerOccupied)
							break;
					}
				}
				if (currentTile->PlayerOccupied)
					break;
			}
			while (currentTile != nullptr)
			{
				Path.push_back(currentTile);
				currentTile = currentTile->Parent;
			}
			UE_LOG(LogTemp, Warning, TEXT("Path Length : %d"), Path.size());
			CalculateHeading();
			bUnitMoving = true;
		}
		else
		{
			EndEnemyTurn();
		}
	}
		
	if (bUnitMoving)
	{
		FollowHeading();
	}
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
void APlayerPawn::CalculateHeading()
{
	//UE_LOG(LogTemp, Warning, TEXT("Heading Calculated"));
	if (!Path.empty() && Path.back()->Distance <= CurrentUnit->GetMovement())
	{
		ATile* currentTile = Path.back();
		UnitHeading = (currentTile->GetActorLocation() - CurrentUnit->GetActorLocation()) / UnitMovingVelocity;
		//UE_LOG(LogTemp, Warning, TEXT("Heading : %g, %g"), UnitHeading[0], UnitHeading[1]);
		UnitHeading[2] = 0.0f;
	}
	else
		FinishMoving();
}
void APlayerPawn::FollowHeading()
{
	//UE_LOG(LogTemp, Warning, TEXT("Follow Heading Called"));
	CurrentUnit->SetActorLocation(CurrentUnit->GetActorLocation() + UnitHeading);
	Director->SetActorLocation(CurrentUnit->GetActorLocation());
	if (CurrentUnit->GetActorLocation()[0] == Path.back()->GetActorLocation()[0] && CurrentUnit->GetActorLocation()[1] == Path.back()->GetActorLocation()[1])
	{
		Path.pop_back();
		CalculateHeading();
	}
}
void APlayerPawn::FinishMoving()
{
	bUnitMoving = false;
	CurrentUnit->SetPreviousTile();
	CurrentUnit->FindCurrentTile();
	ResetTiles();
	Path = std::vector<ATile*>();
	// Remove this later...
	CurrentUnit->Active = false;
	CurrentUnit = nullptr;
	int ActiveUnits = 0;
	for (APlayerUnit* unit : PlayerUnits)
	{
		if (unit->Active)
			ActiveUnits++;
	}
	if (ActiveUnits == 0)
		EndPlayerTurn();
}

// uses Dijkistras algo to find all tiles in movement range of Unit
// and set them as selectable
void APlayerPawn::ShowPlayerUnitMovementRange(APlayerUnit* Unit)
{
	Unit->GetCurrentTile()->Distance = 0;
	ATile* currentTile;
	std::vector<ATile*> tiles = Tiles;
	while (!tiles.empty())
	{
		sort(tiles.begin(), tiles.end(), largestDistance);
		currentTile = tiles.back();
		tiles.pop_back();
		if (currentTile->Distance <= Unit->GetMovement())
		{
			currentTile->SetSelected();
			currentTile->Visited = true;
			for (ATile* neighbor : currentTile->GetAdjList())
			{
				if (neighbor != nullptr && !neighbor->Visited)
				{
					uint32_t tmpDistance = currentTile->Distance + neighbor->MovementPenalty;
					if (neighbor->Distance > tmpDistance)
					{
						neighbor->Distance = tmpDistance;
						neighbor->Parent = currentTile;
					}
				}
			}
		}
	}
}

void APlayerPawn::ResetTiles()
{
	for (ATile* tile : Tiles)
		tile->Reset();
	
}

void APlayerPawn::EndPlayerTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("End Player Turn Called"));
	bPlayerTurn = false;
	for (AUnit* unit : PlayerUnits)
	{
		unit->Active = true;
	}
}

void APlayerPawn::EndEnemyTurn()
{
	bPlayerTurn = true;
	TArray<AActor*> enemyUnits;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyUnit::StaticClass(), enemyUnits);
	for (AActor* enemy : enemyUnits)
		EnemyUnits.push_back(Cast<AEnemyUnit>(enemy));
}


