// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("Remove From World"));

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	RemoveFromViewport();
	bIsFocusable = false;

	if (!ensure(InWorld != NULL)) { return; }

	APlayerController* PlayerController = InWorld->GetFirstPlayerController();
	if (!ensure(PlayerController != NULL)) { return; }

	FInputModeGameOnly InputModeData;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void UMenuWidget::Setup()
{
	AddToViewport();
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != NULL)) { return; }

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::SetMenuInterface(IMenuInterface* NewMenuInterface)
{
	this->MenuInterface = NewMenuInterface;
}