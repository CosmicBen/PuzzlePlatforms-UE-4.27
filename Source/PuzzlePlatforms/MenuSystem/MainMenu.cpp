// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include <UObject/ConstructorHelpers.h>
#include <Components/Button.h>
#include <Components/EditableTextBox.h>
#include <Components/WidgetSwitcher.h>
#include <Components/TextBlock.h>
#include <Kismet/KismetSystemLibrary.h>
#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBpClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBpClass.Class != NULL)) { return; }

	ServerRowClass = ServerRowBpClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (!HostButton) { return false; }
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!CancelHostMenuButton) { return false; }
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!HostHostMenuButton) { return false; }
	HostHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!JoinButton) { return false; }
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!CancelJoinMenuButton) { return false; }
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!JoinJoinMenuButton) { return false; }
	JoinJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!QuitButton) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

	return true;
}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != NULL)) { return; }
	if (!ensure(HostMenu != NULL)) { return; }
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::HostServer()
{
	if (MenuInterface != NULL)
	{
		FString ServerName = ServerHostName->Text.ToString();
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != NULL)) { return; }

	ServerList->ClearChildren();
	uint32 Index = 0;

	for (const FServerData& ServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != NULL)) { return; }

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUserName));
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->Setup(this, Index);
		++Index;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;

	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));

		if (Row != NULL)
		{
			Row->bSelected = SelectedIndex.IsSet() && SelectedIndex.GetValue() == i;
		}
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set"));
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != NULL)) { return; }
	if (!ensure(JoinMenu != NULL)) { return; }
	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (MenuInterface != NULL)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != NULL)) { return; }
	if (!ensure(MainMenu != NULL)) { return; }
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != NULL)) { return; }

	UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
	//PlayerController->ConsoleCommand(TEXT("quit"));
}