#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

/*
 * Class designed to manage the waves.
 */
UCLASS()
class ENTERAGORADEVTEST_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveManager();
    
	// Starts the next wave
	UFUNCTION(BlueprintCallable)
	void StartWave();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnFrequency = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FiniteWaves = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "FiniteWaves"))
	class UCurveFloat* SpawnPattern;

private:
	// Spawn enemies
	UFUNCTION()
	void WaveTimer();

	// Ends the current wave
	void EndWave();

	// Called when an enemy is killed
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

    UPROPERTY()
    int CurrentWave;

	UPROPERTY()
	int EnemiesToSpawn;

	UPROPERTY()
	int EnemiesToKill;

	UPROPERTY()
	int EnemiesKilled;

	UPROPERTY()
	FTimerHandle SpawnHandle;

	/*
	 * In case nu spawn pattern is provided, we spawn the waves according to this algorithm just to spice things up :)
	 * http://oeis.org/A280998
	 * Generates numbers that contain a prime number of "1" in their binary reflected Gray representation.
	*/

	FString BitsToString(int n);
	int Count(FString String, FString ElementToCount);
	bool IsPrime(int n);
	int Sequence(int n);
};
