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
    void Log(const float Value)
    {
        Log(FString::SanitizeFloat(Value));
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void Log(const FVector& Value)
    {
        Log(Value.ToString());
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void Log(const FRotator& Value)
    {
        Log(Value.ToString());
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void Log(const bool bValue)
    {
        FString result = bValue ? "true" : "false";
        Log(result);
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

    //------------------------------------------------------------
    //------------------------------------------------------------
    FVector SamplePointFromDiskOnXYPlane(const float Radius, const FVector& Center)
    {
        constexpr float QLTwoPi = 2.0f * PI;

        // sample a point in a circle
        // reference:
        // - http://mathworld.wolfram.com/DiskPointPicking.html
        // - https://www.arl.army.mil/arlreports/2015/ARL-TR-7333.pdf
        float RandomTheta = FMath::RandRange(0.0f, QLTwoPi);
        float RandomU = FMath::RandRange(0.0f, Radius * Radius);

        float RandomRadius = FMath::Sqrt(RandomU);
        float x = Center.X + RandomRadius * FMath::Cos(RandomTheta);
        float y = Center.Y + RandomRadius * FMath::Sin(RandomTheta);

        return FVector(x, y, Center.Z);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    FVector SamplePointFromSquareOnXYPlane(const float XHalfSide, const float YHalfSide, const FVector& Center)
    {
        float x = Center.X + FMath::RandRange(-XHalfSide, XHalfSide);
        float y = Center.Y + FMath::RandRange(-YHalfSide, YHalfSide);
        return FVector(x, y, Center.Z);
    }
}

