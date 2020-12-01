// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Tile.h"
#include "Unit.h"
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
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
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
	if (selectedActor->GetClass() == AUnit::StaticClass())
	{
		AUnit* HitUnit = Cast<AUnit>(selectedActor);
		ShowPlayerUnitMovementRange(HitUnit);
		CurrentUnit = HitUnit;
		Director->SetActorLocation(CurrentUnit->GetActorLocation());
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bPlayerTurn)
		return;
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
	if (!Path.empty())
	{
		ATile* currentTile = Path.back();
		UnitHeading = (currentTile->GetActorLocation() - CurrentUnit->GetActorLocation()) / UnitMovingVelocity;
		UnitHeading[2] = 0.0f;
	}
	else
		FinishMoving();
}
void APlayerPawn::FollowHeading()
{
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

	// Remove this later...
	CurrentUnit = nullptr;
}
// just a helper that finds the largest distance stored in each Tile
bool largestDistance(ATile* tile1, ATile* tile2)
{
	return tile1->Distance > tile2->Distance;
}
// uses Dijkistras algo to find all tiles in movement range of Unit
// and set them as selectable
void APlayerPawn::ShowPlayerUnitMovementRange(AUnit* Unit)
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
			for (int i = 0; i < 4; i++)
			{
				ATile* neighbor = currentTile->GetAdjList()[i];
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


