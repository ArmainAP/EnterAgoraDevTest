#include "PlayerPawn.h"

#include "TemplateDefaults/EnterAgoraDevTestProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include <Kismet/KismetMaterialLibrary.h>

#include "StaticBindingsLibrary.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));

    // Create the mesh component
    ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
    RootComponent = ShipMeshComponent;
    ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

    ShipDynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ShipMeshComponent->GetMaterial(0));
    ShipMeshComponent->SetMaterial(0, ShipDynamicMaterial);
    SetShipColor(FColor::Blue);

    // Cache our sound effect
    static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
    FireSound = FireAudio.Object;

    // Movement
    MoveSpeed = 1000.0f;
    MovementDirection = FVector2D::ZeroVector;

    // Weapon
    GunOffset = FVector(90.f, 0.f, 0.f);
    FireRate = 0.1f;
    bCanFire = true;
}

void APlayerPawn::SetForwardDirection(float Value)
{
    if (IsAlive())
    {
        MovementDirection.X = Value;
        HandleMovement();
    }
}

void APlayerPawn::SetRightDirection(float Value)
{
    if (IsAlive())
    {
        MovementDirection.Y = Value;
        HandleMovement();
    }
}

void APlayerPawn::FireShot(float Value)
{
    // If it's ok to fire again
    if (bCanFire == true
        && Value != 0.0f
        && IsAlive())
    {
        const FRotator FireRotation = GetActorForwardVector().Rotation();
        // Spawn projectile at an offset from this pawn
        const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            // spawn the projectile
            World->SpawnActor<AEnterAgoraDevTestProjectile>(SpawnLocation, FireRotation);
        }

        bCanFire = false;
        World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &APlayerPawn::ShotTimerExpired, FireRate);

        // try and play the sound if specified
        if (FireSound != nullptr)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        bCanFire = false;
    }
}

void APlayerPawn::DealDamage(int Ammount)
{
    if (bCanBeDamaged)
    {
        Health -= Ammount;
        bCanBeDamaged = false;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_ImmunityTimerExpired, this, &APlayerPawn::ImmunityTimerExpired, 1.0f);
    }
}

bool APlayerPawn::IsAlive()
{
    return Health > 0;
}

void APlayerPawn::SetShipColor(FColor newColor)
{
    ShipDynamicMaterial->SetVectorParameterValue("DiffuseColor", newColor);
}

void APlayerPawn::HandleMovement()
{
    // Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
    const FVector MoveDirection = FVector(MovementDirection.X, MovementDirection.Y, 0.f).GetClampedToMaxSize(1.0f);

    // Calculate  movement
    const FVector Movement = MoveDirection * MoveSpeed * GetWorld()->GetDeltaSeconds();

    // If non-zero size, move this actor
    if (Movement.SizeSquared() > 0.0f)
    {
        const FRotator NewRotation = Movement.Rotation();
        FHitResult Hit(1.f);
        RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

        if (Hit.IsValidBlockingHit())
        {
            const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
            const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
            RootComponent->MoveComponent(Deflection, NewRotation, true);
        }
    }
}

void APlayerPawn::ShotTimerExpired()
{
    bCanFire = true;
}

void APlayerPawn::ImmunityTimerExpired()
{
    bCanBeDamaged = true;
}
