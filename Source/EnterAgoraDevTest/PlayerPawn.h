#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class ENTERAGORADEVTEST_API APlayerPawn : public APawn
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* ShipMeshComponent;

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

    //Sets the movements directions of the ship
    void SetForwardDirection(float Value);
    void SetRightDirection(float Value);

    //Fire a shot forward
    void FireShot(float Value);

    /** Offset from the ships location to spawn projectiles */
    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    FVector GunOffset;

    /* How fast the weapon will fire */
    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float FireRate;

    /* The speed our ship moves around the level */
    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float MoveSpeed;

    /** Sound to play each time we fire */
    UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
    class USoundBase* FireSound;

private:
    //Handles how the pawn should move
    void HandleMovement();

    /* Handler for the fire timer expiry */
    void ShotTimerExpired();

    //Movement direction of the ship
    FVector2D MovementDirection;

    /* Flag to control firing  */
    uint32 bCanFire : 1;

    /** Handle for efficient management of ShotTimerExpired timer */
    FTimerHandle TimerHandle_ShotTimerExpired;
};
