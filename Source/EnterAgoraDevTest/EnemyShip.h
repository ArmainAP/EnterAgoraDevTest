#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyShip.generated.h"

/*
 * Class designed to be used as an enemy
 */
UCLASS()
class ENTERAGORADEVTEST_API AEnemyShip : public AActor
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

public:	
	// Sets default values for this actor's properties
	AEnemyShip();

	// Enemy Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Health = 100;

    // Enemy Health
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage = 25;

    /* The speed our ship moves around the level */
    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Responds to collision
    UFUNCTION()
	void OnMeshCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	// Picks the closes Player as a target and will focus on it
	void PickTarget();

	UPROPERTY()
	class APlayerPawn* Target;
	
	UPROPERTY()
	bool bSeekTarget = true;
};
