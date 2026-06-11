#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyBP_Boss.generated.h"

class AMyBP_Player;
class AMyBP_BossBullet;
class AMyBP_Bullet;
class UBoxComponent;
class UNiagaraSystem;
class USoundBase;

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Phase1,
	Phase2
};

UCLASS()
class INTERIOR_API AMyBP_Boss : public APawn
{
	GENERATED_BODY()

public:
	AMyBP_Boss();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:

	UPROPERTY()
	AMyBP_Player* Player;

	FTimerHandle BossLoopHandle;

	void BossLoopFunc();

public:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere)
	float BossMaxHP = 100.f;

	UPROPERTY(EditAnywhere)
	float BossHP = 100.f;

	UPROPERTY(EditAnywhere)
	float FireInterval = 1.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyBP_BossBullet> BossBulletClass;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DeathEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	EBossState BossState = EBossState::Phase1;

	UPROPERTY(EditAnywhere)
	bool bBossDead = false;

	void FireStraight();
	void FireSpread();
	void BossTakeDamage(float Damage);
};