// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include <Components/Button.h>

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (!CancelButton) { return false; }
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);

	if (!QuitButton) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

	return true;
}

void UInGameMenu::CancelPressed()
{
	RemoveFromParent();
	OnLevelRemovedFromWorld(NULL, GetWorld());
}

void UInGameMenu::QuitPressed()
{
	if (MenuInterface != NULL)
	{
		RemoveFromParent();
		OnLevelRemovedFromWorld(NULL, GetWorld());
		MenuInterface->LoadMainMenu();
	}
}