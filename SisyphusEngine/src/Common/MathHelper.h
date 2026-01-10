#pragma once
#include <DirectXMath.h>

namespace MathHelper {
    // 수학 상수
    const float PI = 3.1415926535f;
    const float DEG_TO_RAD = PI / 180.0f;
    const float RAD_TO_DEG = 180.0f / PI;

    // 물리/이동 기본 설정
    const float DEFAULT_MOVE_SPEED = 50.0f;
    const float DEFAULT_TURN_SPEED = 150.0f;
    const float DEFAULT_LOOK_SPEED = 75.0f;
    const float FRICTION_RATIO = 0.5f;

    const float MOVE_ACCEL = 1.0f;  // 이동 가속도
    const float MOVE_BRAKE = 0.5f;  // 이동 감속도 (마찰력)
    const float VERTICAL_MOVE_SPEED = 15.0f;

    const float TURN_ACCEL = 5.0f;  // 회전 가속도
    const float TURN_BRAKE = 3.5f;  // 회전 감속도
    const float LOOK_ACCEL = 7.5f;  // 상하 시선 가속도
    const float LOOK_BRAKE = 2.0f; // 상하 시선 감속도

    // 변환 함수
    inline float ToRadians(float degrees) { return degrees * DEG_TO_RAD; }
    inline float ToDegrees(float radians) { return radians * RAD_TO_DEG; }


    template <typename T>
    T clamp(T value, T min_val, T max_val)
    {
        if (value < min_val)
            return min_val;
       
        else if (value > max_val)
            return max_val;
        
        else 
            return value;
    } // clamp

} // MathHelper