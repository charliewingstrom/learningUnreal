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
		ShowUnitMovementRange(HitUnit);
		CurrentUnit = HitUnit;
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		bUnitMoving = false;
}

void APlayerPawn::FollowHeading()
{
	CurrentUnit->SetActorLocation(CurrentUnit->GetActorLocation() + UnitHeading);
	if (CurrentUnit->GetActorLocation()[0] == Path.back()->GetActorLocation()[0] && CurrentUnit->GetActorLocation()[1] == Path.back()->GetActorLocation()[1])
	{
		Path.pop_back();
		CalculateHeading();
	}
}
// just a helper that finds the largest distance stored in each Tile
bool largestDistance(ATile* tile1, ATile* tile2)
{
	return tile1->Distance > tile2->Distance;
}
// uses Dijkistras algo to find all tiles in movement range of Unit
// and set them as selectable
void APlayerPawn::ShowUnitMovementRange(AUnit* Unit)
{
	Unit->GetCurrentTile()->Distance = 0;
	ATile* currentTile;
	while (!Tiles.empty())
	{
		sort(Tiles.begin(), Tiles.end(), largestDistance);
		currentTile = Tiles.back();
		Tiles.pop_back();
		if (currentTile->Distance <= Unit->GetMovement())
		{
			currentTile->SetSelected();
			SelectableTiles.push_back(currentTile);
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


