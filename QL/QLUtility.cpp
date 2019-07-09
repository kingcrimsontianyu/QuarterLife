//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLUtility.h"
#include "Engine/Engine.h"

namespace QLUtility
{
    //------------------------------------------------------------
    //------------------------------------------------------------
    void Log(const FString& msg)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void Screen(const FString& msg)
    {
        //bool bNewerOnTop = true;
        //FVector2D TextScale(1.0f, 1.0f);
        //GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Cyan, msg, bNewerOnTop, TextScale);

        bool bNewerOnTop = false;
        FVector2D TextScale(1.0f, 1.0f);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, msg, bNewerOnTop, TextScale);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void MakePredictionShot(
        FVector& LocationProjectileHitsEnemy,
        float& TimeProjectileHitsEnemy,
        const FVector MyLocation,
        const FVector EnemyLocation,
        const FVector EnemyVelocity,
        const float ProjectileSpeed) // m
    {
        float EnemySpeedSquared = EnemyVelocity.SizeSquared();
        float ProjectileSpeedSquared = ProjectileSpeed * ProjectileSpeed;

        // If the enemy speed is no less than the projectile speed
        // it is possible that the projectile does not reach enemy.
        // For simplicity, we simply aim at the enemy initial location
        if (EnemySpeedSquared >= ProjectileSpeedSquared)
        {
            LocationProjectileHitsEnemy = EnemyLocation;
            TimeProjectileHitsEnemy = -1.0;
            return;
        }

        FVector DistanceVector = EnemyLocation - MyLocation;

        // Special case: the enemy nearly overlaps the player
        if (DistanceVector.IsNearlyZero())
        {
            LocationProjectileHitsEnemy = EnemyLocation;
            TimeProjectileHitsEnemy = 0.0f;
            return;
        }

        float Distance = DistanceVector.Size();

        // Special case: the enemy barely moves
        if (EnemyVelocity.IsNearlyZero())
        {
            LocationProjectileHitsEnemy = EnemyLocation;
            TimeProjectileHitsEnemy = Distance / ProjectileSpeed;
            return;
        }

        // General case
        float EnemyProjectedSpeed = FVector::DotProduct(EnemyVelocity, DistanceVector.GetSafeNormal()); // q

        float a = EnemySpeedSquared - ProjectileSpeed * ProjectileSpeed; // negative

        float b = 2.0f * Distance * EnemyProjectedSpeed;
        float c = Distance * Distance;

        float delta = b * b - 4.0f * a * c;
        float deltaSqrt = FMath::Sqrt(delta);
        float root = -b + deltaSqrt;

        if (root > 0.0f)
        {
            root = -b - deltaSqrt;
        }

        root /= 2.0f * a;

        TimeProjectileHitsEnemy = root;

        LocationProjectileHitsEnemy = EnemyLocation + EnemyVelocity * root;
    }
}

