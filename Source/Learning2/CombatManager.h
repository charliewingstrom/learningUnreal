// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Unit.h"
#include "CoreMinimal.h"

/**
 * 
 */
class LEARNING2_API CombatManager
{
public:
	CombatManager();
	~CombatManager();

public:
	void StartCombat(AUnit* attackingUnit);
private:
	
	void FindUnitsInRange();
	void CalculateAttack();
	void Attack();

	AUnit* CurrentUnit;
	std::vector<AUnit*> UnitsInRange;
	AUnit* DefendingUnit;

	std::vector<ATile*> TouchedTiles;
};

