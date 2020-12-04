// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"

AInGameHUD::AInGameHUD()
{
}

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (StartTurnWidgetClass)
	{
		StartTurnWidget = CreateWidget<UStartTurnWidget>(GetWorld(), StartTurnWidgetClass);

		if (StartTurnWidget)
		{
			StartTurnWidget->AddToViewport();
		}
	}
}

void AInGameHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AInGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AInGameHUD::UpdateHealth(int Value)
{
	if (StartTurnWidget)
	{
		StartTurnWidget->UpdateHealth(Value);
	}
}