// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"

#include <Components/BoxComponent.h>
#include "MovingPlatform.h"

APlatformTrigger::APlatformTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	SetRootComponent(TriggerVolume);
}

void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (!TriggerVolume) { return; }
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
}

void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}