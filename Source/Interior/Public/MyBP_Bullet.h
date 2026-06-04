// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "MyBP_Bullet.generated.h"

UCLASS()
class INTERIOR_API AMyBP_Bullet : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyBP_Bullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

};
