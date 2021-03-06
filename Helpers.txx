#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"

namespace Helpers
{


template<typename TImage>
void DeepCopy(typename TImage::Pointer input, typename TImage::Pointer output)
{
  output->SetRegions(input->GetLargestPossibleRegion());
  output->Allocate();

  itk::ImageRegionConstIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<TImage> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }
}

template<typename TImage>
void ExtractComponent(typename TImage::Pointer input, unsigned int component, FloatScalarImageType::Pointer output)
//void ExtractComponent(typename TImage::Pointer input, unsigned int component, typename itk::Image<typename TImage::PixelType, 2>::Pointer output)
{
  if(component >= TImage::PixelType::Dimension)
    {
    std::cerr << "Component is " << component << " but must be less than " << TImage::PixelType::Dimension << std::endl;
    exit(-1);
    }

  output->SetRegions(input->GetLargestPossibleRegion());
  output->Allocate();

  itk::ImageRegionConstIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  //itk::ImageRegionIterator<itk::Image<typename TImage::PixelType, 2> > outputIterator(output, output->GetLargestPossibleRegion());
  itk::ImageRegionIterator<FloatScalarImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get()[component]);
    ++inputIterator;
    ++outputIterator;
    }
}


template<typename TImage>
void SetComponent(typename TImage::Pointer input, unsigned int component, FloatScalarImageType::Pointer componentImage)
{
  if(component >= TImage::PixelType::Dimension)
    {
    std::cerr << "Component is " << component << " but must be less than " << TImage::PixelType::Dimension << std::endl;
    exit(-1);
    }

  itk::ImageRegionIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionConstIterator<FloatScalarImageType> componentIterator(componentImage, componentImage->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    typename TImage::PixelType pixel = inputIterator.Get();
    pixel[component] = componentIterator.Get();
    inputIterator.Set(pixel);
    ++inputIterator;
    ++componentIterator;
    }
}


template<typename TImage>
void CastAndWriteImage(typename TImage::Pointer input, std::string filename)
{
  if(TImage::PixelType::Dimension != 3)
    {
    std::cerr << "WriteImage: input doesn't have 3 components (it has " << TImage::PixelType::Dimension << ")" << std::endl;
    return;
    }

  typedef itk::CastImageFilter< TImage, UnsignedCharVectorImageType > CastFilterType;
  typename CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput(input);
  castFilter->Update();

  typedef  itk::ImageFileWriter< UnsignedCharVectorImageType > PNGWriterType;
  PNGWriterType::Pointer pngWriter = PNGWriterType::New();
  pngWriter->SetFileName(filename);
  pngWriter->SetInput(castFilter->GetOutput());
  pngWriter->Update();
}


template<typename TImage>
void CastAndWriteScalarImage(typename TImage::Pointer input, std::string filename)
{
  typedef itk::CastImageFilter< TImage, UnsignedCharScalarImageType > CastFilterType;
  typename CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput(input);
  castFilter->Update();

  typedef  itk::ImageFileWriter< UnsignedCharScalarImageType > PNGWriterType;
  PNGWriterType::Pointer pngWriter = PNGWriterType::New();
  pngWriter->SetFileName(filename);
  pngWriter->SetInput(castFilter->GetOutput());
  pngWriter->Update();
}


template<typename TImage>
void ScaleAndWriteScalarImage(typename TImage::Pointer input, std::string filename)
{
  typedef itk::RescaleIntensityImageFilter< TImage, UnsignedCharScalarImageType > RescaleFilterType;
  typename RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(input);
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();

  typedef  itk::ImageFileWriter< UnsignedCharScalarImageType > PNGWriterType;
  PNGWriterType::Pointer pngWriter = PNGWriterType::New();
  pngWriter->SetFileName(filename);
  pngWriter->SetInput(rescaleFilter->GetOutput());
  pngWriter->Update();
}


template<typename TImage>
void WriteImage(typename TImage::Pointer input, std::string filename)
{
  typedef  itk::ImageFileWriter< TImage > MHDWriterType;
  typename MHDWriterType::Pointer mhdWriter = MHDWriterType::New();
  mhdWriter->SetFileName(filename);
  mhdWriter->SetInput(input);
  mhdWriter->Update();
}

template<typename TImage>
void ClampImage(typename TImage::Pointer image)
{
  itk::ImageRegionIterator<TImage> imageIterator(image, image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    typename TImage::PixelType pixel = imageIterator.Get();

    for(unsigned int i = 0; i < TImage::PixelType::Dimension; i++)
      {
      if(pixel[i] < 0)
        {
        pixel[i] = 0;
        }
      if(pixel[i] > 255)
        {
        pixel[i] = 255;
        }
      }// end for components

    imageIterator.Set(pixel);
    ++imageIterator;
    }// end iterator while
}


}// end namespace