// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyShip.h"
#include <Kismet/KismetMaterialLibrary.h>
#include <Materials/Material.h>
#include <UObject/ConstructorHelpers.h>
#include <Engine/CollisionProfile.h>
#include <Components/StaticMeshComponent.h>
#include "PlayerPawn.h"
#include "TemplateDefaults/EnterAgoraDevTestProjectile.h"
#include <EngineUtils.h>
#include "MyGameModeBase.h"

// Sets default values
AEnemyShip::AEnemyShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));

    // Create the mesh component
    ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
    RootComponent = ShipMeshComponent;
    ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

    UMaterialInstanceDynamic* ShipDynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ShipMeshComponent->GetMaterial(0));
    ShipMeshComponent->SetMaterial(0, ShipDynamicMaterial);
    ShipDynamicMaterial->SetVectorParameterValue("DiffuseColor", FColor::Red);

    ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
    ShipMeshComponent->OnComponentHit.AddDynamic(this, &AEnemyShip::OnMeshCollision);

    MoveSpeed = 900.0f;
}

// Called every frame
void AEnemyShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    PickTarget();

    // Just to make sure we keep trying to pick targets when we don't have anymore targets that are alive.
    if (bSeekTarget)
    {
        // Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
        const FVector MoveDirection = FVector(Target->GetActorLocation() - GetActorLocation()).GetClampedToMaxSize(1.0f);

        // Calculate  movement
        const FVector Movement = MoveDirection * MoveSpeed * DeltaTime;

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
}

void AEnemyShip::OnMeshCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // If its a projectile, we take some damage
    // If its a player, we deal some damage
    if (AEnterAgoraDevTestProjectile* projectile = Cast<AEnterAgoraDevTestProjectile>(OtherActor))
    {
        Health -= 50;
        if (Health <= 0)
        {
            if (GetWorld())
            {
                AMyGameModeBase* gameMode = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
                if (gameMode)
                {
                    if (projectile->bIsP0) gameMode->P0Score++;
                    else gameMode->P1Score++;
                }
            }
            
            Destroy();
        }
    }
    else if (APlayerPawn* playerPawn = Cast<APlayerPawn>(OtherActor))
    {
        playerPawn->DealDamage(Damage);
    }
}

void AEnemyShip::PickTarget()
{
    Target = nullptr;
    float lastDistance = 0;

    for (TActorIterator<APlayerPawn> Itr(GetWorld()); Itr; ++Itr)
    {
        if (Itr->IsAlive())
        {
            if (Target == nullptr)
            {
                //Target = Cast<APlayerPawn>(Itr);
                Target = *Itr;
                lastDistance = FVector(Target->GetActorLocation() - GetActorLocation()).Size();
            }
            else
            {
                float itrDistance = FVector(Itr->GetActorLocation() - GetActorLocation()).Size();
                if (itrDistance < lastDistance)
                {
                    Target = *Itr;
                    lastDistance = itrDistance;
                }
            }
        }
    }

    bSeekTarget = Target != nullptr;
}

