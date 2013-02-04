#include "ioutils.h"

#include "itkCloseBinImageFilter.h"

int main(int argc, char * argv[])
{
  const unsigned dim = 2;
  typedef itk::Image<unsigned char, dim> ImageType;
  ImageType::Pointer raw = readIm<ImageType>(argv[1]);

  typedef itk::FlatStructuringElement<dim> FlatKernelType;

  typedef itk::CloseBinImageFilter<ImageType, FlatKernelType> CloseBinType;

  FlatKernelType::RadiusType rad;
  rad.Fill(7);

  FlatKernelType kernel = FlatKernelType::Polygon(rad, 6);

  CloseBinType::Pointer cb = CloseBinType::New();
  cb->SetInput(raw);
  cb->SetKernel(kernel);

  writeIm<ImageType>(cb->GetOutput(), argv[2]);
  return EXIT_SUCCESS;
}
