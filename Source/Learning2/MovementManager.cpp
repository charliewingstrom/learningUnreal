// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementManager.h"

MovementManager::~MovementManager()
{
}
MovementManager::MovementManager(std::vector<ATile*> tiles, ACameraDirector* director)
{
	UnitHeading = FVector();
	CurrentUnit = nullptr;
	Tiles = tiles;
	Path = std::vector<ATile*>();
	Director = director;
}
// just a helper that finds the largest distance stored in each Tile
bool largestDistance(ATile* tile1, ATile* tile2)
{
	return tile1->Distance > tile2->Distance;
}

void MovementManager::ShowPlayerUnitMovementRange(APlayerUnit* unit)
{
	ATile* firstTile = unit->GetCurrentTile();
	firstTile->Distance = 0;
	TouchedTiles.push_back(firstTile);
	unit->GetCurrentTile()->Distance = 0;
	ATile* currentTile;
	std::vector<ATile*> tiles = Tiles;
	while (!tiles.empty())
	{
		sort(tiles.begin(), tiles.end(), largestDistance);
		currentTile = tiles.back();
		tiles.pop_back();
		if (currentTile->Distance <= unit->GetMovement())
		{
			currentTile->SetSelected();
			currentTile->Visited = true;
			for (ATile* neighbor : currentTile->GetAdjList())
			{
				if (neighbor != nullptr && !neighbor->Visited && !neighbor->EnemyOccupied)
				{
					uint32_t tmpDistance = currentTile->Distance + neighbor->MovementPenalty;
					if (neighbor->Distance > tmpDistance)
					{
						neighbor->Distance = tmpDistance;
						neighbor->Parent = currentTile;
						TouchedTiles.push_back(neighbor);
					}
				}
			}
		}
		else
			break;
	}
}

void MovementManager::StartMoving(ATile* tile)
{
	bUnitMoving = true;
	AddToPathFromTile(tile);
	CalculateHeading();
}

void MovementManager::CalculateHeading()
{
	if (!Path.empty() && Path.back()->Distance <= CurrentUnit->GetMovement())
	{
		ATile* currentTile = Path.back();
		UnitHeading = (currentTile->GetActorLocation() - CurrentUnit->GetActorLocation()) / UnitMovingVelocity;
		UnitHeading[2] = 0.0f;
	}
	else
		FinishMoving();
}

void MovementManager::FollowHeading()
{
	CurrentUnit->SetActorLocation(CurrentUnit->GetActorLocation() + UnitHeading);
	Director->SetActorLocation(CurrentUnit->GetActorLocation());
	if (CurrentUnit->GetActorLocation()[0] == Path.back()->GetActorLocation()[0] && CurrentUnit->GetActorLocation()[1] == Path.back()->GetActorLocation()[1])
	{
		Path.pop_back();
		CalculateHeading();
	}
}

AUnit* MovementManager::FindOpponent(AUnit* unit)
{
	CurrentUnit = unit;
	ATile* firstTile = CurrentUnit->GetCurrentTile();
	firstTile->Distance = 0;
	TouchedTiles.push_back(firstTile);

	std::vector<ATile*> tiles = Tiles;
	ATile* currentTile = nullptr;

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
					TouchedTiles.push_back(neighbor);
				}
				if (neighbor->PlayerOccupied)
					break;
			}
		}
		if (currentTile->PlayerOccupied)
			break;
	}
	StartMoving(currentTile->Parent);
	return currentTile->GetCurrentUnit();
}

void MovementManager::FinishMoving()
{
	bUnitMoving = false;
	CurrentUnit->SetPreviousTile();
	CurrentUnit->FindCurrentTile();
	ResetTiles();
	Path = std::vector<ATile*>();
	// Remove this later...
	CurrentUnit = nullptr;
}

void MovementManager::ResetTiles()
{
	while (!TouchedTiles.empty())
	{
		ATile* currentTile = TouchedTiles.back();
		currentTile->Reset();
		TouchedTiles.pop_back();
	}
}

void MovementManager::AddToPathFromTile(ATile* tile)
{
	while (tile != nullptr)
	{
		Path.push_back(tile);
		tile = tile->Parent;
	}
}


