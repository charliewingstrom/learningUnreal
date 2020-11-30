// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraDirector.generated.h"


/*
	The Rotation and Location is controlled by this component,
	Zoom is handled by changing the X value for CameraOne
*/
UCLASS()
class LEARNING2_API ACameraDirector : public AActor
{
	GENERATED_BODY()
	
	enum Direction {
		North,
		East,
		South,
		West
	};
	
public:	
	// Sets default values for this actor's properties
	ACameraDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		AActor* CameraOne;
	UPROPERTY(EditAnywhere)
		float UpperLimit;
	UPROPERTY(EditAnywhere)
		float LowerLimit;
	UPROPERTY(EditAnywhere)
		float RightLimit;
	UPROPERTY(EditAnywhere)
		float LeftLimit;

	void MoveForward();
	void MoveBack();
	void MoveLeft();
	void MoveRight();

	void SetMovingForward();
	void SetMovingBack();
	void SetMovingLeft();
	void SetMovingRight();
	void ChangeVelocity();
	void RotateCamera(bool bRight);
	void Zoom(bool bIn);

private:
	bool bMovingForward;
	bool bMovingBack;
	bool bMovingLeft;
	bool bMovingRight;
	float Velocity;
	Direction CurrentDirection;

};

