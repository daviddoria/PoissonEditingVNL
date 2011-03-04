#ifndef TYPES_H
#define TYPES_H

#include "itkImage.h"
#include "itkCovariantVector.h"

typedef itk::Image<unsigned char, 2> UnsignedCharScalarImageType;
typedef itk::Image<itk::CovariantVector<float, 3>, 2> FloatVectorImageType;
typedef itk::Image<itk::CovariantVector<unsigned char, 3>, 2> UnsignedCharVectorImageType;

#endif