// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem.h"

std::vector<std::unique_ptr<InventoryItem>> InventoryItem::Inventory;

InventoryItem::InventoryItem()
{
	UE_LOG(LogTemp, Log, TEXT("Constructor called"));
	int size = Inventory.size();
	Index = size;
	InventoryItemID = chair;
}

InventoryItem::~InventoryItem()
{
}

InventoryItem::InventoryItem(const InventoryItem& src)
{
	UE_LOG(LogTemp, Log, TEXT("Copy Constructor called"));
	this->InventoryItemID = src.InventoryItemID;
	this->Index = src.Index;
}

void InventoryItem::AddItemToInventory()
{
	Inventory.push_back(std::move(std::make_unique<InventoryItem>(*this)));
}
