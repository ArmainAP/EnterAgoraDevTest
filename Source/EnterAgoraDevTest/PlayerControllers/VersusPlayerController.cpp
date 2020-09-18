#include "VersusPlayerController.h"
#include "../StaticBindingsLibrary.h"
#include "../PlayerPawn.h"

void AVersusPlayerController::BeginPlay()
{
    Super::BeginPlay();

    P1 = SpawnP1(FVector(0, 500, 300), FRotator::ZeroRotator);
}
