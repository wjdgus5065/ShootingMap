// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBP_Boss.h"

// Sets default values
AMyBP_Boss::AMyBP_Boss()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyBP_Boss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBP_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyBP_Boss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

