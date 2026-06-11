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

	CollisionComp =
		CreateDefaultSubobject<UBoxComponent>(
			TEXT("CollisionComp"));

	RootComponent = CollisionComp;

	CollisionComp->SetCollisionProfileName(
		TEXT("OverlapAllDynamic"));

	CollisionComp->SetGenerateOverlapEvents(true);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(
		this,
		&AMyBP_Boss::OnOverlap
	);
}

void AMyBP_Boss::BeginPlay()
{
	Super::BeginPlay();

	BossHP = BossMaxHP;

	Player =
		Cast<AMyBP_Player>(
			UGameplayStatics::GetPlayerPawn(
				GetWorld(),
				0));

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

	if (BossHP <= BossMaxHP * 0.5f)
	{
		BossState = EBossState::Phase2;
	}

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