// Fill out your copyright notice in the Description page of Project Settings.

#include "Unit.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> UnitAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));

	if (UnitAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(UnitAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f));
		static ConstructorHelpers::FObjectFinder<UMaterial> UnitMaterial(TEXT("/Game/CustomMaterials/GenericPlayerUnitMaterial.GenericPlayerUnitMaterial"));
		Material = UnitMaterial.Object;
		VisualMesh->SetMaterial(0, Material);
	}
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	FindCurrentTile();
	CurrentTile->SetSelected();
}

void AUnit::FindCurrentTile()
{
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
		}
	}
}

void AUnit::SetPreviousTile()
{
	PreviousTile = CurrentTile;
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ATile* AUnit::GetCurrentTile()
{
	return CurrentTile;
}

uint32_t AUnit::GetMovement()
{
	return Movement;
}

