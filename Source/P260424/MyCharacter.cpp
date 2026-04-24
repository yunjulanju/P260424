// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator(0, -90.0f, 0));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (UIC)
	{
		UIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		UIC->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AMyCharacter::Turn);

		UIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AMyCharacter::Jump);
		UIC->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &AMyCharacter::StopJumping);
		UIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AMyCharacter::StopJumping);
	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Direction = Value.Get<FVector2D>();

	FRotator CameraRotation = GetControlRotation();
	FRotator CameraRotationFloor = FRotator(0, CameraRotation.Yaw, 0);
	FVector CameraForwrad = UKismetMathLibrary::GetForwardVector(CameraRotationFloor);
	FVector CameraRight = UKismetMathLibrary::GetRightVector(CameraRotationFloor);

	AddMovementInput(CameraForwrad * Direction.X);
	AddMovementInput(CameraRight * Direction.Y);
}

void AMyCharacter::Turn(const FInputActionValue& Value)
{
	FVector2D Rotation = Value.Get<FVector2D>();

	AddControllerPitchInput(Rotation.Y);
	AddControllerYawInput(Rotation.X);
}

