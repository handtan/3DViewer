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
#ifndef itkWeightedCentroidKdTreeGenerator_hxx
#define itkWeightedCentroidKdTreeGenerator_hxx

#include "itkWeightedCentroidKdTreeGenerator.h"

namespace itk
{
namespace Statistics
{

template< typename TSample >
void
WeightedCentroidKdTreeGenerator< TSample >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template< typename TSample >
inline typename WeightedCentroidKdTreeGenerator< TSample >::KdTreeNodeType *
WeightedCentroidKdTreeGenerator< TSample >
::GenerateNonterminalNode(unsigned int beginIndex,
                          unsigned int endIndex,
                          MeasurementVectorType & lowerBound,
                          MeasurementVectorType & upperBound,
                          unsigned int level)
{
  MeasurementType dimensionLowerBound;
  MeasurementType dimensionUpperBound;
  MeasurementType partitionValue;
  unsigned int    partitionDimension = 0;
  unsigned int    i;
  unsigned int    j;
  MeasurementType spread;
  MeasurementType maxSpread;
  unsigned int    medianIndex;

  SubsamplePointer subsample = this->GetSubsample();

  // Sanity check. Verify that the subsample has measurement vectors of the
  // same length as the sample generated by the tree.
  if ( this->GetMeasurementVectorSize() != subsample->GetMeasurementVectorSize() )
    {
    itkExceptionMacro(<< "Measurement Vector Length mismatch");
    }

  // calculates the weighted centroid which is the vector sum
  // of all the associated instances.
  typename KdTreeNodeType::CentroidType weightedCentroid;
  NumericTraits<typename KdTreeNodeType::CentroidType>::SetLength( weightedCentroid,
    this->GetMeasurementVectorSize() );
  MeasurementVectorType tempVector;
  weightedCentroid.Fill(NumericTraits< MeasurementType >::ZeroValue());

  for ( i = beginIndex; i < endIndex; i++ )
    {
    tempVector = subsample->GetMeasurementVectorByIndex(i);
    for ( j = 0; j < this->GetMeasurementVectorSize(); j++ )
      {
      weightedCentroid[j] += tempVector[j];
      }
    }

  // find most widely spread dimension
  Algorithm::FindSampleBoundAndMean< SubsampleType >(this->GetSubsample(),
                                                     beginIndex, endIndex,
                                                     m_TempLowerBound, m_TempUpperBound,
                                                     m_TempMean);

  maxSpread = NumericTraits< MeasurementType >::NonpositiveMin();
  for ( i = 0; i < this->GetMeasurementVectorSize(); i++ )
    {
    spread = m_TempUpperBound[i] - m_TempLowerBound[i];
    if ( spread >= maxSpread )
      {
      maxSpread = spread;
      partitionDimension = i;
      }
    }

  medianIndex = ( endIndex - beginIndex ) / 2;

  //
  // Find the medial element by using the NthElement function
  // based on the STL implementation of the QuickSelect algorithm.
  //
  partitionValue =
    Algorithm::NthElement< SubsampleType >(this->GetSubsample(),
                                           partitionDimension,
                                           beginIndex, endIndex,
                                           medianIndex);

  medianIndex += beginIndex;

  // save bounds for cutting dimension
  dimensionLowerBound = lowerBound[partitionDimension];
  dimensionUpperBound = upperBound[partitionDimension];

  upperBound[partitionDimension] = partitionValue;
  const unsigned int beginLeftIndex = beginIndex;
  const unsigned int endLeftIndex   = medianIndex;
  KdTreeNodeType *   left = this->GenerateTreeLoop(beginLeftIndex, endLeftIndex, lowerBound, upperBound, level + 1);
  upperBound[partitionDimension] = dimensionUpperBound;

  lowerBound[partitionDimension] = partitionValue;
  const unsigned int beginRightIndex = medianIndex + 1;
  const unsigned int endRighIndex    = endIndex;
  KdTreeNodeType *   right = this->GenerateTreeLoop(beginRightIndex, endRighIndex, lowerBound, upperBound, level + 1);
  lowerBound[partitionDimension] = dimensionLowerBound;

  using KdTreeNonterminalNodeType = KdTreeWeightedCentroidNonterminalNode< TSample >;

  auto * nonTerminalNode = new KdTreeNonterminalNodeType(partitionDimension,
                                  partitionValue,
                                  left, right,
                                  weightedCentroid,
                                  endIndex - beginIndex);

  nonTerminalNode->AddInstanceIdentifier(
    subsample->GetInstanceIdentifier(medianIndex) );

  return nonTerminalNode;
}
} // end of namespace Statistics
} // end of namespace itk

#endif
