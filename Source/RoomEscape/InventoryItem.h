// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FP_FirstPerson/FP_FirstPersonCharacter.h"

enum ItemID : int
{
	chair,
	door
};

class InventoryItem
{
public:
	InventoryItem();
	InventoryItem(const InventoryItem &src);
	~InventoryItem();

	int Index;
	static std::vector<std::unique_ptr<InventoryItem>> Inventory;

	void AddItemToInventory();
private:
	ItemID InventoryItemID;
};