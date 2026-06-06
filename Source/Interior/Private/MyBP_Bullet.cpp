#include "MyBP_Bullet.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

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
}

void AMyBP_Bullet::BeginPlay()
{
	Super::BeginPlay();
}

void AMyBP_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(Direction * Speed * DeltaTime, true);
}

void AMyBP_Bullet::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
		return;

	UGameplayStatics::ApplyDamage(
		OtherActor,
		Damage,
		nullptr,
		this,
		nullptr
	);

	Destroy();
}