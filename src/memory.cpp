#include "memory.h"

DataObject dataObjects[DATA_OBJECT_COUNT];
uint32_t dataObjectsCount;
RawData rawData[RAW_DATA_COUNT];
uint32_t rawDataCount;
uint32_t freeObjectsList[DATA_OBJECT_COUNT];
uint32_t freeRawData[RAW_DATA_COUNT];
CapabilityExecute exe[128][2];
uint32_t freeObjectsListLength;
uint32_t freeRawDataLength;
uint32_t exe_size[2];
uint32_t exe_actual;
