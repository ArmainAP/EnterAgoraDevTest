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

	// Returns the current wave
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCurrentWave() { return CurrentWave; };

	// How often the enemies should spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnFrequency = 0.5f;

	// Where should the enemies spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> SpawnPoints;

	// For how long should the enemies spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FiniteWaves = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "FiniteWaves"))
	int MaxWaves;

	// In which wave pattern should the enemies spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "FiniteWaves"))
	class UCurveFloat* SpawnPattern;

	UPROPERTY(BlueprintReadOnly)
	bool bGameOver = false;

private:
	// Spawn enemies
	UFUNCTION()
	void WaveTimer();

	// Ends the current wave
	void EndWave();

	// Spawn enemy across all clients
	UFUNCTION(NetMulticast, Reliable)
	void SpawnEnemy(FTransform SpawnTransform);

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
	 * In case no spawn pattern is provided, we spawn the waves according to this algorithm just to spice things up :)
	 * http://oeis.org/A280998
	 * Generates numbers that contain a prime number of "1" in their binary reflected Gray representation.
	*/

	FString BitsToString(int n);
	int Count(FString String, FString ElementToCount);
	bool IsPrime(int n);
	int Sequence(int n);
};
