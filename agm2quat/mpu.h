#ifndef __MPU_H
#define __MPU_H

#define ENABLEQUATERNION 1
#define MPU_GEOMETRY_BENCH 0

// Families of modes
// The number is built using a bitmask: qmgal (l=low power)
#define MPU_MODE_BM_LP 1
#define MPU_MODE_BM_A 2
#define MPU_MODE_BM_G 4
#define MPU_MODE_BM_M 8
#define MPU_MODE_BM_Q 16
#define MPU_MODE_BM_E 32
#define MPU_MODE_BM_DBG 64
#define MPU_MODE_BM_QDBG 128
#define MPU_MODE_GYR 		MPU_MODE_BM_G
#define MPU_MODE_ACC 		MPU_MODE_BM_A
#define MPU_MODE_ACCGYR 	(MPU_MODE_BM_A|MPU_MODE_BM_G)
#define MPU_MODE_LPACC 		(MPU_MODE_BM_A|MPU_MODE_BM_LP)
#define MPU_MODE_ACCGYRMAG 	(MPU_MODE_BM_A|MPU_MODE_BM_G|MPU_MODE_BM_M)
#define MPU_MODE_ACCGYRMAGQ (MPU_MODE_BM_A|MPU_MODE_BM_G|MPU_MODE_BM_M|MPU_MODE_BM_Q)
#define MPU_MODE_Q 			MPU_MODE_BM_Q
#define MPU_MODE_E 			MPU_MODE_BM_E
#define MPU_MODE_QDBG		MPU_MODE_BM_QDBG

// Gyro scale
#define MPU_GYR_SCALE_250 0
#define MPU_GYR_SCALE_500 1
#define MPU_GYR_SCALE_1000 2
#define MPU_GYR_SCALE_2000 3


typedef struct {
	signed short ax,ay,az;
	signed short gx,gy,gz;
	signed short mx,my,mz;
	unsigned char ms;
	signed short temp;
	unsigned long int time;
	unsigned long packetctr;
} MPUMOTIONDATA;


typedef struct {
	float yaw,pitch,roll;		// Aerospace
	float alpha,x,y,z;			// Quaternion debug
	float q0,q1,q2,q3;			// Quaternion
} MPUMOTIONGEOMETRY;

extern float mpu_gtorps;
extern unsigned char sample_mode;

void mpu_init_gtorps(int level);
void mpu_madgwick_init_magnodownsample(float _mpu_samplerate,float _mpu_beta);
void mpu_madgwick_init_magdownsample(float _mpu_samplerate,float _mpu_beta);

#endif
