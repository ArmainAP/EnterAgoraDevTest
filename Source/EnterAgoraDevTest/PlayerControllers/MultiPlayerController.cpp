#include "MultiPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "../PlayerPawn.h"
#include <Net/UnrealNetwork.h>
#include "../StaticBindingsLibrary.h"

void AMultiPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMultiPlayerController, ID);
}

void AMultiPlayerController::BeginPlay()
{
    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerPawn::StaticClass(), ID == 0 ? "P0" : "P1", Pawns);
    if (Pawns.IsValidIndex(0))
    {
        ControlledPlayer = Cast<APlayerPawn>(Pawns[0]);
        if(HasAuthority()) Possess(ControlledPlayer);
    }

    Super::BeginPlay();
}