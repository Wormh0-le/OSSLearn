// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "XMultiPlayerSessionsSubsystem.generated.h"


// Declaring our own custom delegates for the menu class to bind callbacks to
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiPlayerOnCreateSessionComplete, bool, bWasSuccessful);


/**
 * 
 */
UCLASS()
class XMULTIPLAYERSESSIONS_API UXMultiPlayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UXMultiPlayerSessionsSubsystem();

	// To handle session functionality. The menu class will call these
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();

	// Our own custom delegates for the Menu class to bind callbacks to
	FMultiPlayerOnCreateSessionComplete MultiPlayerOnCreateSessionComplete;

protected:
	// Internal callbacks for the delegates we'll add to the Online Session Interface delegate list.
	// These don't need to be called outside this class.

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnjoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	// To add to the Online Session Interface delegate list.
	// We' ll bind Our MultiplayerSessionSubsystem internal callbacks to these.

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate FindSessionCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;
};
