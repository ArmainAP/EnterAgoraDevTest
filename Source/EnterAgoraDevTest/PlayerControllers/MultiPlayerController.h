#pragma once

#include "CoreMinimal.h"
#include "ShipPlayerController.h"
#include "MultiPlayerController.generated.h"

/**
 * Player controller designed to be used in multiplayer
 */
UCLASS()
class ENTERAGORADEVTEST_API AMultiPlayerController : public AShipPlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void PossessByIndex(int index);

	UPROPERTY()
	class APlayerPawn* ControlledPlayer;
};
