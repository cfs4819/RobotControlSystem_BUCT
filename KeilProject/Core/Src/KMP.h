/*
 * KMP.h
 *
 *  Created on: 2020年12月4日
 *      Author: cfs
 */

#ifndef INC_KMP_H_
#define INC_KMP_H_

#include "main.h"

#define MaxSize 20

int KMP(uint8_t* s,int sLength,uint8_t* t,int tLength);
void Getnext(uint8_t* t,int tLength,int next[]);

#endif /* INC_KMP_H_ */
