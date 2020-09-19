#include "MultiplayerGameModeBase.h"
#include "PlayerControllers/ShipPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "WaveManager.h"
#include "PlayerPawn.h"
#include <Net/UnrealNetwork.h>
#include "PlayerControllers/MultiPlayerController.h"

void AMultiplayerGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    
    if (AMultiPlayerController* playerController = Cast<AMultiPlayerController>(NewPlayer))
    {
        playerController->ID = Connections;
        Connections++;
    }

//     if (Connections > 1)
//     {
//         TArray<AActor*> WaveManagers;
//         UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), WaveManagers);
//         if (WaveManagers.Num() > 0 && WaveManagers[0])
//         {
//             if (AWaveManager* waveManager = Cast<AWaveManager>(WaveManagers[0]))
//             {
//                 waveManager->StartWave();
//             }
//         }
//     }
}