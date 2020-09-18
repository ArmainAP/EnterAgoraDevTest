#include "MyGameModeBase.h"
#include <Blueprint/UserWidget.h>

void AMyGameModeBase::TogglePauseMenuVisibility()
{
    if (PauseMenu_Widget)
    {
        bool isVisible = PauseMenu_Widget->GetVisibility() == ESlateVisibility::Visible;
        PauseMenu_Widget->SetVisibility(isVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
    }
}

void AMyGameModeBase::BeginPlay()
{
    // Create the User Widgets
    if (PauseMenu_WidgetClass)
    {
        PauseMenu_Widget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), PauseMenu_WidgetClass);
        if (PauseMenu_Widget)
        {
            PauseMenu_Widget->AddToViewport();
            PauseMenu_Widget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    if (GameUI_WidgetClass)
    {
        GameUI_Widget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), GameUI_WidgetClass);
        if (GameUI_Widget)
        {
            GameUI_Widget->AddToViewport();
        }
    }
}
