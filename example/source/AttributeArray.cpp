//
//  AttributeArray.cpp
//  ExampleVR
//
//  Created by Seth Johnson on 7/29/15.
//
//

#include "AttributeArray.h"
AttributeArray::AttributeArray(void * data, size_t elementSize, size_t elementCount, size_t elementDimension, GLenum elementType) : RawData(data, elementSize, elementCount, elementDimension, elementType) {}

