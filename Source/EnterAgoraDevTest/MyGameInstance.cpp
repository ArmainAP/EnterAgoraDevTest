#include "MyGameInstance.h"
#include <OnlineSessionSettings.h>
#include <OnlineSubsystem.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/Engine.h>
#include <UObject/CoreOnline.h>
#include <Templates/SharedPointer.h>

UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    // Create session bindings
    OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnCreateSessionComplete);
    OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnStartOnlineGameComplete);

    // Find session bindings
    OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnFindSessionsComplete);

    // Join session bindings
    OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnJoinSessionComplete);
}

bool UMyGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
    // Get the Online Subsystem to work with
    IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub)
    {
        // Get the Session Interface, so we can call the "CreateSession" function on it
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid() && UserId.IsValid())
        {
            SessionSettings = MakeShareable(new FOnlineSessionSettings());
            SessionSettings->bIsLANMatch = bIsLAN;
            SessionSettings->bUsesPresence = bIsPresence;
            SessionSettings->NumPublicConnections = MaxNumPlayers;
            SessionSettings->NumPrivateConnections = 0;
            SessionSettings->bAllowInvites = true;
            SessionSettings->bAllowJoinInProgress = true;
            SessionSettings->bShouldAdvertise = true;
            SessionSettings->bAllowJoinViaPresence = true;
            SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

            SessionSettings->Set(SETTING_MAPNAME, FString("NewMap"), EOnlineDataAdvertisementType::ViaOnlineService);

            // Set the delegate to the Handle of the SessionInterface
            OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

            // Our delegate should get called when this is complete (doesn't need to be successful!)
            return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No OnlineSubsytem!"));
    }

    return false;
}

void UMyGameInstance::FindAvailableSession()
{
    // Creating a local player where we can get the UserID from
    ULocalPlayer* const Player = GetFirstGamePlayer();
    FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), true, true);
}

void UMyGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
    // Get the OnlineSubsystem we want to work with
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub)
    {
        // Get the SessionInterface from our OnlineSubsystem
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid() && UserId.IsValid())
        {
            SessionSearch = MakeShareable(new FOnlineSessionSearch());
            SessionSearch->bIsLanQuery = bIsLAN;
            SessionSearch->MaxSearchResults = 20;
            SessionSearch->PingBucketSize = 50;

            // We only want to set this Query Setting if "bIsPresence" is true
            if (bIsPresence)
            {
                SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
            }

            TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

            // Set the Delegate to the Delegate Handle of the FindSession function
            OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

            // Call the SessionInterface function. The Delegate gets called once this is finished
            Sessions->FindSessions(*UserId, SearchSettingsRef);
        }
    }
    else
    {
        // If something goes wrong, just call the Delegate Function directly with "false".
        OnFindSessionsComplete(false);
    }
}

bool UMyGameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
    bool bSuccessful = false;

    // Get OnlineSubsystem we want to work with
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub)
    {
        // Get SessionInterface from the OnlineSubsystem
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
        if (Sessions.IsValid() && UserId.IsValid())
        {
            // Set the Handle again
            OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
            bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
        }
    }

    return bSuccessful;
}

void UMyGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Create Session Complete: %s, %d"), *SessionName.ToString(), bWasSuccessful);

    // Get the OnlineSubsystem so we can get the Session Interface
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub)
    {
        // Get the Session Interface to call the StartSession function
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid())
        {
            // Clear the SessionComplete delegate handle, since we finished this call
            Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
            if (bWasSuccessful)
            {
                // Set the StartSession delegate handle
                OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

                // Our StartSessionComplete delegate should get called after this
                Sessions->StartSession(SessionName);
            }
        }

    }
}

void UMyGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Start Session Complete: %s, %d"), *SessionName.ToString(), bWasSuccessful);

    // Get the Online Subsystem so we can get the Session Interface
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub)
    {
        // Get the Session Interface to clear the Delegate
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
        if (Sessions.IsValid())
        {
            // Clear the delegate, since we finished this call
            Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
        }
    }

    if (bWasSuccessful)
    {
        UGameplayStatics::OpenLevel(GetWorld(), "Multiplayer", true, "listen");
    }
}

void UMyGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Find Session Complete: %d"), bWasSuccessful);

    ULocalPlayer* const Player = GetFirstGamePlayer();
    if (!bWasSuccessful)
    {
        // Creating a local player where we can get the UserID from
        HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, true, true, 4);
        return;
    }

    // Get OnlineSubsystem we want to work with
    IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub)
    {
        // Get SessionInterface of the OnlineSubsystem
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
        if (Sessions.IsValid())
        {
            // Clear the Delegate handle, since we finished this call
            Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

            UE_LOG(LogTemp, Warning, TEXT("Search Results: %d"), SessionSearch->SearchResults.Num());

            if (SessionSearch->SearchResults.Num() > 0)
            {
                for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
                    JoinSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, SessionSearch->SearchResults[SearchIdx]);
                }
            }
            else
            {
                // Creating a local player where we can get the UserID from
                HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, true, true, 4);
                return;
            }
        }
    }
}

void UMyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    UE_LOG(LogTemp, Warning, TEXT("Join Session Complete:  %s, %d"), *SessionName.ToString(), static_cast<int32>(Result));

    // Get the OnlineSubsystem we want to work with
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub)
    {
        // Get SessionInterface from the OnlineSubsystem
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid())
        {
            // Clear the Delegate again
            Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
            APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(this, 0);
            FString TravelURL;

            if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
            {
                PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
            }
        }
    }
}
