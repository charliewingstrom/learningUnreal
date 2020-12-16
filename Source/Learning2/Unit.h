// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tile.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"

enum Stat 
{
	MaxHp,
	CurrHp,
	Str,
	Def,
	Dex,
	Spd
};
UCLASS()
class LEARNING2_API AUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnit();

	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere)
		UMaterial* Material;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	uint32_t Movement = 5;
	
protected:
	ATile* CurrentTile;
	ATile* PreviousTile;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void FindCurrentTile();
	ATile* GetCurrentTile();
	void SetPreviousTile();

	uint32_t GetMovement();
	uint32_t GetAttackRange();

	int GetStats(Stat stat);

	bool Active = false;
	int MaxHp = 20;
	int CurrHp = MaxHp;
	int Str = 5;
	int Def = 3;
	int Dex = 5;
	int Spd = 5;
	int Luck = 3;
};
