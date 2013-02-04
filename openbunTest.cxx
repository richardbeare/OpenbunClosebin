#include "ioutils.h"

#include "itkOpenBunImageFilter.h"

int main(int argc, char * argv[])
{
  // itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  const unsigned dim = 2;
  typedef itk::Image<unsigned char, dim> ImageType;
  ImageType::Pointer raw = readIm<ImageType>(argv[1]);

  typedef itk::FlatStructuringElement<dim> FlatKernelType;

  typedef itk::OpenBunImageFilter<ImageType, FlatKernelType> OpenBunType;

  FlatKernelType::RadiusType rad;
  rad.Fill(19);

  FlatKernelType kernel = FlatKernelType::Polygon(rad, 12);

  OpenBunType::Pointer cb = OpenBunType::New();
  cb->SetInput(raw);
  cb->SetKernel(kernel);

  writeIm<ImageType>(cb->GetOutput(), argv[2]);
  return EXIT_SUCCESS;
}
