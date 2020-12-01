// Fill out your copyright notice in the Description page of Project Settings.


#include "FGPlayer.h"

// Sets default values
AFGPlayer::AFGPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFGPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

