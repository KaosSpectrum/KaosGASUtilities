// Copyright (C) 2024, Daniel Moss
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "KaosGASUtilitiesEditor.h"

#include "FKaosAttributeInitKeyCustomization.h"
#include "AbilitySystem/KaosAbilitySystemGlobals.h"

#define LOCTEXT_NAMESPACE "FKaosGASUtilitiesEditorModule"

FText FKaosGASUtilitiesEditorModule::Menu_ReloadAttributesText()
{
	return LOCTEXT("ReloadAttributeText", "Reload Attribute Tables..");
}
void FKaosGASUtilitiesEditorModule::Menu_ReloadAttributes()
{
	UKaosAbilitySystemGlobals::Get().ReloadAttributeDefaults();
}

void FKaosGASUtilitiesEditorModule::StartupModule()
{
	// Register the details customizer
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("KaosAttributeInitializationKey", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FKaosAttributeInitKeyCustomization::MakeInstance));

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
		FToolMenuSection& Section = Menu->AddSection("KaosCore", LOCTEXT("KaosGASUtilities", "KaosGASUtilities"));
		Section.AddEntry(FToolMenuEntry::InitMenuEntry(
			"ReloadAttributeTables",
			TAttribute<FText>::Create(&FKaosGASUtilitiesEditorModule::Menu_ReloadAttributesText),
			LOCTEXT("ReloadAttributeTables", "Reload Attribute Tables"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "DeveloperTools.MenuIcon"),
			FUIAction(FExecuteAction::CreateStatic(&FKaosGASUtilitiesEditorModule::Menu_ReloadAttributes))
		));
	}
}

void FKaosGASUtilitiesEditorModule::ShutdownModule()
{
	// Unregister the details customizer
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout("KaosAttributeInitializationKey");

	// remove menu extension
	UToolMenus::UnregisterOwner(this);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FKaosGASUtilitiesEditorModule, KaosGASUtilitiesEditor)