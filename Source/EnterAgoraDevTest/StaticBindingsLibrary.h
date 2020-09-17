#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StaticBindingsLibrary.generated.h"

/**
 * Library intended to be used for input bindings names and functions.
 */
UCLASS()
class ENTERAGORADEVTEST_API UStaticBindingsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    // Static names for input bindings
    static const FName MoveForwardP0_Binding;
    static const FName MoveRightP0_Binding;
    static const FName FireP0_Binding;
    static const FName MoveForwardP1_Binding;
    static const FName MoveRightP1_Binding;
    static const FName FireP1_Binding;
};
