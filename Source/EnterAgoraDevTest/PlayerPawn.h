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

    // Sets the movements directions of the ship
    void SetForwardDirection(float Value);
    void SetRightDirection(float Value);

    // Fire a shot forward
    void FireShot(float Value);

    // Deals some amount of Damage
    void DealDamage(int Ammount);

    //Returns true if we are still alive
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool IsAlive();

    // Sets the color to the ship's material
    void SetShipColor(FColor newColor);

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsP0 = true;

protected:
    virtual void BeginPlay();

    // Player Health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    int Health = 100;

    UPROPERTY()
    UMaterialInstanceDynamic* ShipDynamicMaterial;

private:
    //Handles how the pawn should move
    void HandleMovement();

    //Functia genereaza o pozitie noua neocupata de alta piesa
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerReplicateTransform(FTransform transform);

    //Functia muta regina pe noua pozitie generata
    UFUNCTION(NetMulticast, Reliable)
    void ReplicateTransform(FTransform transform);

    /* Handler for the fire timer expiry */
    void ShotTimerExpired();

    // Handler for the immunity timer expiry
    void ImmunityTimerExpired();

    // Flag to check if the player can be damaged
    bool bCanBeDamaged = true;

    //Movement direction of the ship
    FVector2D MovementDirection;

    /* Flag to control firing  */
    uint32 bCanFire : 1;

    /** Handle for efficient management of ShotTimerExpired timer */
    FTimerHandle TimerHandle_ShotTimerExpired;

    /** Handle for the management of the damage immunity timer */
    FTimerHandle TimerHandle_ImmunityTimerExpired;
};
