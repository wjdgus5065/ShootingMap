#include "MyBP_EnemyManager.h"

#include "MyBP_Enemy.h"
#include "MyBP_Boss.h"

AMyBP_EnemyManager::AMyBP_EnemyManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyBP_EnemyManager::BeginPlay()
{
	Super::BeginPlay();
}

void AMyBP_EnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 보스가 생성되었으면 일반 적 생성 중지
	if (bBossSpawned)
	{
		return;
	}

	CurrentTime += DeltaTime;

	if (CurrentTime >= CreateTime)
	{
		CurrentTime = 0.f;

		if (!EnemyClass)
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("EnemyClass NULL")
			);
			return;
		}

		FVector SpawnLocation(0.f, 0.f, 930.f);

		AMyBP_Enemy* Enemy =
			GetWorld()->SpawnActor<AMyBP_Enemy>(
				EnemyClass,
				SpawnLocation,
				FRotator::ZeroRotator
			);

		if (Enemy)
		{
			Enemy->EnemyManagerRef = this;

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Enemy Spawned")
			);
		}
	}
}

void AMyBP_EnemyManager::EnemyKilled()
{
	KillCount++;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Kill Count : %d"),
		KillCount
	);

	if (KillCount >= 10 &&
		!bBossSpawned &&
		CanSpawnBoss)
	{
		if (!BossClass)
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("BossClass NULL")
			);
			return;
		}

		if (!BossSpawnRef)
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("BossSpawnRef NULL")
			);
			return;
		}

		AMyBP_Boss* Boss =
			GetWorld()->SpawnActor<AMyBP_Boss>(
				BossClass,
				BossSpawnRef->GetActorLocation(),
				FRotator::ZeroRotator
			);

		if (Boss)
		{
			bBossSpawned = true;

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Boss Spawned!")
			);
		}
	}
}