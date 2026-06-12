#include "MyBP_Enemy.h"

#include "MyBP_Player.h"
#include "MyBP_Bullet.h"
#include "MyBP_EnemyManager.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Sound/SoundBase.h"

AMyBP_Enemy::AMyBP_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 적 충돌체 생성
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// 충돌 이벤트 등록
	CollisionComp->OnComponentBeginOverlap.AddDynamic(
		this,
		&AMyBP_Enemy::OnOverlap
	);
}

void AMyBP_Enemy::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Players;

	// 플레이어 탐색
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AMyBP_Player::StaticClass(),
		Players
	);

	if (Players.Num() > 0)
	{
		Target = Players[0];

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Target Found : %s"),
			*Target->GetName()
		);
	}
	else
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Player Not Found!")
		);
	}
}

void AMyBP_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Target)
	{
		UE_LOG(LogTemp, Error, TEXT("Target NULL"));
		return;
	}

	// 플레이어 추적 이동
	FVector MoveDirection =
		(Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	FVector NewLocation =
		GetActorLocation() +
		MoveDirection * Speed * DeltaTime;

	SetActorLocation(NewLocation);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Enemy Pos : %s"),
		*GetActorLocation().ToString()
	);
}

void AMyBP_Enemy::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 이미 사망한 경우 무시
	if (bIsDead)
	{
		return;
	}

	if (!OtherActor)
	{
		return;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Overlap : %s"),
		*OtherActor->GetName()
	);

	// 플레이어와 충돌 시 플레이어 사망
	if (AMyBP_Player* Player = Cast<AMyBP_Player>(OtherActor))
	{
		Player->Player_Die();
		return;
	}

	// 총알과 충돌 시 적 사망
	if (AMyBP_Bullet* Bullet = Cast<AMyBP_Bullet>(OtherActor))
	{
		bIsDead = true;

		// 처치 수 증가
		if (EnemyManagerRef)
		{
			EnemyManagerRef->EnemyKilled();
		}

		// 추가 충돌 방지
		if (CollisionComp)
		{
			CollisionComp->SetCollisionEnabled(
				ECollisionEnabled::NoCollision
			);
		}

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

		// 총알 제거 후 적 제거
		Bullet->Destroy();
		Destroy();
	}
}