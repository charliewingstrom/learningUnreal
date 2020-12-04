// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Components/WidgetComponent.h"
#include "StartTurnWidget.h"

#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING2_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AInGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void UpdateHealth(int Value);

	UPROPERTY(EditDefaultsOnly, Category = "Interactive")
	TSubclassOf<UUserWidget> StartTurnWidgetClass;

private:
	UStartTurnWidget* StartTurnWidget;
};