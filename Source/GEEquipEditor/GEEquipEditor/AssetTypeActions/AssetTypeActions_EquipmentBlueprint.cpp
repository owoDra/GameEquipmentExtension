// Copyright (C) 2024 owoDra

#include "AssetTypeActions_EquipmentBlueprint.h"

#include "Equipment/EquipmentBlueprint.h"
#include "Equipment/Equipment.h"
#include "Factory/EquipmentBlueprintFactory.h"
#include "GEEquipEditor.h"

#include "Misc/MessageDialog.h"
#include "BlueprintEditor.h"
#include "Kismet2/BlueprintEditorUtils.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass* FAssetTypeActions_EquipmentBlueprint::GetSupportedClass() const
{
	return UEquipmentBlueprint::StaticClass();
}

FText FAssetTypeActions_EquipmentBlueprint::GetName() const
{ 
	return LOCTEXT("AssetTypeActions_EquipmentBlueprint", "Equipment"); 
}

FColor FAssetTypeActions_EquipmentBlueprint::GetTypeColor() const
{
	return FColor(153, 102, 255);
}

uint32 FAssetTypeActions_EquipmentBlueprint::GetCategories()
{
	return FGEEquipEditorModule::Category;
}

const TArray<FText>& FAssetTypeActions_EquipmentBlueprint::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		LOCTEXT("SubMenus_EquipmentBlueprint", "Equipment")
	};

	return SubMenus;
}


void FAssetTypeActions_EquipmentBlueprint::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor )
{
	auto Mode{ EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone };

	for (auto ObjIt{ InObjects.CreateConstIterator() }; ObjIt; ++ObjIt)
	{
		if (auto Blueprint{ Cast<UBlueprint>(*ObjIt) })
		{
			auto bLetOpen{ true };

			if (!Blueprint->ParentClass)
			{
				const auto MessageReplyType
				{
					FMessageDialog::Open(EAppMsgType::YesNo,
						LOCTEXT("FailedToLoadEquipmentBlueprintWithContinue", "Equipment Blueprint could not be loaded because it derives from an invalid class.  Check to make sure the parent class for this blueprint hasn't been removed! Do you want to continue (it can crash the editor)?"))
				};

				bLetOpen = (EAppReturnType::Yes == MessageReplyType);
			}
		
			if (bLetOpen)
			{
				TSharedRef<FBlueprintEditor> NewEditor{ new FBlueprintEditor() };

				TArray<UBlueprint*> Blueprints;
				Blueprints.Add(Blueprint);

				NewEditor->InitBlueprintEditor(Mode, EditWithinLevelEditor, Blueprints, true);
			}
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("FailedToLoadEquipmentBlueprint", "Equipment Blueprint could not be loaded because it derives from an invalid class.  Check to make sure the parent class for this blueprint hasn't been removed!"));
		}
	}
}


UFactory* FAssetTypeActions_EquipmentBlueprint::GetFactoryForBlueprintType(UBlueprint* InBlueprint) const
{
	auto* EquipmentBlueprintFactory{ NewObject<UEquipmentBlueprintFactory>() };

	EquipmentBlueprintFactory->ParentClass = TSubclassOf<UEquipment>(*InBlueprint->GeneratedClass);

	return EquipmentBlueprintFactory;
}

#undef LOCTEXT_NAMESPACE
