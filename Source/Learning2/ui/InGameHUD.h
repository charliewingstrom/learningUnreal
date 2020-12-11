// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Components/WidgetComponent.h"
#include "MyTextWidget.h"
#include "EnemyTurnStartWidget.h"

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
	void ShowPlayerPhaseText();
	
	UFUNCTION()
	void ShowEnemyPhaseText();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerPhaseWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> EnemyPhaseWidgetClass;

private:
	UMyTextWidget* PlayerPhaseWidget;
	UMyTextWidget* EnemyPhaseWidget;
};
