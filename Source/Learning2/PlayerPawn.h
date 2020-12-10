// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tile.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"

#include "CameraDirector.h"
#include "MovementManager.h"
#include "ui/InGameHUD.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class LEARNING2_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();
	~APlayerPawn();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TraceForTile(const FVector& Start, const FVector& End);
	void SelectActor(AActor* selectedActor);
	void UnitWait();
private:
	bool bUnitMoving = false;
	MovementManager* MyMovementManager;
	
	bool bPlayerTurn = true;
	void StartPlayerTurn();
	void EndPlayerTurn();
	void StartEnemyTurn();
	void EndEnemyTurn();

	ACameraDirector* Director;
	AInGameHUD* HUD;
	ATile* CurrentTileFocus;
	AUnit* CurrentUnit;
	std::vector<APlayerUnit*> PlayerUnits;
	std::vector<AEnemyUnit*> EnemyUnits;
	std::vector<ATile*> Tiles;
};
