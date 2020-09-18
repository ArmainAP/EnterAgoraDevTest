#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShipPlayerController.generated.h"

/**
 *  Player Controller intended to be used with PlayerPawn.
 */
UCLASS()
class ENTERAGORADEVTEST_API AShipPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
    //Binds inputs keys
    virtual void BeginPlay() override;

    //We bind pawn input here because GetPawn() is NULL during SetupInputComponent()
    virtual void BindPawnInput();

    class APlayerPawn* SpawnP1(FVector P1Location, FRotator P1Rotation);

    void TogglePauseMenu();

    //virtual void SetupInputComponent() override;
};
