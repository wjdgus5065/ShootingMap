#include "MyBP_BossBullet.h"

#include "MyBP_Player.h"

#include "Components/BoxComponent.h"

AMyBP_BossBullet::AMyBP_BossBullet()
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
		&AMyBP_BossBullet::OnOverlap
	);
}

void AMyBP_BossBullet::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(10.f);
}

void AMyBP_BossBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector MoveDelta =
		Direction.GetSafeNormal()
		* Speed
		* DeltaTime;

	SetActorLocation(
		GetActorLocation() + MoveDelta
	);
}

void AMyBP_BossBullet::OnOverlap(
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

	if (AMyBP_Player* Player =
		Cast<AMyBP_Player>(OtherActor))
	{
		Player->Player_Die();

		Destroy();
	}
}