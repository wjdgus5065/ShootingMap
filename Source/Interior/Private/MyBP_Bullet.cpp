// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBP_Bullet.h"

// Sets default values
AMyBP_Bullet::AMyBP_Bullet()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyBP_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBP_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyBP_Bullet::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

