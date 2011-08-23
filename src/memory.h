#pragma once
#include "config.h"
#include <stdint.h>

typedef uint32_t DataPointer;
typedef uint32_t RawDataPointer;

typedef uint32_t DataPointerFlags;

struct Capability {
  DataPointer data;
  DataPointer program;
} __attribute__((__packed__));;

enum DataObjectFlag {
  DATA_OBJECT_EXECUTE = 0,
  DATA_OBJECT_READ = 1,
  DATA_OBJECT_WRITE = 2,
  DATA_OBJECT_READWRITE = 3
};

struct DataObject {
  DataPointerFlags data[3];
  RawDataPointer ext_data_cap;
  Capability cap[2]; // here can be saved a few flags
  union {
    uint8_t bytes[32];
    RawDataPointer program[8];
  } raw;
} __attribute__((__packed__));;

struct ExtensibleDataCap {
  DataPointerFlags data[16];
  Capability cap[8];
} __attribute__((__packed__));;

union RawData {
  uint32_t pointers[32];
  ExtensibleDataCap extensibleData;
  uint8_t byte[128];
} __attribute__((__packed__));;

struct CapabilityExecute {
  Capability cap;
  DataPointerFlags data;
} __attribute__((__packed__));;

extern DataObject dataObjects[DATA_OBJECT_COUNT];
extern uint32_t dataObjectsCount;
extern RawData rawData[RAW_DATA_COUNT];
extern uint32_t rawDataCount;
extern uint32_t freeObjectsList[DATA_OBJECT_COUNT];
extern uint32_t freeRawData[RAW_DATA_COUNT];
extern uint32_t freeObjectsListLength;
extern uint32_t freeRawDataLength;
extern CapabilityExecute exe[128][2];
extern uint32_t exe_size[2];
extern uint32_t exe_actual;

inline DataPointer newDataObject () {
  if (freeObjectsListLength) {
    return freeObjectsList[--freeObjectsListLength];
  }
  else {
    return dataObjectsCount++;
  }
}

inline void removeDataObject (DataPointer data) {
  freeObjectsList[freeObjectsListLength++] = data;
}

inline RawDataPointer newRawData () {
  if (freeRawDataLength) {
    return freeRawData[--freeRawDataLength];
  }
  else {
    return freeRawDataLength++;
  }
}

inline void removeRawData (RawDataPointer data) {
  freeRawData[freeRawDataLength++] = data;
}