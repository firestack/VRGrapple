// Fill out your copyright notice in the Description page of Project Settings.

#include "VRTest.h"
#include "VRTestGameMode.h"

#include "VRPawn.h"


AVRTestGameMode::AVRTestGameMode() {
	// Set Default VR Pawn Class so that it will never be None
	DefaultVRPawnClass = AVRPawn::StaticClass();
}

APawn* AVRTestGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	// Select Correct Spawn Position based on VR mode
	FVector StartLocation = StartSpot->GetActorLocation();
	if (ShouldUseVR()) {

		// Take origin location and subtract the bounds of the capsule of the playerstart actor
		StartLocation.Z -= StartSpot->GetRootComponent()->Bounds.BoxExtent.Z;
		DrawDebugPoint(GetWorld(), StartLocation, 10.0f, FColor::Red, true);
	}

	// don't allow pawn to be spawned with any pitch or roll
	FRotator StartRotation(ForceInit);
	StartRotation.Yaw = StartSpot->GetActorRotation().Yaw;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
	APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, StartLocation, StartRotation, SpawnInfo);
	if (ResultPawn == NULL)
	{
		//UE_LOG(LogGameMode, Warning, TEXT("Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *StartSpot->GetName()); 
	}
	return ResultPawn;
}

bool AVRTestGameMode::ShouldUseVR_Implementation()
{	
	// Simple overrideable check to test if this game mode will use VR when spawning the player
	return (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() && UseVR) || ForceVR;
}

UClass * AVRTestGameMode::GetDefaultPawnClassForController_Implementation(AController * InController)
{	
	// Terary operator to return the class to use when spawning the pawn for the player
	return (ShouldUseVR() ? DefaultVRPawnClass : DefaultPawnClass);
}

