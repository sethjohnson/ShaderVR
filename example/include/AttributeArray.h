//
//  AttributeArray.h
//  ExampleVR
//
//  Created by Seth Johnson on 7/29/15.
//
//

#ifndef ATTRIBUTEARRAY_H
#define ATTRIBUTEARRAY_H

#include <stdio.h>
#include "GL/glew.h"
#include "RawData.h"

class AttributeArray : public RawData {

    
public:
    AttributeArray(void * data, size_t elementSize, size_t elementCount, size_t elementDimension, GLenum elementType);
    ~AttributeArray();

};


#endif