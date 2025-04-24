// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "XMultiPlayerSessionsSubsystem.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch)
{
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	UWorld* World = GetWorld();
	if(World) {
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiPlayerSessionsSubsystem = GameInstance->GetSubsystem<UXMultiPlayerSessionsSubsystem>();
	}

	if (MultiPlayerSessionsSubsystem) {
		MultiPlayerSessionsSubsystem->MultiPlayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	if (Btn_Host)
	{
		Btn_Host->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}
	if (Btn_Join)
	{
		Btn_Join->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	return true;
}

void UMenu::NativeDestruct()
{
	Menu_TearDown();
	Super::NativeDestruct();
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Green,
				FString(TEXT("Session created successfully."))
			);
		}

		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel("/XMultiplayerSessions/Lobby?listen");
		}
	}
	else {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session."))
			);
		}
	}
}

void UMenu::HostButtonClicked()
{
	if (MultiPlayerSessionsSubsystem)
	{
		MultiPlayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void UMenu::JoinButtonClicked()
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("Join button clicked."))
		);
	}
}

void UMenu::Menu_TearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}
