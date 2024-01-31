// Copyright (C) 2024 owoDra

#include "GEEquipEditor.h"

#include "AssetTypeActions/AssetTypeActions_EquipmentBlueprint.h"
#include "AssetTypeActions/AssetTypeActions_EquipmentFragmentBlueprint.h"

IMPLEMENT_MODULE(FGEEquipEditorModule, GEEquipEditor)


void FGEEquipEditorModule::StartupModule()
{
	RegisterAssetTypeActions();
}

void FGEEquipEditorModule::ShutdownModule()
{
	UnregisterAssetTypeActions();
}


void FGEEquipEditorModule::RegisterAssetTypeActionCategory()
{
	static const FName CategoryKey{ TEXT("GameItem") };
	static const FText CategoryDisplayName{ NSLOCTEXT("GameXXX","GameItemCategory", "Item") };

	Category = IAssetTools::Get().RegisterAdvancedAssetCategory(CategoryKey, CategoryDisplayName);
}

void FGEEquipEditorModule::RegisterAssetTypeActions()
{
	RegisterAssetTypeActionCategory();

	RegisterAsset<FAssetTypeActions_EquipmentBlueprint>(RegisteredAssetTypeActions);
	RegisterAsset<FAssetTypeActions_EquipmentFragmentBlueprint>(RegisteredAssetTypeActions);
}

void FGEEquipEditorModule::UnregisterAssetTypeActions()
{
	UnregisterAssets(RegisteredAssetTypeActions);
}

void FGEEquipEditorModule::UnregisterAssets(TArray<TSharedPtr<FAssetTypeActions_Base>>& RegisteredAssets)
{
	const auto* AssetToolsPtr{ FModuleManager::GetModulePtr<FAssetToolsModule>(NAME_AssetToolsModule) };
	if (!AssetToolsPtr)
	{
		return;
	}

	auto& AssetTools{ AssetToolsPtr->Get() };
	for (auto& AssetTypeActionIt : RegisteredAssets)
	{
		if (AssetTypeActionIt.IsValid())
		{
			AssetTools.UnregisterAssetTypeActions(AssetTypeActionIt.ToSharedRef());
		}
	}
}
