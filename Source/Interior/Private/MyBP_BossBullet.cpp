#include "MyBP_BossBullet.h"

#include "MyBP_Player.h"

#include "Components/BoxComponent.h"

AMyBP_BossBullet::AMyBP_BossBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 보스 총알 충돌체 생성
	CollisionComp =
		CreateDefaultSubobject<UBoxComponent>(
			TEXT("CollisionComp"));

	RootComponent = CollisionComp;

	CollisionComp->SetCollisionProfileName(
		TEXT("OverlapAllDynamic"));

	CollisionComp->SetGenerateOverlapEvents(true);

	// 충돌 이벤트 등록
	CollisionComp->OnComponentBeginOverlap.AddDynamic(
		this,
		&AMyBP_BossBullet::OnOverlap
	);
}

void AMyBP_BossBullet::BeginPlay()
{
	Super::BeginPlay();

	// 일정 시간 후 자동 제거
	SetLifeSpan(10.f);
}

void AMyBP_BossBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 총알 이동
	FVector MoveDelta =
		Direction.GetSafeNormal()
		* Speed
		* DeltaTime;

	SetActorLocation(
		GetActorLocation() + MoveDelta
	);
}

// 플레이어 충돌 처리
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