#include "MyBP_Player.h"
#include "MyBP_Bullet.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// 생성자
AMyBP_Player::AMyBP_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	// 루트 생성 (필수)
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

// 시작
void AMyBP_Player::BeginPlay()
{
	Super::BeginPlay();
}

// 이동
void AMyBP_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;
	SetActorLocation(NewLocation);
}

// 좌우
void AMyBP_Player::MoveHorizontal(float Value)
{
	Direction.Y = Value;
}

// 앞뒤
void AMyBP_Player::MoveVertical(float Value)
{
	Direction.Z = Value;
}

// 입력
void AMyBP_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Horizontal", this, &AMyBP_Player::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AMyBP_Player::MoveVertical);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyBP_Player::FireBullet);
}

// 🔥 총알 발사
void AMyBP_Player::FireBullet()
{
	if (!GetWorld() || !BulletClass)
		return;

	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.X = 0.f;
	SpawnLocation.Y = GetActorLocation().Y;
	SpawnLocation.Z += 110.f;

	AMyBP_Bullet* Bullet = GetWorld()->SpawnActor<AMyBP_Bullet>(
		BulletClass,
		SpawnLocation,
		FRotator::ZeroRotator
	);

	if (Bullet)
	{
		Bullet->Direction = GetActorUpVector();
	}
}

// 죽음
void AMyBP_Player::Player_Die()
{
	FVector Location = GetActorLocation();

	if (DeathEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, Location);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);
	}

	Destroy();
}