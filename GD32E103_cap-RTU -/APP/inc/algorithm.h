#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "gd32e10x.h"

void ArrayMerge(uint8_t A1[], uint8_t A2[], uint8_t A[], uint8_t Size1, uint8_t Size2);
void SortArrayExtreme(uint16_t Array[], const uint32_t ArraySize,
                      const uint32_t SortHeadSize, const uint32_t SortTailSize);
uint32_t GetDelExtremeAndAverage(uint16_t Array[], const uint32_t ArraySize,
                            const uint32_t SortHeadSize, const uint32_t SortTailSize);

uint32_t ArrToHex(uint8_t *pData);
float HexToFloat(uint8_t *pData);
void floatToHexArray(float fSource, uint8_t *puint8_tTemp, long num);
uint32_t HexToUlong(uint8_t *pData);



#endif
