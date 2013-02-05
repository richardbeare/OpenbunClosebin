#include "ioutils.h"

#include "itkCloseBinImageFilter.h"

int main(int argc, char * argv[])
{
  const unsigned dim = 2;
  typedef itk::Image<unsigned char, dim> ImageType;
  ImageType::Pointer raw = readIm<ImageType>(argv[3]);

  int r = atoi(argv[1]);
  int count = atoi(argv[2]);

  typedef itk::FlatStructuringElement<dim> FlatKernelType;

  typedef itk::CloseBinImageFilter<ImageType, FlatKernelType> CloseBinType;

  FlatKernelType::RadiusType rad;
  rad.Fill(r);

  FlatKernelType kernel = FlatKernelType::Polygon(rad, count);

  CloseBinType::Pointer cb = CloseBinType::New();
  cb->SetInput(raw);
  cb->SetKernel(kernel);

  writeIm<ImageType>(cb->GetOutput(), argv[4]);
  return EXIT_SUCCESS;
}
