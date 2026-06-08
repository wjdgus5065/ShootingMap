#include "MyBP_Enemy.h"


#include "MyBP_Player.h"
#include "MyBP_Bullet.h"
#include "MyBP_EnemyManager.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Sound/SoundBase.h"

AMyBP_Enemy::AMyBP_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	CollisionComp->OnComponentBeginOverlap.AddDynamic(
		this,
		&AMyBP_Enemy::OnOverlap
	);
}

void AMyBP_Enemy::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Players;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AMyBP_Player::StaticClass(),
		Players
	);

	if (Players.Num() > 0)
	{
		Target = Players[0];

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Target Found : %s"),
			*Target->GetName()
		);
	}
	else
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Player Not Found!")
		);
	}
}

void AMyBP_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Target)
	{
		UE_LOG(LogTemp, Error, TEXT("Target NULL"));
		return;
	}

	FVector MoveDirection =
		(Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	FVector NewLocation =
		GetActorLocation() +
		MoveDirection * Speed * DeltaTime;

	SetActorLocation(NewLocation);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Enemy Pos : %s"),
		*GetActorLocation().ToString()
	);
}

void AMyBP_Enemy::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (bIsDead)
	{
		return;
	}

	if (!OtherActor)
	{
		return;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Overlap : %s"),
		*OtherActor->GetName()
	);

	if (AMyBP_Player* Player = Cast<AMyBP_Player>(OtherActor))
	{
		Player->Player_Die();
		return;
	}

	if (AMyBP_Bullet* Bullet = Cast<AMyBP_Bullet>(OtherActor))
	{
		bIsDead = true;

		// 적 처치 카운트 증가
		if (EnemyManagerRef)
		{
			EnemyManagerRef->EnemyKilled();
		}
		
		if (CollisionComp)
		{
			CollisionComp->SetCollisionEnabled(
				ECollisionEnabled::NoCollision
			);
		}

		FVector Location = GetActorLocation();

		if (DeathEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				DeathEffect,
				Location
			);
		}

		if (ExplosionSound)
		{
			UGameplayStatics::PlaySound2D(
				GetWorld(),
				ExplosionSound
			);
		}

		Bullet->Destroy();
		Destroy();
	}
}