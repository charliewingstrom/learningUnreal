// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"

#include "MyTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING2_API UMyTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMyTextWidget(const FObjectInitializer& ObjectInitalizer);

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text;


};
