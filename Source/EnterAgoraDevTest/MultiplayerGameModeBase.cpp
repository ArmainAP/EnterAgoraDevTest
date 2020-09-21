#include "MultiplayerGameModeBase.h"
#include "PlayerControllers/ShipPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "WaveManager.h"
#include "PlayerPawn.h"
#include <Net/UnrealNetwork.h>
#include "PlayerControllers/MultiPlayerController.h"
#include <GameFramework/Actor.h>

void AMultiplayerGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    AMultiPlayerController* playerController = Cast<AMultiPlayerController>(NewPlayer);
    if (playerController)
    {
        playerController->PossessByIndex(Connections);
        Connections++;
    }

    if (Connections > 1)
    {
        TArray<AActor*> WaveManagers;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), WaveManagers);
        if (WaveManagers.Num() > 0 && WaveManagers[0])
        {
            if (AWaveManager* waveManager = Cast<AWaveManager>(WaveManagers[0]))
            {
                waveManager->StartWave();
            }
        }
    }
}