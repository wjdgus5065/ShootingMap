#include "MyBP_Bullet.h"
#include "Components/SphereComponent.h"

AMyBP_Bullet::AMyBP_Bullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 총알 충돌체 생성
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	CollisionComp->InitSphereRadius(10.f);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// 충돌 이벤트 등록
	CollisionComp->OnComponentBeginOverlap.AddDynamic(
		this,
		&AMyBP_Bullet::OnOverlap
	);

	// 기본 이동 설정
	Direction = FVector::UpVector;
	Speed = 1500.f;
}

void AMyBP_Bullet::BeginPlay()
{
	Super::BeginPlay();
}

void AMyBP_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 총알 이동
	FVector MoveDelta =
		Direction.GetSafeNormal() *
		Speed *
		DeltaTime;

	FHitResult Hit;

	AddActorWorldOffset(
		MoveDelta,
		true,
		&Hit
	);
}

// 충돌 처리
void AMyBP_Bullet::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 자기 자신 무시
	if (!OtherActor || OtherActor == this)
	{
		return;
	}
}