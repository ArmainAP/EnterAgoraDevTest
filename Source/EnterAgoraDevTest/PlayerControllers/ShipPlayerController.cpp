#include "ShipPlayerController.h"
#include "../PlayerPawn.h"
#include "../StaticBindingsLibrary.h"
#include "../MyGameModeBase.h"

void AShipPlayerController::BeginPlay()
{
    Super::BeginPlay();

    BindPawnInput();
}

void AShipPlayerController::BindPawnInput()
{
    check(InputComponent);

    // We want the pause menu to be binded to Escape and only Escape
    InputComponent->BindKey(EKeys::Escape, IE_Released, this, &AShipPlayerController::TogglePauseMenu);
    
    //Asserts if the cast fails, meaning the possessed pawn is not derived from APlayerPawn
    APlayerPawn* playerPawn = Cast<APlayerPawn>(GetPawn());
    if (playerPawn)
    {
        playerPawn->Tags.Add("P0");

        //Setup input bindings
        InputComponent->BindAxis(UStaticBindingsLibrary::MoveForwardP0_Binding, playerPawn, &APlayerPawn::SetForwardDirection);
        InputComponent->BindAxis(UStaticBindingsLibrary::MoveRightP0_Binding, playerPawn, &APlayerPawn::SetRightDirection);
        InputComponent->BindAxis(UStaticBindingsLibrary::FireP0_Binding, playerPawn, &APlayerPawn::FireShot);
    }
}

APlayerPawn* AShipPlayerController::SpawnP1(FVector P1Location, FRotator P1Rotation)
{
    APlayerPawn* P1 = nullptr;
    if (GetWorld())
    {
        FActorSpawnParameters spawnParameters;
        //Adjust the initial spawn position if collides with anmother object
        spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        P1 = GetWorld()->SpawnActor<APlayerPawn>(P1Location, P1Rotation, spawnParameters);
        if (P1)
        {
            P1->SetShipColor(FColor::Green);
            P1->Tags.Add("P1");
            P1->bIsP0 = false;

            //Setup input bindings for P1
            InputComponent->BindAxis(UStaticBindingsLibrary::MoveForwardP1_Binding, P1, &APlayerPawn::SetForwardDirection);
            InputComponent->BindAxis(UStaticBindingsLibrary::MoveRightP1_Binding, P1, &APlayerPawn::SetRightDirection);
            InputComponent->BindAxis(UStaticBindingsLibrary::FireP1_Binding, P1, &APlayerPawn::FireShot);
        }
    }

    return P1;
}

void AShipPlayerController::TogglePauseMenu()
{
    if (GetWorld())
    {
        AMyGameModeBase* gameMode = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
        if (gameMode)
        {
            gameMode->TogglePauseMenuVisibility();
            bShowMouseCursor = !bShowMouseCursor;
        }
    }
}
