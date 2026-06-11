#include "MyBP_Player.h"
#include "MyBP_Bullet.h"

#include "Components/SphereComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AMyBP_Player::AMyBP_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	// 플레이어 충돌체 생성
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

	// 플레이어 이동
	FVector NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;
	SetActorLocation(NewLocation);
}

// 좌우 이동 입력
void AMyBP_Player::MoveHorizontal(float Value)
{
	Direction.Y = Value;
}

// 상하 이동 입력
void AMyBP_Player::MoveVertical(float Value)
{
	Direction.Z = Value;
}

void AMyBP_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 입력 바인딩
	PlayerInputComponent->BindAxis("Horizontal", this, &AMyBP_Player::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AMyBP_Player::MoveVertical);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyBP_Player::FireBullet);
}

// 총알 발사
void AMyBP_Player::FireBullet()
{
	if (!BulletClass)
	{
		return;
	}

	// 플레이어 앞 위치에 생성
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

	// 총알 방향 및 속도 설정
	if (Bullet)
	{
		Bullet->Direction = GetActorUpVector();
		Bullet->Speed = 1500.f;
	}
}

// 플레이어 사망
void AMyBP_Player::Player_Die()
{
	FVector Location = GetActorLocation();

	// 사망 이펙트
	if (DeathEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DeathEffect,
			Location
		);
	}

	// 사망 사운드
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			ExplosionSound
		);
	}

	Destroy();
}