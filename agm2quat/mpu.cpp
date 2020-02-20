#include "mpu.h"
#include "mpu_geometry.h"
#include "MadgwickAHRS_float.h"

// Conversion from Gyro readings to rad/sec
float mpu_gtorps=3.14159665/180.0/131.072;
unsigned char sample_mode=MPU_MODE_Q;

void mpu_init_gtorps(int level)
{
    switch(level)
    {
        case MPU_GYR_SCALE_250:
            mpu_gtorps=3.14159665/180.0/131.072;
            break;
        case MPU_GYR_SCALE_500:
            mpu_gtorps=3.14159665/180.0/65.536;
            break;
        case MPU_GYR_SCALE_1000:
            mpu_gtorps=3.14159665/180.0/32.768;
            break;
        default:
            mpu_gtorps=3.14159665/180.0/16.384;
            break;
    }
}

void mpu_madgwick_init_magdownsample(float _mpu_samplerate,float _mpu_beta)
{
    MadgwickAHRSinit(_mpu_samplerate,_mpu_beta,(_mpu_samplerate/100)*8-1);			// All -> 12.5Hz
}
void mpu_madgwick_init_magnodownsample(float _mpu_samplerate,float _mpu_beta)
{
    MadgwickAHRSinit(_mpu_samplerate,_mpu_beta,0);          // Update always
}




