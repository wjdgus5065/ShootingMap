#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "MyBP_Player.generated.h"

class AMyBP_Bullet;

UCLASS()
class INTERIOR_API AMyBP_Player : public APawn
{
	GENERATED_BODY()

public:
	AMyBP_Player();

	// 이동 방향
	FVector Direction = FVector::ZeroVector;

	// 이동 속도
	UPROPERTY(EditAnywhere, Category = "Move")
	float Speed = 500.f;

	// 생성할 총알 클래스
	UPROPERTY(EditAnywhere, Category = "Bullet")
	TSubclassOf<AMyBP_Bullet> BulletClass;

	// 사망 이펙트
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* DeathEffect;

	// 사망 사운드
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ExplosionSound;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveHorizontal(float Value);
	void MoveVertical(float Value);
	void FireBullet();
	void Player_Die();
};