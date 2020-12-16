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
	std::vector<AUnit*> GetUnitsInRange(AUnit* unit);
	// finds the damage done, chance to hit, and chance to crit of an attack
	void CalculateAttack();
	void InitiateAttack();
	void Cleanup();

	bool bDefenderCanCounter = false;
	int Damage;
	int Hit;
	int Crit;

	int CounterDamage;
	int CounterHit;
	int CounterCrit;

	AUnit* CurrentUnit;
	std::vector<AUnit*> UnitsInRange;
	AUnit* DefendingUnit;

	std::vector<ATile*> TouchedTiles;
};

