// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit.h"
#include "EnemyUnit.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING2_API AEnemyUnit : public AUnit
{
	GENERATED_BODY()

public:
	AEnemyUnit();
	void FindCurrentTile() override;
};
