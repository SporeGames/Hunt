#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "OcclusionAwarePlayerController.generated.h"

USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  const AActor* Actor;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  UStaticMeshComponent* StaticMesh;
  
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TArray<UMaterialInterface*> Materials;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  bool IsOccluded;
};

/**
 * 
 */
UCLASS()
class HUNT_API AOcclusionAwarePlayerController : public APlayerController
{
  GENERATED_BODY()

public:
  AOcclusionAwarePlayerController();
  UFUNCTION(BlueprintCallable)
  void SimpleClientNavMove(const FVector& Destination);
  UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
  void CancelActiveMovement();
  UFUNCTION(BlueprintCallable)
  /** Common request to nagivate player to the given world location. */
  void RequestSetNewMoveDestination(const FVector DestLocation);

  /** Call navigate player to the given world location (Client Version). */
  UFUNCTION(Reliable, Client)
  void ClientSetNewMoveDestination(const FVector DestLocation);

  /** Call navigate player to the given world location (Server Version). */
  UFUNCTION(Reliable, Server)
  void ServerSetNewMoveDestination(const FVector DestLocation);

  /** Navigate player to the given world location. */
  UFUNCTION(BlueprintCallable)
  void SetNewMoveDestination(const FVector DestLocation);

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

  /** How much of the Pawn capsule Radius and Height
   * should be used for the Line Trace before considering an Actor occluded?
   * Values too low may make the camera clip through walls.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion",
    meta=(ClampMin="0.1", ClampMax="10.0") )
  float CapsulePercentageForTrace;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Materials")
  UMaterialInterface* FadeMaterial;

  UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
  class USpringArmComponent* ActiveSpringArm;

  UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
  class UCameraComponent* ActiveCamera;

  UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
  class UCapsuleComponent* ActiveCapsuleComponent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion")
  bool IsOcclusionEnabled;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion")
  bool DebugLineTraces;
  
private:
  TMap<const AActor*, FCameraOccludedActor> OccludedActors;
  
  bool HideOccludedActor(const AActor* Actor);
  bool OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const;
  void ShowOccludedActor(FCameraOccludedActor& OccludedActor);
  bool OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const;
  void ForceShowOccludedActors();
  void OnPossess(APawn* InPawn) override;

  __forceinline bool ShouldCheckCameraOcclusion() const
  {
    return IsOcclusionEnabled && FadeMaterial && ActiveCamera && ActiveCapsuleComponent;
  }
  
public:
  UFUNCTION(BlueprintCallable)
  void SyncOccludedActors();
};