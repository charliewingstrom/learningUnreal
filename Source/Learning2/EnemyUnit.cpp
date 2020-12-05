// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUnit.h"
AEnemyUnit::AEnemyUnit()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> UnitMaterial(TEXT("/Game/CustomMaterials/GenericEnemyMaterial.GenericEnemyMaterial"));
	Material = UnitMaterial.Object;
	VisualMesh->SetMaterial(0, Material);
}
void AEnemyUnit::FindCurrentTile()
{
	if (CurrentTile != nullptr)
		CurrentTile->EnemyOccupied = false;

	FVector currentLocation = GetActorLocation();
	FVector below = FVector(0.0f, 0.0f, -50.0f);
	FCollisionQueryParams lineParams;
	lineParams.AddIgnoredActor(this);
	FVector lineEnd = currentLocation + below;

	FHitResult hitResult;
	if (
		GetWorld()->LineTraceSingleByChannel(
			OUT hitResult,
			currentLocation,
			lineEnd,
			ECollisionChannel::ECC_WorldDynamic,
			lineParams
		)
		)
	{
		AActor* hitActor = hitResult.GetActor();
		if (hitActor != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s current tile is set to %s"), *this->GetName(), *hitActor->GetName());
			CurrentTile = Cast<ATile>(hitActor);
			CurrentTile->EnemyOccupied = true;
		}
	}
}