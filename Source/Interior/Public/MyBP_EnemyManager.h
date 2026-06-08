#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBP_EnemyManager.generated.h"

class AMyBP_Boss;
class AMyBP_Enemy;
class AActor;

UCLASS()
class INTERIOR_API AMyBP_EnemyManager : public AActor
{
	GENERATED_BODY()

public:
	AMyBP_EnemyManager();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Spawn")
	float CreateTime = 2.f;

	UPROPERTY(EditAnywhere, Category="Spawn")
	float CurrentTime = 0.f;

	UPROPERTY(EditAnywhere, Category="Spawn")
	TSubclassOf<AMyBP_Enemy> EnemyClass;

	UPROPERTY(EditAnywhere, Category="Boss")
	TSubclassOf<AMyBP_Boss> BossClass;

	UPROPERTY(EditAnywhere, Category="Boss")
	AActor* BossSpawnRef;

	UPROPERTY(EditAnywhere, Category="Boss")
	bool CanSpawnBoss = true;

	UPROPERTY(BlueprintReadOnly)
	bool bBossSpawned = false;

	UPROPERTY(BlueprintReadOnly)
	int32 KillCount = 0;

	UFUNCTION()
	void EnemyKilled();
};