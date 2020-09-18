// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

class UUserWidget;

/**
 * Parent class intended for all the play game modes.
 */
UCLASS()
class ENTERAGORADEVTEST_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    void TogglePauseMenuVisibility();
	
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Classes")
    TSubclassOf<UUserWidget> PauseMenu_WidgetClass;

private:
    UPROPERTY()
    UUserWidget* PauseMenu_Widget;
};
