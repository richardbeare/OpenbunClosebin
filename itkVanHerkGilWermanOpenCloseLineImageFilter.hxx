/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkVanHerkGilWermanOpenCloseLineImageFilter_hxx
#define __itkVanHerkGilWermanOpenCloseLineImageFilter_hxx

#include "itkVanHerkGilWermanOpenCloseLineImageFilter.h"
#include "itkImageRegionIterator.h"

#include "itkVanHerkGilWermanUtilitiesMod.h"

namespace itk
{
template< class TImage, class TKernel, class TFunction1, class TFunction2 >
VanHerkGilWermanOpenCloseLineImageFilter< TImage, TKernel, TFunction1, TFunction2 >
::VanHerkGilWermanOpenCloseLineImageFilter()
{
}

template< class TImage, class TKernel, class TFunction1, class TFunction2 >
void
VanHerkGilWermanOpenCloseLineImageFilter< TImage, TKernel, TFunction1, TFunction2 >
::ThreadedGenerateData(const InputImageRegionType & outputRegionForThread,
                       ThreadIdType threadId)
{
  // check that we are using a decomposable kernel
  if ( !this->GetKernel().GetDecomposable() )
    {
    itkExceptionMacro("VanHerkGilWerman morphology only works with decomposable structuring elements");
    return;
    }

// TFunction1 will be < for erosions

  // the initial version will adopt the methodology of loading a line
  // at a time into a buffer vector, carrying out the opening or
  // closing, and then copy the result to the output. Hopefully this
  // will improve cache performance when working along non raster
  // directions.

  ProgressReporter progress(this, threadId, this->GetKernel().GetLines().size() + 1);

  InputImageConstPointer input = this->GetInput();

  InputImageRegionType IReg = outputRegionForThread;
  IReg.PadByRadius( this->GetKernel().GetRadius() );
  // IReg.PadByRadius( this->GetKernel().GetRadius() );
  IReg.Crop( this->GetInput()->GetRequestedRegion() );

  // allocate an internal buffer
  typename InputImageType::Pointer internalbuffer = InputImageType::New();
  internalbuffer->SetRegions(IReg);
  internalbuffer->Allocate();
  InputImagePixelType initval;
  TFunction1 MMFunc;

  initval = MMFunc(itk::NumericTraits<InputImagePixelType>::max(),
                   itk::NumericTraits<InputImagePixelType>::NonpositiveMin());
  internalbuffer->FillBuffer(initval);
  InputImagePointer output = internalbuffer;

  // get the region size
  InputImageRegionType OReg = outputRegionForThread;
  // maximum buffer length is sum of dimensions
  unsigned int bufflength = 0;
  for ( unsigned i = 0; i < TImage::ImageDimension; i++ )
    {
    bufflength += IReg.GetSize()[i];
    }

  // compat
  bufflength += 2;

  std::vector<InputImagePixelType> buffer(bufflength);
  std::vector<InputImagePixelType> forward(bufflength);
  std::vector<InputImagePixelType> reverse(bufflength);
  // iterate over all the structuring elements
  typename KernelType::DecompType decomposition = this->GetKernel().GetLines();
  BresType BresLine;

  typedef typename KernelType::LType KernelLType;
  for ( unsigned i = 0; i < decomposition.size(); i++ )
    {
    typename KernelType::LType ThisLine = decomposition[i];
    // fix up the line to use in a non-decomposition form
    unsigned int SELength = this->GetKernel().GetRadius()[0];
    ThisLine = ThisLine * ((float)SELength/ThisLine.GetNorm());
    typename BresType::OffsetArray TheseOffsets = BresLine.BuildLine(ThisLine, bufflength);
    // not building a circle, like we do with the traditional
    //application of FlatStructuringElement. Instead we get the radius
    //from the first element of the neighbourhood radius vector, as we
    //are assuming a circular/spherical symmetry

    // how should we take voxel dimensions into account?
    //if (this->UseImageDimensions())
    //{
      // this bit takes care of line angles, but assumes istropic voxels.
    SELength = GetLinePixels<KernelLType>(ThisLine);
      //  }
    // want lines to be odd
    if ( !( SELength % 2 ) )
      {
      ++SELength;
      }
    InputImageRegionType BigFace = MakeEnlargedFace< InputImageType, KernelLType >(input, IReg, ThisLine);

    DoFace2Op< TImage, BresType, TFunction1, TFunction2, KernelLType >
      (input, output,
       m_Boundary, ThisLine,
       TheseOffsets, SELength,
       buffer, forward,
       reverse, IReg, BigFace);

    progress.CompletedPixel();
    }

  // copy internal buffer to output
  typedef ImageRegionIterator< InputImageType > IterType;
  IterType oit(this->GetOutput(), OReg);
  IterType iit(internalbuffer, OReg);
  for ( oit.GoToBegin(), iit.GoToBegin(); !oit.IsAtEnd(); ++oit, ++iit )
    {
    oit.Set( iit.Get() );
    }
  progress.CompletedPixel();
}

template< class TImage, class TKernel, class TFunction1, class TFunction2 >
void
VanHerkGilWermanOpenCloseLineImageFilter< TImage, TKernel, TFunction1, TFunction2 >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Boundary: " << m_Boundary << std::endl;
}

} // end namespace itk

#endif