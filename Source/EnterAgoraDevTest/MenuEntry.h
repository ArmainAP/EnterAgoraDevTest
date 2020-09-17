#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MenuEntry.generated.h"

UCLASS()
class ENTERAGORADEVTEST_API AMenuEntry : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMenuEntry();

    /* The mesh component */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* ButtonMeshComponent;

	// The text component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ButtonTextComponet;

protected:
	// Executes the desired menu action, setup in blueprints.
	UFUNCTION(BlueprintImplementableEvent)
	void ExecuteMenuAction();

private:
	// Responds to the ButtonMeshComponent hit event.
    UFUNCTION()
    void OnButtonMeshComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
