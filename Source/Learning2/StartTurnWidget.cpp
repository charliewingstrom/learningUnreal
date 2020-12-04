// Fill out your copyright notice in the Description page of Project Settings.


#include "StartTurnWidget.h"

UStartTurnWidget::UStartTurnWidget(const FObjectInitializer& ObjectInitalizer) : Super(ObjectInitalizer)
{

}

void UStartTurnWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStartTurnWidget::UpdateHealth(int Value)
{
	if (HealthText)
	{
		if (HealthText->Visibility == ESlateVisibility::Hidden)
			HealthText->SetVisibility(ESlateVisibility::Visible);
		HealthText->SetText(FText::FromString((FString::FromInt(Value) + " Health")));
	}
}
