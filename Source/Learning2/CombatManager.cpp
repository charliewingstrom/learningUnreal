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
	UE_LOG(LogTemp, Warning, TEXT("Combat Started"));
	if (CurrentUnit != nullptr)
	{
		FindUnitsInRange();
		if (!UnitsInRange.empty())
		{
			DefendingUnit = UnitsInRange[0];
			CalculateAttack();
			// display attack forcast UI

		}
	}
}

void CombatManager::FindUnitsInRange()
{
	// need to find the units in attacking range
	// queue
	// add first to queue
	// get neighbors, add them to queue
	// loop
	std::vector<ATile*> queue;
	ATile* firstTile = CurrentUnit->GetCurrentTile();
	firstTile->Visited = true;
	queue.push_back(firstTile);
	ATile* currTile;
	while (!queue.empty())
	{
		currTile = queue.back();
		queue.pop_back();
		TouchedTiles.push_back(currTile);
		if (currTile->AttackDistance <= CurrentUnit->GetAttackRange())
		{
			currTile->SetAttackable();
			AUnit* target = currTile->GetCurrentUnit();
			if (currTile->EnemyOccupied && target != nullptr)
				UnitsInRange.push_back(target);

			for (ATile* neighbor : currTile->GetAdjList())
			{
				if (!neighbor->Visited)
				{
					neighbor->Visited = true;
					neighbor->AttackDistance = currTile->AttackDistance + 1;
					queue.push_back(neighbor);
				}

			}
		}
	}
}

std::vector<AUnit*> CombatManager::GetUnitsInRange(AUnit* unit)
{
	std::vector<AUnit*> toReturn;
	ATile* firstTile = unit->GetCurrentTile();

	return std::vector<AUnit*>();
}

void CombatManager::CalculateAttack()
{
	Damage = std::max(0, CurrentUnit->Str - DefendingUnit->Def);

	Hit = (CurrentUnit->Dex + 50) - DefendingUnit->Dex;

	Crit = CurrentUnit->Luck;
	
	// need to check if bDefenderCanCounter

}

void CombatManager::InitiateAttack()
{
	int hitChance = rand() % 100;
	int critChance = rand() % 100;
	// if hitChance is lower than Hit, continue, else miss
	if (hitChance <= Hit)
	{
		if (critChance <= Crit)
			DefendingUnit->CurrHp -= Damage * 3;
		else
			DefendingUnit->CurrHp -= Damage;
	}
	if (DefendingUnit->CurrHp <= 0)
	{
		DefendingUnit->Destroy();
	}
	else if (bDefenderCanCounter)
	{
		hitChance = rand() % 100;
		if (hitChance <= CounterHit)
		{
			critChance = rand() % 100;
			if (critChance <= CounterCrit)
				CurrentUnit->CurrHp -= CounterDamage * 3;
			else
				CurrentUnit->CurrHp -= CounterDamage;
		}
	}
	if (CurrentUnit->CurrHp <= 0)
	{
		CurrentUnit->Destroy();
	}
}

void CombatManager::Cleanup()
{
	bDefenderCanCounter = false;
	Damage = 0;
	Hit = 0;
	Crit = 0;
	CounterDamage = 0;
	CounterHit = 0;
	CounterCrit = 0;
	CurrentUnit = nullptr;
	DefendingUnit = nullptr;
	UnitsInRange.clear();
	while (!TouchedTiles.empty())
	{
		TouchedTiles.back()->Reset();
		TouchedTiles.pop_back();
	}
}
