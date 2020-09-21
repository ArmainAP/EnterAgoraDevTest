#include "WaveManager.h"
#include <Kismet/KismetMathLibrary.h>
#include <TimerManager.h>
#include <Engine/World.h>
#include "Curves/CurveFloat.h"

#include "EnemyShip.h"


// Sets default values
AWaveManager::AWaveManager()
{
    SetReplicates(true);
    bAlwaysRelevant = true;

    // We want this to be available only on the server.
    bNetLoadOnClient = false;

    // Just to make sure
    FiniteWaves = FiniteWaves && SpawnPattern != nullptr;
}

void AWaveManager::StartWave()
{
    CurrentWave++;
    // Get the numbers of enemies to spawn, either from the set spawn pattern or generated from the sequence.
    EnemiesToSpawn = FiniteWaves ? SpawnPattern->GetFloatValue(CurrentWave) : Sequence(CurrentWave);
    EnemiesToKill = EnemiesToSpawn;

    check(GetWorld());
    GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AWaveManager::WaveTimer, SpawnFrequency, true);
}

void AWaveManager::WaveTimer()
{
    // Spawn enemies if we still have to spawn them, otherwise check if we can end the wave.
    if (EnemiesToSpawn > 0)
    {
        // We know that we spawned one more enemy
        EnemiesToSpawn--;

        // Choose a random spawn point
        // If not provided, use the manager's location
        FTransform SpawnTransform(GetActorTransform());
        if (SpawnPoints.Num() > 0)
        {
            int spawnPointIndex = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPoints.Num() - 1);
            if (SpawnPoints[spawnPointIndex])
            {
                SpawnTransform = SpawnPoints[spawnPointIndex]->GetActorTransform();
            }
        }

        SpawnEnemy(SpawnTransform);
    }
    else if (EnemiesKilled == EnemiesToKill && EnemiesToKill != 0)
    {
        EndWave();
    }
}

void AWaveManager::EndWave()
{
    // We reset the enemy count
    EnemiesKilled = 0;
    GetWorld()->GetTimerManager().ClearTimer(SpawnHandle);

    // Start next wave if the configuration allows it
    if (FiniteWaves == false)
    {
        StartWave();
    }
    else if (MaxWaves > CurrentWave)
    {
        StartWave();
    }
    else
    {
        bGameOver = true;
    }
}

void AWaveManager::SpawnEnemy_Implementation(FTransform SpawnTransform)
{
    UWorld* const World = GetWorld();
    if (World != NULL)
    {
        // spawn the projectile
        AEnemyShip* spawnedActor = World->SpawnActor<AEnemyShip>(SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator());
        spawnedActor->OnDestroyed.AddDynamic(this, &AWaveManager::OnEnemyDestroyed);
    }
}

void AWaveManager::OnEnemyDestroyed(AActor* DestroyedActor)
{
    EnemiesKilled++;
}

FString AWaveManager::BitsToString(int n)
{
    n = FMath::Pow(n, FMath::FloorToInt(n / 2));
    FString Bits;
    //Reserving space for string in memory as FString won't reallocate string multiple times in process
    Bits.Reserve(FMath::FloorToInt(FMath::Sqrt(n)));
    //We multiply by 2 until it's bigger then n where every bit will be 0 after that
    for (int i = 1; i < n; i = i * 2)
    {
        if (n & i)
            Bits.Append("1");
        else
            Bits.Append("0");
    }
    //We need to reverse bit order, so smallest is last
    return Bits.Reverse();
}

int AWaveManager::Count(FString String, FString ElementToCount)
{
    int count = 0;
    for (int i = 0; i < String.Len(); i++)
        if (String[i] == ElementToCount[0])
            count++;
    return count;
}

bool AWaveManager::IsPrime(int n)
{
    bool isPrime = true;
    for (int i = 2; i <= n / 2; i++)
        if (n % i == 0)
        {
            isPrime = false;
            break;
        }
    return isPrime;
}

int AWaveManager::Sequence(int n)
{
    int i = 1;
    int j = 1;
    while (j <= n)
    {
        if (IsPrime(Count(BitsToString(i), "1")))
            j++;
        i++;
    }
    return i + 18;
}