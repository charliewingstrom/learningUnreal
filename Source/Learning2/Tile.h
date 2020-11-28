// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UMaterialInstanceDynamic;

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

	// for dijsktras
	uint32_t Distance = 256;
	bool Visited = false;
	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere)
		UMaterialInstanceDynamic* Material;

	UPROPERTY(EditAnywhere)
		int MovementPenalty = 1;
private:
	bool Selectable = false;

	UMaterial* DefaultMaterial;
	UMaterial* HighlightMaterial;
	UMaterial* SelectableMaterial;
	
	std::vector<ATile*> AdjList;
	
	void FindAdjList();
	void Reset();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
