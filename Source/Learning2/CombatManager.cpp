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
		if (currTile->AttackDistance <= CurrentUnit->GetAttackRange())
		{
			currTile->SetSelected();
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
