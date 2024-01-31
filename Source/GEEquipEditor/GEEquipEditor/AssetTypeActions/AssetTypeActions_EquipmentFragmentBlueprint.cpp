// Copyright (C) 2024 owoDra

#include "AssetTypeActions_EquipmentFragmentBlueprint.h"

#include "Equipment/Fragment/EquipmentFragmentBlueprint.h"
#include "Equipment/Fragment/EquipmentFragment.h"
#include "Factory/EquipmentFragmentBlueprintFactory.h"
#include "GEEquipEditor.h"

#include "Misc/MessageDialog.h"
#include "BlueprintEditor.h"
#include "Kismet2/BlueprintEditorUtils.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass* FAssetTypeActions_EquipmentFragmentBlueprint::GetSupportedClass() const
{
	return UEquipmentFragmentBlueprint::StaticClass();
}

FText FAssetTypeActions_EquipmentFragmentBlueprint::GetName() const
{ 
	return LOCTEXT("AssetTypeActions_EquipmentFragmentBlueprint", "Equipment Fragment"); 
}

FColor FAssetTypeActions_EquipmentFragmentBlueprint::GetTypeColor() const
{
	return FColor(153, 102, 102);
}

uint32 FAssetTypeActions_EquipmentFragmentBlueprint::GetCategories()
{
	return FGEEquipEditorModule::Category;
}

const TArray<FText>& FAssetTypeActions_EquipmentFragmentBlueprint::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		LOCTEXT("SubMenus_EquipmentFragmentBlueprint", "Equipment")
	};

	return SubMenus;
}


void FAssetTypeActions_EquipmentFragmentBlueprint::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor )
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
						LOCTEXT("FailedToLoadEquipmentFragmentBlueprintWithContinue", "EquipmentFragment Blueprint could not be loaded because it derives from an invalid class.  Check to make sure the parent class for this blueprint hasn't been removed! Do you want to continue (it can crash the editor)?"))
				};

				bLetOpen = (EAppReturnType::Yes == MessageReplyType);
			}
		
			if (bLetOpen)
			{
				TSharedRef<FBlueprintEditor> NewEditor{ new FBlueprintEditor() };

				TArray<UBlueprint*> Blueprints;
				Blueprints.Add(Blueprint);

				NewEditor->InitBlueprintEditor(Mode, EditWithinLevelEditor, Blueprints, ShouldUseDataOnlyEditor(Blueprint));
			}
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("FailedToLoadEquipmentFragmentBlueprint", "EquipmentFragment Blueprint could not be loaded because it derives from an invalid class.  Check to make sure the parent class for this blueprint hasn't been removed!"));
		}
	}
}


UFactory* FAssetTypeActions_EquipmentFragmentBlueprint::GetFactoryForBlueprintType(UBlueprint* InBlueprint) const
{
	auto* EquipmentFragmentBlueprintFactory{ NewObject<UEquipmentFragmentBlueprintFactory>() };

	EquipmentFragmentBlueprintFactory->ParentClass = TSubclassOf<UEquipmentFragment>(*InBlueprint->GeneratedClass);

	return EquipmentFragmentBlueprintFactory;
}


bool FAssetTypeActions_EquipmentFragmentBlueprint::ShouldUseDataOnlyEditor(const UBlueprint* Blueprint) const
{
	return FBlueprintEditorUtils::IsDataOnlyBlueprint(Blueprint)
		&& !FBlueprintEditorUtils::IsLevelScriptBlueprint(Blueprint)
		&& !FBlueprintEditorUtils::IsInterfaceBlueprint(Blueprint)
		&& !Blueprint->bForceFullEditor
		&& !Blueprint->bIsNewlyCreated;
}

#undef LOCTEXT_NAMESPACE
