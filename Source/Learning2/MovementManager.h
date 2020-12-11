// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tile.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"
#include "CameraDirector.h"
#include "CoreMinimal.h"

/**
 * 
 */
class LEARNING2_API MovementManager
{
public:
	MovementManager() = delete;
	~MovementManager();

	MovementManager(std::vector<ATile*> tiles, ACameraDirector* director);

public:
	void ShowPlayerUnitMovementRange(APlayerUnit* unit);
	void StartMoving(ATile* tile);
	void FollowHeading();
	void FindOpponent(AUnit* unit);

	bool bUnitMoving = false;
	AUnit* CurrentUnit;

private:
	void CalculateHeading();
	void FinishMoving();
	void ResetTiles();

	// Adds Tile to path in a loop until tile == nullptr
	void AddToPathFromTile(ATile* tile);

	float UnitMovingVelocity = 40.0f;
	FVector UnitHeading;
	
	
	std::vector<ATile*> Tiles;
	// any tile that has its distance or parent changed must go here
	// to save from reseting every tile each time tiles must be reset
	std::vector<ATile*> TouchedTiles;
	std::vector<ATile*> Path;

	ACameraDirector* Director;
};
