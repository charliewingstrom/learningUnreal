// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

#define MAX_DISTANCE 256

UCLASS()
class LEARNING2_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
	
	std::vector<ATile*> GetAdjList();
	
	void Highlight(bool On);
	void SetSelected();
	void Reset();

	bool PlayerOccupied = false;
	bool EnemyOccupied = false;

	// for dijsktras or BFS
	ATile* Parent;
	uint32_t Distance = MAX_DISTANCE;
	uint32_t AttackDistance = 0;
	bool Visited = false;
	bool Selectable = false;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere)
		int MovementPenalty = 1;

	
private:
	UMaterial* DefaultMaterial;
	UMaterial* HighlightMaterial;
	UMaterial* SelectableMaterial;
	
	std::vector<ATile*> AdjList;

	void FindAdjList();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
