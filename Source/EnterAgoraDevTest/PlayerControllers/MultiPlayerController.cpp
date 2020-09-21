#include "MultiPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "../PlayerPawn.h"
#include <Net/UnrealNetwork.h>
#include "../StaticBindingsLibrary.h"

void AMultiPlayerController::PossessByIndex_Implementation(int index)
{
    FString tag = "P" + FString::FromInt(index);
    TArray<AActor*> actors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(tag), actors);
    if (actors.IsValidIndex(0))
    {
        APlayerPawn* pawn = Cast<APlayerPawn>(actors[0]);
        if (pawn)
        {
            Possess(pawn);
            BindPawnInput();

            UE_LOG(LogTemp, Warning, TEXT("Test"));
        }
    }
}
