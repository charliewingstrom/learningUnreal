// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"

CombatManager::CombatManager()
{
}

CombatManager::~CombatManager()
{
}

void CombatManager::StartCombat(AUnit* attackingUnit)
{
	CurrentUnit = attackingUnit;

	// need to find the units in attacking range;
}
