// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"

CombatManager::CombatManager()
{
	CurrentUnit = nullptr;
	DefendingUnit = nullptr;
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
		UnitsInRange = FindEnemiesInRange(CurrentUnit);
		if (!UnitsInRange.empty())
		{
			DefendingUnit = UnitsInRange[0];
			CalculateAttack();
			// display attack forcast UI

		}
	}
}

std::vector<AUnit*> CombatManager::FindEnemiesInRange(AUnit* unit)
{
	// need to find the units in attacking range
	// queue
	// add first to queue
	// get neighbors, add them to queue
	// loop
	std::vector<AUnit*> toReturn;
	std::vector<ATile*> queue;
	ATile* firstTile = unit->GetCurrentTile();
	firstTile->Visited = true;
	queue.push_back(firstTile);
	ATile* currTile;
	while (!queue.empty())
	{
		currTile = queue.back();
		queue.pop_back();
		TouchedTiles.push_back(currTile);
		if (currTile->AttackDistance <= unit->GetAttackRange())
		{
			currTile->SetAttackable();
			AUnit* target = currTile->GetCurrentUnit();
			if (currTile->EnemyOccupied && target != nullptr)
				toReturn.push_back(target);

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
	while (!TouchedTiles.empty())
	{
		TouchedTiles.back()->Reset();
		TouchedTiles.pop_back();
	}
	return toReturn;
}

std::vector<AUnit*> CombatManager::GetPlayersInRange(AUnit* unit)
{
	std::vector<AUnit*> toReturn;
	std::vector<ATile*> queue;
	ATile* firstTile = unit->GetCurrentTile();
	firstTile->Visited = true;
	queue.push_back(firstTile);
	ATile* currTile;
	while (!queue.empty())
	{
		currTile = queue.back();
		queue.pop_back();
		TouchedTiles.push_back(currTile);
		if (currTile->AttackDistance <= unit->GetAttackRange())
		{
			AUnit* target = currTile->GetCurrentUnit();
			if (currTile->PlayerOccupied && target != nullptr)
				toReturn.push_back(target);

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
	while (!TouchedTiles.empty())
	{
		TouchedTiles.back()->Reset();
		TouchedTiles.pop_back();
	}
	return toReturn;
}

void CombatManager::CalculateAttack()
{
	Damage = std::max(0, CurrentUnit->Str - DefendingUnit->Def);

	Hit = (CurrentUnit->Dex + 50) - DefendingUnit->Dex;

	Crit = CurrentUnit->Luck;
	
	// need to check if bDefenderCanCounter
	std::vector<AUnit*> playersInRange = GetPlayersInRange(DefendingUnit);

	if (std::find(playersInRange.begin(), playersInRange.end(), CurrentUnit) != playersInRange.end())
	{
		bDefenderCanCounter = true;
		UE_LOG(LogTemp, Warning, TEXT("defender can counter"));
		CounterDamage = std::max(0, DefendingUnit->Str - CurrentUnit->Def);
		CounterHit = (DefendingUnit->Dex + 50) - CurrentUnit->Dex;
		CounterCrit = DefendingUnit->Luck;
	}
	else
	{
		bDefenderCanCounter = false;
	}
}

void CombatManager::InitiateAttack()
{
	if (CurrentUnit != nullptr && DefendingUnit != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack Initiated"));
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
	Cleanup();
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
