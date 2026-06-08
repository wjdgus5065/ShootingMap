#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyBP_Enemy.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class USoundBase;
class AMyBP_EnemyManager; 

UCLASS()
class INTERIOR_API AMyBP_Enemy : public APawn
{
	GENERATED_BODY()

public:
	AMyBP_Enemy();

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
	AActor* Target = nullptr;

	bool bIsDead = false;

public:

	// EnemyManager 참조
	UPROPERTY()
	AMyBP_EnemyManager* EnemyManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float Speed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* DeathEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundBase* ExplosionSound;
};