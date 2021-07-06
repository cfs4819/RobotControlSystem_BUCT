#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"

#define ACTGROUP_SECTOR 254
#define ACTGROUP_OFFSET 0
#define ACTGROUP_SIZE	20

#define ACTGROUPTORUN_SECTOR 253
#define ACTGROUPTORUN_OFFSET 0
#define ACTGROUP_DEMO_ENABLE_OFFSET 20
#define ACTGROUP_DEMO_INFLASH_OFFSET 24

typedef struct{
	int angle_new;
	int angle_old;
	int runtime;
}myservo;

void demo_group(void);

#endif

