// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

class UMainMenu;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostUser;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConnectionFraction;

	void Setup(UMainMenu* Parent, uint32 Index);

	UPROPERTY(BlueprintReadOnly)
	bool bSelected = false;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;

	UPROPERTY()
	UMainMenu* Parent;
	
	uint32 Index;

	UFUNCTION()
	void OnClicked();
};