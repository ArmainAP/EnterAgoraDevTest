#include "SurvivalPlayerController.h"
#include "../PlayerPawn.h"
#include "../StaticBindingsLibrary.h"

void ASurvivalPlayerController::BindPawnInput()
{
    Super::BindPawnInput();

    InputComponent->BindAxis(UStaticBindingsLibrary::FireP1_Binding, this, &ASurvivalPlayerController::OnP2Fire);
}

void ASurvivalPlayerController::OnP2Fire(float Value)
{
    if (Value != 0.0f &&
        P1 == nullptr)
    {
        P1 = SpawnP1(FVector(0, 0, 300), FRotator::ZeroRotator);
    }
}
