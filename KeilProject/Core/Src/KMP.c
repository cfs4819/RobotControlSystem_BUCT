/*
 * KMP.c
 *
 *  Created on: 2020年12月4日
 *      Author: cfs
 */
#include "KMP.h"

//s：目标串 t：模式串
int KMP(uint8_t* s,int sLength,uint8_t* t,int tLength)
{
   int next[MaxSize],i=0,j=0;
   Getnext(t,tLength,next);
   while(i<sLength&&j<tLength)
   {
      if(j==-1 || s[i]==t[j])
      {
         i++;
         j++;
      }
      else j=next[j];               //j回退。。。
   }
   if(j>=tLength)
       return (i-tLength);         //匹配成功，返回子串的位置
   else
      return (-1);                  //没找到
}

void Getnext(uint8_t* t,int tLength,int next[])
{
   int j=0,k=-1;
   next[0]=-1;
   while(j<tLength-1)
   {
      if(k == -1 || t[j] == t[k])
      {
         j++;k++;
         if(t[j]==t[k])//当两个字符相同时，就跳过
            next[j] = next[k];
         else
            next[j] = k;
      }
      else k = next[k];
   }
}

