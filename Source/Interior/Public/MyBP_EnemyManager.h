#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBP_EnemyManager.generated.h"

class AMyBP_Boss;
class AMyBP_Enemy;

UCLASS()
class INTERIOR_API AMyBP_EnemyManager : public AActor
{
	GENERATED_BODY()

public:
	AMyBP_EnemyManager();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 적 생성 주기
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float CreateTime = 2.f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float CurrentTime = 0.f;

	// 일반 적 클래스
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AMyBP_Enemy> EnemyClass;

	// 보스 클래스
	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<AMyBP_Boss> BossClass;

	// 보스 생성 가능 여부
	UPROPERTY(EditAnywhere, Category = "Boss")
	bool CanSpawnBoss = true;

	// 보스 생성 여부
	UPROPERTY(BlueprintReadOnly)
	bool bBossSpawned = false;

	// 처치 수
	UPROPERTY(BlueprintReadOnly)
	int32 KillCount = 0;

	UFUNCTION()
	void EnemyKilled();
};