// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"

#include "StartTurnWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING2_API UStartTurnWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UStartTurnWidget(const FObjectInitializer& ObjectInitalizer);

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* HealthText;

	void UpdateHealth(int Value);


};
