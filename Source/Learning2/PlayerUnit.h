// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit.h"
#include "PlayerUnit.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING2_API APlayerUnit : public AUnit
{
	GENERATED_BODY()
	
public:
	APlayerUnit();
	void FindCurrentTile();
};
