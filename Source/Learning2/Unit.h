// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tile.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"

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
	void FindCurrentTile();
	ATile* CurrentTile;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ATile* GetCurrentTile();
	uint32_t GetMovement();
};
