// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionListItem.h"

#include "XMultiPlayerSessionsSubsystem.h"

void USessionListItem::SessionListItemSetup(FString& SessionNameStr, FString& HostUserNameStr, FString& UserCountStr, FString& LatencyStr, TSharedPtr<FOnlineSessionSearchResult>& SessionResultPtr)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiPlayerSessionsSubsystem = GameInstance->GetSubsystem<UXMultiPlayerSessionsSubsystem>();
	}
	if (MultiPlayerSessionsSubsystem) {
		MultiPlayerSessionsSubsystem->MultiPlayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
	}
	SessionName->SetText(FText::FromString(SessionNameStr));
	HostUserName->SetText(FText::FromString(HostUserNameStr));
	UserCount->SetText(FText::FromString(UserCountStr));
	Latency->SetText(FText::FromString(LatencyStr));
	SessionResult = SessionResultPtr;
}

bool USessionListItem::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	if (Btn_Join) {
		Btn_Join->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	return true;
}

void USessionListItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void USessionListItem::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem) {
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController) {
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
	if (Result != EOnJoinSessionCompleteResult::Success) {
		Btn_Join->SetIsEnabled(true);
	}
}

void USessionListItem::JoinButtonClicked()
{
	Btn_Join->SetIsEnabled(false);
	if (MultiPlayerSessionsSubsystem) {
		MultiPlayerSessionsSubsystem->JoinSession(*SessionResult);
	}
}
