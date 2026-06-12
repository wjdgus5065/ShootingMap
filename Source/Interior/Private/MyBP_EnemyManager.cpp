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

	// 보스 생성 후 일반 적 생성 중지
	if (bBossSpawned)
	{
		return;
	}

	// 생성 시간 누적
	CurrentTime += DeltaTime;

	if (CurrentTime >= CreateTime)
	{
		CurrentTime = 0.f;

		// 적 클래스 확인
		if (!EnemyClass)
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("EnemyClass NULL")
			);
			return;
		}

		// 일반 적 생성
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

// 적 처치 시 호출
void AMyBP_EnemyManager::EnemyKilled()
{
	KillCount++;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Kill Count : %d"),
		KillCount
	);

	// 10킬 달성 시 보스 생성
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

		// 보스 생성
		FVector BossSpawnLocation(0.f, 0.f, 930.f);

		AMyBP_Boss* Boss =
			GetWorld()->SpawnActor<AMyBP_Boss>(
				BossClass,
				BossSpawnLocation,
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
		else
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("Boss Spawn FAILED")
			);
		}
	}
}