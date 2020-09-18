#pragma once

#include "CoreMinimal.h"
#include "ShipPlayerController.h"
#include "SurvivalPlayerController.generated.h"

/**
 * Player Controller designed to be used with the Survival Game Mode
 */
UCLASS()
class ENTERAGORADEVTEST_API ASurvivalPlayerController : public AShipPlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BindPawnInput() override;

	void OnP2Fire(float Value);

	UPROPERTY()
	class APlayerPawn* P1;
};
