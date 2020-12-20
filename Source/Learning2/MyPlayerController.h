// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Unit.h"
#include "CameraDirector.h"
#include "PlayerPawn.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING2_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	
	
public:
	AMyPlayerController();
	
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
private:
	// camera 
	void MoveForward();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void ChangeVelocity();
	void RotateCameraRight();
	void RotateCameraLeft();
	void ZoomIn();
	void ZoomOut();

	// general actions
	void SelectActor();
	void UnitWait();
	void ResetUnit();
	// combat
	void StartCombat();

	ACameraDirector* Director;
	APlayerPawn* Player;
};
