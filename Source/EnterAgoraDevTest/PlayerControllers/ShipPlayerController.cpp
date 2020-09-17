#include "ShipPlayerController.h"
#include "../PlayerPawn.h"
#include "../StaticBindingsLibrary.h"

void AShipPlayerController::BeginPlay()
{
    Super::BeginPlay();

    BindPawnInput();
}

void AShipPlayerController::BindPawnInput()
{
    check(InputComponent);
    
    //Asserts if the cast fails, meaning the possessed pawn is not derived from APlayerPawn
    APlayerPawn* playerPawn = Cast<APlayerPawn>(GetPawn());
    check(playerPawn);

    //Setup input bindings
    InputComponent->BindAxis(UStaticBindingsLibrary::MoveForwardP0_Binding, playerPawn, &APlayerPawn::SetForwardDirection);
    InputComponent->BindAxis(UStaticBindingsLibrary::MoveRightP0_Binding, playerPawn, &APlayerPawn::SetRightDirection);
    InputComponent->BindAxis(UStaticBindingsLibrary::FireP0_Binding, playerPawn, &APlayerPawn::FireShot);
}
