// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"

AInGameHUD::AInGameHUD()
{
}

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerPhaseWidgetClass)
	{
		PlayerPhaseWidget = CreateWidget<UMyTextWidget>(GetWorld(), PlayerPhaseWidgetClass);

		if (PlayerPhaseWidget)
		{
			PlayerPhaseWidget->AddToViewport();
		}
	}
	if (EnemyPhaseWidgetClass)
	{
		EnemyPhaseWidget = CreateWidget<UMyTextWidget>(GetWorld(), EnemyPhaseWidgetClass);
		if (EnemyPhaseWidget)
		{
			EnemyPhaseWidget->AddToViewport();
			EnemyPhaseWidget->SetVisibility(ESlateVisibility::Hidden);
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

void AInGameHUD::ShowPlayerPhaseText()
{
	if (PlayerPhaseWidget)
	{
		if (PlayerPhaseWidget->Visibility == ESlateVisibility::Hidden)
			PlayerPhaseWidget->SetVisibility(ESlateVisibility::Visible);
		else
			PlayerPhaseWidget->SetVisibility(ESlateVisibility::Hidden);
	}
		
}

void AInGameHUD::ShowEnemyPhaseText()
{
	if (EnemyPhaseWidget)
	{
		if (EnemyPhaseWidget->Visibility == ESlateVisibility::Hidden)
			EnemyPhaseWidget->SetVisibility(ESlateVisibility::Visible);
		else
			EnemyPhaseWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
