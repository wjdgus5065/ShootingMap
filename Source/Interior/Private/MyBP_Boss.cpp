#include "MyBP_Boss.h"

#include "MyBP_Player.h"
#include "MyBP_BossBullet.h"
#include "MyBP_Bullet.h"

#include "Components/BoxComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "TimerManager.h"

AMyBP_Boss::AMyBP_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	// 보스 충돌체 생성
	CollisionComp =
		CreateDefaultSubobject<UBoxComponent>(
			TEXT("CollisionComp"));

	RootComponent = CollisionComp;

	CollisionComp->SetCollisionProfileName(
		TEXT("OverlapAllDynamic"));

	CollisionComp->SetGenerateOverlapEvents(true);

	// 충돌 이벤트 등록
	CollisionComp->OnComponentBeginOverlap.AddDynamic(
		this,
		&AMyBP_Boss::OnOverlap
	);
}

void AMyBP_Boss::BeginPlay()
{
	Super::BeginPlay();

	// HP 초기화
	BossHP = BossMaxHP;

	// 플레이어 참조
	Player =
		Cast<AMyBP_Player>(
			UGameplayStatics::GetPlayerPawn(
				GetWorld(),
				0));

	// 공격 루프 시작
	GetWorldTimerManager().SetTimer(
		BossLoopHandle,
		this,
		&AMyBP_Boss::BossLoopFunc,
		FireInterval,
		true);
}

void AMyBP_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 보스 공격 패턴 실행
void AMyBP_Boss::BossLoopFunc()
{
	if (bBossDead)
	{
		return;
	}

	if (BossState == EBossState::Phase1)
	{
		FireStraight();
	}
	else
	{
		FireSpread();
	}
}

// 1페이즈 직선 공격
void AMyBP_Boss::FireStraight()
{
	if (!Player || !BossBulletClass)
	{
		return;
	}

	FVector Direction =
		(Player->GetActorLocation() -
		 GetActorLocation()).GetSafeNormal();

	AMyBP_BossBullet* Bullet =
		GetWorld()->SpawnActor<AMyBP_BossBullet>(
			BossBulletClass,
			GetActorLocation(),
			FRotator::ZeroRotator);

	if (Bullet)
	{
		Bullet->Direction = Direction;
	}
}

// 2페이즈 확산 공격
void AMyBP_Boss::FireSpread()
{
	if (!Player || !BossBulletClass)
	{
		return;
	}

	FVector BaseDir =
		(Player->GetActorLocation() -
		 GetActorLocation()).GetSafeNormal();

	for (int32 Angle : { -30, 0, 30 })
	{
		FVector Dir =
			FRotator(0.f, Angle, 0.f)
			.RotateVector(BaseDir);

		AMyBP_BossBullet* Bullet =
			GetWorld()->SpawnActor<AMyBP_BossBullet>(
				BossBulletClass,
				GetActorLocation(),
				FRotator::ZeroRotator);

		if (Bullet)
		{
			Bullet->Direction = Dir;
		}
	}
}

// 보스 데미지 처리
void AMyBP_Boss::BossTakeDamage(float Damage)
{
	if (bBossDead)
	{
		return;
	}

	BossHP -= Damage;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Boss HP : %.1f"),
		BossHP
	);

	// HP 50% 이하 시 2페이즈 진입
	if (BossHP <= BossMaxHP * 0.5f)
	{
		BossState = EBossState::Phase2;
	}

	// 보스 사망
	if (BossHP <= 0.f)
	{
		bBossDead = true;

		if (DeathEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				DeathEffect,
				GetActorLocation()
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
}

// 총알 충돌 처리
void AMyBP_Boss::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	if (AMyBP_Bullet* Bullet = Cast<AMyBP_Bullet>(OtherActor))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Boss Hit!")
		);

		BossTakeDamage(Bullet->Damage);

		Bullet->Destroy();
	}
}