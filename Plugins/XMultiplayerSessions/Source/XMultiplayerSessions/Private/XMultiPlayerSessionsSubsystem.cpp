// Fill out your copyright notice in the Description page of Project Settings.


#include "XMultiPlayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

UXMultiPlayerSessionsSubsystem::UXMultiPlayerSessionsSubsystem():
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnjoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem) {
		SessionInterface = Subsystem->GetSessionInterface();
	}
}

void UXMultiPlayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}
	// store the delegate in a FDelegateHandle so we can later remove it from the delegate list
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// Broadcast our own custom delegate
		MultiPlayerOnCreateSessionComplete.Broadcast(false);
	}
}

void UXMultiPlayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
}

void UXMultiPlayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
}

void UXMultiPlayerSessionsSubsystem::DestroySession()
{
}

void UXMultiPlayerSessionsSubsystem::StartSession()
{
}

void UXMultiPlayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	MultiPlayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UXMultiPlayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
}

void UXMultiPlayerSessionsSubsystem::OnjoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}

void UXMultiPlayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UXMultiPlayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}
