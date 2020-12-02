#include "FGPlayer.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameframework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/NetDriver.h"
#include "GameFramework/PlayerState.h"
#include "FG19Networking/Component/FGMovementComponent.h"
#include "FG19Networking/FGMovementStatics.h"


AFGPlayer::AFGPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->SetupAttachment(CollisionComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	MovementComponent = CreateDefaultSubobject<UFGMovementComponent>(TEXT("MovementComponent"));

	SetReplicateMovement(false);
}
void AFGPlayer::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent->SetUpdatedComponent(CollisionComponent);
}

void AFGPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float Friction = IsBraking() ? BrakingFriction : DefaultFriction;
	const float Alpha = FMath::Clamp(FMath::Abs(MovementVelocity / (MaxVelocity * 0.75f)), 0.0f, 1.0f);
	const float TurnSpeed = FMath::InterpEaseOut(0.0f, TurnSpeedDefault, Alpha, 5.0f);
	const float MovementDirection = MovementVelocity > 0.0f ? Turn : -Turn;

	Yaw += (MovementDirection * TurnSpeed) * DeltaSeconds;
	FQuat WantedFacingDirection = FQuat(FVector::UpVector, FMath::DegreesToRadians(Yaw));
	MovementComponent->SetFacingRotation(WantedFacingDirection);

	FFGFrameMovement FrameMovement = MovementComponent->CreateFrameMovement();

	MovementVelocity += Forward * Acceleration * DeltaSeconds;
	MovementVelocity = FMath::Clamp(MovementVelocity, -MaxVelocity, MaxVelocity);
	MovementVelocity *= FMath::Pow(Friction, DeltaSeconds);

	MovementComponent->ApplyGravity();
	FrameMovement.AddDelta(GetActorForwardVector() * MovementVelocity * DeltaSeconds);
	MovementComponent->Move(FrameMovement);

	Server_SendLocation(GetActorLocation());
	//Server_SendRotation(GetActorRotation());
}

void AFGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Accelerate"), this, &AFGPlayer::Handle_Accelerate);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFGPlayer::Handle_Turn);

	PlayerInputComponent->BindAction(TEXT("Brake"), IE_Pressed, this, &AFGPlayer::Brake_Pressed);
	PlayerInputComponent->BindAction(TEXT("Brake"), IE_Released, this, &AFGPlayer::BrakeReleased);
}

int32 AFGPlayer::GetPing() const
{
	if (GetPlayerState())
	{
		return static_cast<int32>(GetPlayerState()->GetPing());
	}
	return 0;
}



//void AFGPlayer::Server_SendRotation_Implementation(const FRotator& RotationToSend)
//{
//	Multicast_SendRotation(RotationToSend);
//}
//
//void AFGPlayer::Multicast_SendRotation_Implementation(const FRotator& RotationToSend)
//{
//	SetActorRotation(RotationToSend);
//}

void AFGPlayer::Multicast_SendLocation_Implementation(const FVector& LocationToSend)
{
	if (!IsLocallyControlled())
	{
		SetActorLocation(LocationToSend);
	}
	
}


void AFGPlayer::Server_SendLocation_Implementation(const FVector& LocationToSend)
{
	Multicast_SendLocation(LocationToSend);
}


void AFGPlayer::Handle_Accelerate(float Value)
{
	Forward = Value;
}

void AFGPlayer::Handle_Turn(float Value)
{
	Turn = Value;
}

void AFGPlayer::Brake_Pressed()
{
	bBrake = true;
}

void AFGPlayer::BrakeReleased()
{
	bBrake = false;
}



