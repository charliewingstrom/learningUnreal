// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraDirector.h"
#include "Kismet/GameplayStatics.h"
#include <math.h>
// Sets default values
ACameraDirector::ACameraDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	CurrentDirection = North;
	Velocity = 10.0f;
	bMovingForward = false;
	bMovingBack = false;
	bMovingLeft = false;
	bMovingRight = false;
}

// Called when the game starts or when spawned
void ACameraDirector::BeginPlay()
{
	// Setup the default camera
	Super::BeginPlay();
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (OurPlayerController && CameraOne != nullptr)
	{
		OurPlayerController->SetViewTarget(CameraOne);
	}
	
}

// Called every frame
void ACameraDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bMovingForward)
		MoveForward();
	if (bMovingBack)
		MoveBack();
	if (bMovingLeft)
		MoveLeft();
	if (bMovingRight)
		MoveRight();
}

void ACameraDirector::MoveForward()
{
	FVector currentLocation = this->GetActorLocation();
	if (CurrentDirection == North)
		currentLocation += FVector(Velocity, 0.0f, 0.0f);
	
	else if (CurrentDirection == East)
		currentLocation += FVector(0.0f, Velocity, 0.0f);
	
	else if (CurrentDirection == South)
		currentLocation += FVector(-Velocity, 0.0f, 0.0f);
	
	else if (CurrentDirection == West)
		currentLocation += FVector(0.0f, -Velocity, 0.0f);
	
	if (currentLocation[0] < UpperLimit && currentLocation[0] > LowerLimit && currentLocation[1] < RightLimit && currentLocation[1] > LeftLimit)
		this->SetActorLocation(currentLocation);
}

void ACameraDirector::MoveBack()
{
	FVector currentLocation = this->GetActorLocation();
	if (CurrentDirection == North)
		currentLocation += FVector(-Velocity, 0.0f, 0.0f);
	
	else if (CurrentDirection == East)
		currentLocation += FVector(0.0f, -Velocity, 0.0f);
	
	else if (CurrentDirection == South)
		currentLocation += FVector(Velocity, 0.0f, 0.0f);

	else if (CurrentDirection == West)
		currentLocation += FVector(0.0f, Velocity, 0.0f);
	
	if (currentLocation[0] < UpperLimit && currentLocation[0] > LowerLimit && currentLocation[1] < RightLimit && currentLocation[1] > LeftLimit)
		this->SetActorLocation(currentLocation);
}

void ACameraDirector::MoveLeft()
{
	FVector currentLocation = this->GetActorLocation();
	if (CurrentDirection == North)
		currentLocation += FVector(0.0f, -Velocity, 0.0f);
	
	else if (CurrentDirection == East)
		currentLocation += FVector(Velocity, 0.0f, 0.0f);
	
	else if (CurrentDirection == South)
		currentLocation += FVector(0.0f, Velocity, 0.0f);
	
	else if (CurrentDirection == West)
		currentLocation += FVector(-Velocity, 0.0f, 0.0f);
	
	if (currentLocation[0] < UpperLimit && currentLocation[0] > LowerLimit && currentLocation[1] < RightLimit && currentLocation[1] > LeftLimit)
		this->SetActorLocation(currentLocation);
}

void ACameraDirector::MoveRight()
{
	FVector currentLocation = this->GetActorLocation();
	if (CurrentDirection == North)
		currentLocation += FVector(0.0f, Velocity, 0.0f);

	else if (CurrentDirection == East)
		currentLocation += FVector(-Velocity, 0.0f, 0.0f);

	else if (CurrentDirection == South)
		currentLocation += FVector(0.0f, -Velocity, 0.0f);

	else if (CurrentDirection == West)
		currentLocation += FVector(Velocity, 0.0f, 0.0f);

	if (currentLocation[0] < UpperLimit && currentLocation[0] > LowerLimit && currentLocation[1] < RightLimit && currentLocation[1] > LeftLimit)
		this->SetActorLocation(currentLocation);
}

void ACameraDirector::SetMovingForward()
{
	bMovingForward = !bMovingForward;
}

void ACameraDirector::SetMovingBack()
{
	bMovingBack = !bMovingBack;
}

void ACameraDirector::SetMovingLeft()
{
	bMovingLeft = !bMovingLeft;
}

void ACameraDirector::SetMovingRight()
{
	bMovingRight = !bMovingRight;
}

void ACameraDirector::ChangeVelocity()
{
	if (Velocity == 10.0f)
		Velocity = 60.0f;
	else
		Velocity = 10.0f;
}

void ACameraDirector::RotateCamera(bool bRight)
{
	FRotator currentRotation = this->GetActorRotation();
	
	if (bRight)
	{
		if (CurrentDirection == North)
		{
			currentRotation.Yaw = 90.0f;
			CurrentDirection = East;
		}
		else if (CurrentDirection == East)
		{
			currentRotation.Yaw = 180.0f;
			CurrentDirection = South;
		}
		else if (CurrentDirection == South)
		{
			currentRotation.Yaw = 270.0f;
			CurrentDirection = West;
		}
		else if (CurrentDirection == West)
		{
			currentRotation.Yaw = 0.0f;
			CurrentDirection = North;
		}
	}
	else 
	{
		if (CurrentDirection == North)
		{
			currentRotation.Yaw = 270.0f;
			CurrentDirection = West;
		}
		else if (CurrentDirection == West)
		{
			currentRotation.Yaw = 180.0f;
			CurrentDirection = South;
		}
		else if (CurrentDirection == South)
		{
			currentRotation.Yaw = 90.0f;
			CurrentDirection = East;
		}
		else if (CurrentDirection == East)
		{
			currentRotation.Yaw = 0.0f;
			CurrentDirection = North;
		}
	}
	this->SetActorRotation(currentRotation);
}

void ACameraDirector::Zoom(bool bIn)
{
	FVector currentLocation = CameraOne->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("%g, %g, %g"), currentLocation[0], currentLocation[1], currentLocation[2]);
	if (bIn)
	{
		currentLocation[0] += 20.0f;
		currentLocation[2] += 20.0f;
	}
	else
	{
		currentLocation[0] -= 20.0f;
		currentLocation[2] -= 20.0f;
	}
		
	CameraOne->SetActorLocation(currentLocation);
}

