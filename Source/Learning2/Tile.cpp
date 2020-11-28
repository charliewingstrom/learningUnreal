// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
FVector vectorArray[]{ FVector(150.0f, 0.0f, 0.0f), FVector(0.0f, 150.0f, 0.0f), FVector(0.0f, -150.0f, 0.0f), FVector(-150.0f, 0.0f, 0.0f) };
// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TileAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (TileAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(TileAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		static ConstructorHelpers::FObjectFinder<UMaterial> defaultMaterial(TEXT("/Game/CustomMaterials/Tiles/DefaultTile.DefaultTile"));
		DefaultMaterial = defaultMaterial.Object;
		VisualMesh->SetMaterial(0, DefaultMaterial);
		static ConstructorHelpers::FObjectFinder<UMaterial> selectableMaterial(TEXT("/Game/CustomMaterials/Tiles/SelectableTile.SelectableTile"));
		SelectableMaterial = selectableMaterial.Object;
		static ConstructorHelpers::FObjectFinder<UMaterial> highlightMaterial(TEXT("/Game/CustomMaterials/Tiles/HighlightTile.HighlightTile"));
		HighlightMaterial = highlightMaterial.Object;
	}
} 

// Gets the 4 tiles touching this and add them to AdjList
void ATile::FindAdjList()
{
	FVector currentLocation = GetActorLocation();
	
	FCollisionQueryParams lineParams;
	lineParams.AddIgnoredActor(this);
	for (int i = 0; i < 4; i++)
	{
		FVector lineEnd = currentLocation + vectorArray[i];

		/*DrawDebugLine(
			GetWorld(),
			currentLocation,
			lineEnd,
			FColor::Red,
			true,
			100.0f,
			0,
			5.0f
		);*/

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
				//UE_LOG(LogTemp, Warning, TEXT("Actor Hit %s"), *hitActor->GetName());
				AdjList.push_back( Cast<ATile>(hitActor));
			}
		}
		//else
			//UE_LOG(LogTemp, Warning, TEXT("Hit nothing"));
	}
}




std::vector<ATile*> ATile::GetAdjList()
{
	return AdjList;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	this->FindAdjList();
}

void ATile::SetSelected()
{
	VisualMesh->SetMaterial(0, SelectableMaterial);
	Selectable = true;
}

void ATile::Highlight(bool On)
{
	if (On)
		VisualMesh->SetMaterial(0, HighlightMaterial);
	else if (Selectable)
		VisualMesh->SetMaterial(0, SelectableMaterial);
	else
		VisualMesh->SetMaterial(0, DefaultMaterial);
}

void ATile::Reset()
{
	VisualMesh->SetMaterial(0, DefaultMaterial);
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

