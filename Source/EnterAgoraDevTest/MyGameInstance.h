#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <Interfaces/OnlineSessionInterface.h>
#include "MyGameInstance.generated.h"

/**
 * GameInstance, we use it to manage the sessions
 */
UCLASS()
class ENTERAGORADEVTEST_API UMyGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
	
public:
    // Searches for a sessions and tries to join them, if that fails, we create one session
    UFUNCTION(BlueprintCallable, Category = "Sessions")
    void FindAvailableSession();

private:
    // Host a game session
    bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

    // Find a game session
    void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

    // Join session by search result
    bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

    // Function called when the session create request has been completed
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

    // Function called when the session start request has been completed
    void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

    // Function called when the search query is completed
    void OnFindSessionsComplete(bool bWasSuccessful);

    // Function called when the session join request has been completed
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    /* Delegate called when session created */
    FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
    /* Delegate called when session started */
    FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

    /** Handles to registered delegates for creating/starting a session */
    FDelegateHandle OnCreateSessionCompleteDelegateHandle;
    FDelegateHandle OnStartSessionCompleteDelegateHandle;

    TSharedPtr<class FOnlineSessionSettings> SessionSettings;

    /** Delegate for searching for sessions */
    FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

    /** Handle to registered delegate for searching a session */
    FDelegateHandle OnFindSessionsCompleteDelegateHandle;

    TSharedPtr<class FOnlineSessionSearch> SessionSearch;

    /** Delegate for joining a session */
    FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

    /** Handle to registered delegate for joining a session */
    FDelegateHandle OnJoinSessionCompleteDelegateHandle;
};
