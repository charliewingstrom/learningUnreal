// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Unit.h"
#include "Tile.h"
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
	
private:
	bool bUnitMoving = false;
	float UnitMovingVelocity = 40.0f;
	FVector UnitHeading;
	void CalculateHeading();
	void FollowHeading();

	void ShowUnitMovementRange(AUnit* unit);
	ATile* CurrentTileFocus;
	AUnit* CurrentUnit;
	std::vector<ATile*> Tiles;
	std::vector<ATile*> SelectableTiles;
	std::vector<ATile*> Path;

};
