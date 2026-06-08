#include "MyBP_Player.h"
#include "MyBP_Bullet.h"

#include "Components/SphereComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AMyBP_Player::AMyBP_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	CollisionComp->InitSphereRadius(50.f);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComp->SetGenerateOverlapEvents(true);
}

void AMyBP_Player::BeginPlay()
{
	Super::BeginPlay();
}

void AMyBP_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;
	SetActorLocation(NewLocation);
}

void AMyBP_Player::MoveHorizontal(float Value)
{
	Direction.Y = Value;
}

void AMyBP_Player::MoveVertical(float Value)
{
	Direction.Z = Value;
}

void AMyBP_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Horizontal", this, &AMyBP_Player::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AMyBP_Player::MoveVertical);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyBP_Player::FireBullet);
}

void AMyBP_Player::FireBullet()
{
	if (!BulletClass)
	{
		return;
	}

	FVector SpawnLocation =
		GetActorLocation() +
		GetActorUpVector() * 120.f;

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;

	AMyBP_Bullet* Bullet =
		GetWorld()->SpawnActor<AMyBP_Bullet>(
			BulletClass,
			SpawnLocation,
			GetActorRotation(),
			Params
		);

	if (Bullet)
	{
		Bullet->Direction = GetActorUpVector();
		Bullet->Speed = 1500.f;
	}
}

void AMyBP_Player::Player_Die()
{
	FVector Location = GetActorLocation();

	if (DeathEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DeathEffect,
			Location
		);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			ExplosionSound
		);
	}

	Destroy();
}