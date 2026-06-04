// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBP_Player.h"
#include "MyBP_Bullet.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyBP_Player::AMyBP_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FirePosition =
		CreateDefaultSubobject<USceneComponent>(
			TEXT("FirePosition"));

	RootComponent = FirePosition;

}

// Called when the game starts or when spawned
void AMyBP_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBP_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Velocity =
		Direction * Speed;

	FVector NewLocation =
		GetActorLocation()
		+ Velocity * DeltaTime;

	SetActorLocation(NewLocation);
}

void AMyBP_Player::MoveHorizontal(float Value)
{
	Direction.Y = Value;    // A,D
}

void AMyBP_Player::MoveVertical(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Vertical %f"), Value);
	
	Direction.Z = Value;    // W,S
}

// Called to bind functionality to input
void AMyBP_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(
	"Horizontal",
	this,
	&AMyBP_Player::MoveHorizontal);

	PlayerInputComponent->BindAxis(
		"Vertical",
		this,
		&AMyBP_Player::MoveVertical);
	
	PlayerInputComponent->BindAction(
		"Fire",
		IE_Pressed,
		this,
		&AMyBP_Player::FireBullet);
	
}
void AMyBP_Player::FireBullet()
{
	if (!BulletClass)
		return;

	FVector SpawnLocation =
		FirePosition->GetComponentLocation();

	FRotator SpawnRotation =
		FirePosition->GetComponentRotation();

	GetWorld()->SpawnActor<AMyBP_Bullet>(
		BulletClass,
		SpawnLocation,
		SpawnRotation
	);
}

void AMyBP_Player::Player_Die()
{
	FVector Location = GetActorLocation();

	// 이펙트 생성
	if (DeathEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DeathEffect,
			Location
		);
	}

	// 사운드 재생
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			ExplosionSound
		);
	}

	// 자기 자신 삭제
	Destroy();
}


