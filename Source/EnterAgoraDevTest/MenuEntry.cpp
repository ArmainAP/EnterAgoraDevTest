#include "MenuEntry.h"
#include <Engine/CollisionProfile.h>
#include "TemplateDefaults/EnterAgoraDevTestProjectile.h"
#include <Components/TextRenderComponent.h>

// Sets default values
AMenuEntry::AMenuEntry()
{
    // Create the mesh component, Static Mesh is set in BP
    ButtonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Shape"));
    RootComponent = ButtonMeshComponent;
    ButtonMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
    ButtonMeshComponent->OnComponentHit.AddDynamic(this, &AMenuEntry::OnButtonMeshComponentHit);

    // Create the text component
    ButtonTextComponet = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Button Text"));
    ButtonTextComponet->SetupAttachment(ButtonMeshComponent);
}

void AMenuEntry::OnButtonMeshComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //Respond only to the Default Projectile.
    if (Cast<AEnterAgoraDevTestProjectile>(OtherActor))
    {
        ExecuteMenuAction();
    }
}
