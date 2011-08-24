#pragma once
#include "memory.h"
#include <string.h>
#include <assert.h>

inline DataPointerFlags data_from_pointer (DataPointer data, DataObjectFlag flags) {
  return data << 2 | flags;
}

inline DataPointer data_to_pointer (DataPointerFlags data) {
  return data >> 2;
}

inline DataPointerFlags create_data() {
  DataPointer result = newDataObject();
  memset (dataObjects+result, 0, sizeof (DataObject));
  return data_from_pointer (result, DATA_OBJECT_READWRITE);
}



inline DataPointerFlags data_get_data (DataPointerFlags d, size_t index) {
  DataPointer data = data_to_pointer(d);
  if (index <= 2)
    return dataObjects[data].data[index];
  else {
    RawDataPointer raw = dataObjects[data].ext_data_cap;
    if (raw && index < 18) {
      return rawData[raw].extensibleData.data[index-2];
    }
    else {
      return 0;
    }
  }
}

inline void data_set_data (DataPointerFlags d, size_t index, DataPointerFlags data2) {
  if (d & DATA_OBJECT_WRITE == 0) return;
  DataPointer data = data_to_pointer(d);
  if (index > 18) return;
  if (index <= 2)
    dataObjects[data].data[index] = data2;
  else {
    if (!dataObjects[data].ext_data_cap) {
      dataObjects[data].ext_data_cap = newRawData();
    }
    rawData[dataObjects[data].ext_data_cap].extensibleData.data[index-2] = data2;
  }
}

inline void data_set_data_r (DataPointerFlags data, size_t index, DataPointerFlags data2) {
  if (data & DATA_OBJECT_WRITE == 0) return;
  data_set_data (data_to_pointer(data), index, data2 & 0xFFFFFFFD);
}

inline Capability data_get_cap (DataPointer d, size_t index) {
  DataPointer data = data_to_pointer(d);
  if (index <= 2) {
    Capability result = dataObjects[data].cap[index];
    result.program &= 0x3FFFFFFF;
    return result;
  }
  else {
    RawDataPointer raw = dataObjects[data].ext_data_cap;
    if (raw && index < 10) {
      return rawData[raw].extensibleData.cap[index-2];
    }
    else {
      Capability result;
      result.data = 0;
      result.program = 0;
    }
  }
}

inline void data_set_cap (DataPointer d, size_t index, Capability cap) {
  if (d & DATA_OBJECT_WRITE == 0) return;
  DataPointer data = data_to_pointer(d);
  if (index > 10) return;
  if (index <= 2) {
    DataPointer pp = dataObjects[data].cap[index].program;
    cap.program = (cap.program & 0x3FFFFFFF) | (pp & 0xC00000000);
    dataObjects[data].cap[index] = cap;
  }
  else {
    if (!dataObjects[data].ext_data_cap) {
      dataObjects[data].ext_data_cap = newRawData();
    }
    rawData[dataObjects[data].ext_data_cap].extensibleData.cap[index-2] = cap;
  }
}

inline uint8_t data_get (DataPointerFlags d_, size_t index) {
  DataPointer d = data_to_pointer(d_);
  DataObject &data = dataObjects[d];
  if ((data.cap[0].program & 0xC0000000) == 0) {
    if (index < 32)
      return data.raw.bytes[index];
    else
      return 0;
  }
  else if ((data.cap[0].program & 0xC0000000) == 0x40000000) {
    if (index/sizeof(RawData) < 8)
      return rawData[data.raw.program[index/sizeof(RawData)]].byte[index%sizeof(RawData)];
    else
      return 0;
  }
  else if ((data.cap[0].program & 0xC0000000) == 0x80000000) {
    if (index >= sizeof(RawData)*sizeof(RawData))
      return 0;
    else {
      return rawData[rawData[data.raw.program[index/sizeof(RawData)*8/sizeof(RawData)]].pointers[index/sizeof(RawData)%(sizeof(RawData)*sizeof(RawData)/8)]].byte[index%sizeof(RawData)];
    }
  }
  else
    assert(0);
}

inline void data_set (DataPointerFlags d, size_t index, uint8_t b) {
  if (d & DATA_OBJECT_WRITE == 0) return;
  DataPointer data = data_to_pointer(d);
  if (index >= sizeof(RawData)*sizeof(RawData))
    return;
  uint32_t flags = dataObjects[data].cap[0].program & 0xC0000000;
  bool transform = (flags == 0 && index >= 32) || (flags = 0x40000000 && index >= 8*sizeof(RawData));
  if (transform) {
    if (index < 8*sizeof(RawData))
      flags = 0x40000000;
    else
      flags = 0x80000000;
  
    RawDataPointer raw = newRawData();
    memcpy(rawData[raw].byte, dataObjects[data].raw.bytes, 32);
    memset(rawData[raw].byte+32, 0, 128-32);
    for (int i = 1; i != 8; ++i)
      dataObjects[data].raw.program[i] = 0;
    dataObjects[data].raw.program[0] = raw;
    dataObjects[data].cap[0].program &= 0x3FFFFFFF;
    dataObjects[data].cap[0].program |= flags;
  }
  
  if (flags == 0)
    dataObjects[data].raw.bytes[index] = b;
  else if (flags == 0x40000000) {
    RawDataPointer rp = dataObjects[data].raw.program[index/sizeof(RawData)];
    if (!rp) {
      rp = dataObjects[data].raw.program[index/sizeof(RawData)] = newRawData();
      memset (rawData[rp].byte, 0, sizeof(RawData));
    }
    rawData[rp].byte[index%sizeof(RawData)] = b;
  }
  else if (flags == 0x80000000) {
    assert(0); //TODO: later!!
  }
}