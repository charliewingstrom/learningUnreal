// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "Tile.h"
#include "Unit.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
using namespace std;

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMyPlayerController::BeginPlay()
{
	// Setup the default camera
	Super::BeginPlay();
	Director = Cast<ACameraDirector>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraDirector::StaticClass()));
	Player = Cast<APlayerPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPawn::StaticClass()));
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	

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

	InputComponent->BindAction("SelectActor", IE_Pressed, this, &AMyPlayerController::SelectActor);
	InputComponent->BindAction("UnitWait", IE_Pressed, this, &AMyPlayerController::UnitWait);
}

// Camera Functions
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

// General Functions
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
		Player->SelectActor(HitActor);
	}
}
void AMyPlayerController::UnitWait()
{
	Player->UnitWait();
}


