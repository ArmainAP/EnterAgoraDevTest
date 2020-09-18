#pragma once

#include "CoreMinimal.h"
#include "ShipPlayerController.h"
#include "VersusPlayerController.generated.h"

class APlayerPawn;

/**
 * Player Controller designed to be used with the Versus Game Mode
 */
UCLASS()
class ENTERAGORADEVTEST_API AVersusPlayerController : public AShipPlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay();
	
	UPROPERTY()
    APlayerPawn* P1;
};
