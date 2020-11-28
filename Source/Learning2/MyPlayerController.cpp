// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "Tile.h"
#include "Unit.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <queue>
using namespace std;

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	// Get all tiles in the level
	TArray<AActor*> tiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATile::StaticClass(), tiles);
	for (AActor* tile : tiles)
	{
		Tiles.push_back(Cast<ATile>(tile));
	}
}

void AMyPlayerController::BeginPlay()
{
	// Setup the default camera
	Super::BeginPlay();
	Director = Cast<ACameraDirector>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraDirector::StaticClass()));
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SelectActor", IE_Pressed, this, &AMyPlayerController::SelectActor);

	InputComponent->BindAction("MoveForward", IE_Pressed, this, &AMyPlayerController::MoveForward);
	InputComponent->BindAction("MoveForward", IE_Released, this, &AMyPlayerController::MoveForward);
	InputComponent->BindAction("MoveBack", IE_Pressed, this, &AMyPlayerController::MoveBack);
	InputComponent->BindAction("MoveBack", IE_Released, this, &AMyPlayerController::MoveBack);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &AMyPlayerController::MoveLeft);
	InputComponent->BindAction("MoveLeft", IE_Released, this, &AMyPlayerController::MoveLeft);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &AMyPlayerController::MoveRight);
	InputComponent->BindAction("MoveRight", IE_Released, this, &AMyPlayerController::MoveRight);
	InputComponent->BindAction("ChangeVelocity", IE_Pressed, this, &AMyPlayerController::ChangeVelocity);
	InputComponent->BindAction("ChangeVelocity", IE_Released, this, &AMyPlayerController::ChangeVelocity);
	InputComponent->BindAction("RotateCameraRight", IE_Pressed, this, &AMyPlayerController::RotateCameraRight);
	InputComponent->BindAction("RotateCameraLeft", IE_Pressed, this, &AMyPlayerController::RotateCameraLeft);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AMyPlayerController::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AMyPlayerController::ZoomOut);
}

void AMyPlayerController::MoveForward()
{
	Director->SetMovingForward();
}
void AMyPlayerController::MoveBack()
{
	Director->SetMovingBack();
}
void AMyPlayerController::MoveLeft()
{
	Director->SetMovingLeft();
}
void AMyPlayerController::MoveRight()
{
	Director->SetMovingRight();
}
void AMyPlayerController::ChangeVelocity()
{
	Director->ChangeVelocity();
}
void AMyPlayerController::RotateCameraRight()
{
	Director->RotateCamera(true);
}
void AMyPlayerController::RotateCameraLeft()
{
	Director->RotateCamera(false);
}
void AMyPlayerController::ZoomIn()
{
	Director->Zoom(true);
}
void AMyPlayerController::ZoomOut()
{
	Director->Zoom(false);
}

void AMyPlayerController::SelectActor()
{
	FVector Start, Dir, End;
	this->DeprojectMousePositionToWorld(Start, Dir);
	End = Start + (Dir * 8000.0f);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (HitResult.Actor.IsValid())
	{
		AActor* HitActor = HitResult.Actor.Get();
		if (HitActor->GetClass() == AUnit::StaticClass())
		{
			UE_LOG(LogTemp, Warning, TEXT("ITS a Unit"));
			AUnit* HitUnit = Cast<AUnit>(HitActor);
			ShowUnitMovementRange(HitUnit);
		}
		
	}
}

// just a helper that finds the largest distance stored in each Tile
bool largestDistance(ATile* tile1, ATile* tile2)
{
	return tile1->Distance > tile2->Distance;
}
// uses Dijkistras algo to find all tiles in movement range of Unit
// and set them as selectable
void AMyPlayerController::ShowUnitMovementRange(AUnit* Unit)
{
	Unit->GetCurrentTile()->Distance = 0;
	
	ATile* currentTile;
	while (!Tiles.empty())
	{
		sort(Tiles.begin(), Tiles.end(), largestDistance);
		currentTile = Tiles.back();
		Tiles.pop_back();
		if (currentTile->Distance <= Unit->GetMovement())
		{
			currentTile->SetSelected();
			currentTile->Visited = true;
			for (int i = 0; i < 4; i++)
			{
				ATile* neighbor = currentTile->GetAdjList()[i];
				if (neighbor != nullptr && !neighbor->Visited)
				{
					uint32_t tmpDistance = currentTile->Distance + neighbor->MovementPenalty;
					if (neighbor->Distance > tmpDistance)
						neighbor->Distance = tmpDistance;
				}
			}
		}
	}
}

