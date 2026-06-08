#include "MyBP_Bullet.h"
#include "Components/SphereComponent.h"

AMyBP_Bullet::AMyBP_Bullet()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	CollisionComp->InitSphereRadius(10.f);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	CollisionComp->OnComponentBeginOverlap.AddDynamic(
		this,
		&AMyBP_Bullet::OnOverlap
	);

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