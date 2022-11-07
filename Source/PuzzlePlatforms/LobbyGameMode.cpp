// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 1)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 5);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame()
{
	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (GameInstance == NULL) { return; }

	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) { return; }

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}