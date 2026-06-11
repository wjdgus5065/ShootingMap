#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBP_BossBullet.generated.h"

class UBoxComponent;

UCLASS()
class INTERIOR_API AMyBP_BossBullet : public AActor
{
	GENERATED_BODY()

public:
	AMyBP_BossBullet();

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

public:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction = FVector::ForwardVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10.f;
};