#pragma once

#include "CoreMinimal.h"
#include "MyGameModeBase.h"
#include "MultiplayerGameModeBase.generated.h"

class AShipPlayerController;
class APlayerPawn;

/**
 * GameMode designed to be used in multiplayer
 */
UCLASS()
class ENTERAGORADEVTEST_API AMultiplayerGameModeBase : public AMyGameModeBase
{
	GENERATED_BODY()
	
protected:
    // Called when a player joins
    virtual void PostLogin(APlayerController* NewPlayer) override;

    UPROPERTY()
    int Connections = 0;
};
