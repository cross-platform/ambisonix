#include "AmbComponent.h"

static const float fSqrt3_2 = sqrtf( 3.f ) / 2.f;
static const float fSqrt_3_8 = sqrtf( 3.f / 8.f );
static const float fSqrt15_2 = sqrtf( 15.f ) / 2.f;
static const float fSqrt_5_8 = sqrtf( 5.f / 8.f );

//=================================================================================================

AmbComponent::AmbComponent()
: _speakerRadius( 1.f )
{
  position_.azimuth = 0.f;
  position_.elevation = 0.f;
  position_.distance = 0.f;

  coeffWeights_[0] = 1.f / sqrtf( 2.f );

  coeffWeights_[1] = 1.f;
  coeffWeights_[2] = 1.f;
  coeffWeights_[3] = 1.f;

  coeffWeights_[4] = 1.f;
  coeffWeights_[5] = 2.f / sqrtf( 3.f );
  coeffWeights_[6] = 2.f / sqrtf( 3.f );
  coeffWeights_[7] = 2.f / sqrtf( 3.f );
  coeffWeights_[8] = 2.f / sqrtf( 3.f );

  coeffWeights_[9] = 1.f;
  coeffWeights_[10] = sqrtf( 45.f / 32.f );
  coeffWeights_[11] = sqrtf( 45.f / 32.f );
  coeffWeights_[12] = 3.f / sqrtf( 5.f );
  coeffWeights_[13] = 3.f / sqrtf( 5.f );
  coeffWeights_[14] = sqrtf( 8.f / 5.f );
  coeffWeights_[15] = sqrtf( 8.f / 5.f );

  coeffWeights_[16] = 1.f;

  SetOrder( 3 );
  SetPosition( position_ );
}

//-------------------------------------------------------------------------------------------------

AmbComponent::~AmbComponent() {}

//=================================================================================================

void AmbComponent::SetPosition( PolarCoord position )
{
  position_ = position;

  float hypSphDistance = 0.0;

  if( _speakerRadius != 0 )
  {
    if( position_.distance < _speakerRadius )
    {
      hypSphDistance = ( position_.distance * 3.14159265358979323846f ) / ( 2.0f * _speakerRadius );
    }
    else
    {
      hypSphDistance = 3.14159265358979323846f / 2.0f;
    }
  }
  else
  {
    hypSphDistance = 0;
  }

  float fSinDist = sinf( hypSphDistance );
  float fCosDist = cosf( hypSphDistance );

  float fCosAzim = cosf( position_.azimuth );
  float fSinAzim = sinf( position_.azimuth );
  float fCosElev = cosf( position_.elevation );
  float fSinElev = sinf( position_.elevation );

  float fCos2Azim = cosf( 2.f * position_.azimuth );
  float fSin2Azim = sinf( 2.f * position_.azimuth );
  float fSin2Elev = sinf( 2.f * position_.elevation );

  float fCos3Azim = cosf( 3.f * position_.azimuth );
  float fSin3Azim = sinf( 3.f * position_.azimuth );

  float fCosElev2 = powf( fCosElev, 2.f );
  float fCosElev3 = powf( fCosElev, 3.f );
  float fSinElev2 = powf( fSinElev, 2.f );

  // clear coefficients
  memset( coeffValues_, 0, 17 * sizeof( float ) );

  // Zero Order
  coeffValues_[0] = coeffWeights_[0];

  // First Order
  if( order_ >= 1 )
  {
    coeffValues_[1] = ( fCosAzim * fCosElev * fSinDist ) * coeffWeights_[1];
    coeffValues_[2] = ( fSinAzim * fCosElev * fSinDist ) * coeffWeights_[2];
    coeffValues_[3] = ( fSinElev * fSinDist ) * coeffWeights_[3];
  }

  // Second Order
  if( order_ >= 2 )
  {
    coeffValues_[4] = ( 0.5f * ( 3.f * fSinElev2 - 1.f ) * fSinDist ) * coeffWeights_[4];
    coeffValues_[5] = ( fSqrt3_2 * fCosAzim * fSin2Elev * fSinDist ) * coeffWeights_[5];
    coeffValues_[6] = ( fSqrt3_2 * fSinAzim * fSin2Elev * fSinDist ) * coeffWeights_[6];
    coeffValues_[7] = ( fSqrt3_2 * fCos2Azim * fCosElev2 * fSinDist ) * coeffWeights_[7];
    coeffValues_[8] = ( fSqrt3_2 * fSin2Azim * fCosElev2 * fSinDist ) * coeffWeights_[8];
  }

  // Third Order
  if( order_ >= 3 )
  {
    coeffValues_[9] = ( 0.5f * fSinElev * ( 5.f * fSinElev2 - 3.f ) * fSinDist ) * coeffWeights_[9];
    coeffValues_[10] = ( fSqrt_3_8 * fCosAzim * fCosElev * ( 5.f * fSinElev2 - 1.f ) * fSinDist ) * coeffWeights_[10];
    coeffValues_[11] = ( fSqrt_3_8 * fSinAzim * fCosElev * ( 5.f * fSinElev2 - 1.f ) * fSinDist ) * coeffWeights_[11];
    coeffValues_[12] = ( fSqrt15_2 * fCos2Azim * fSinElev * fCosElev2 * fSinDist ) * coeffWeights_[12];
    coeffValues_[13] = ( fSqrt15_2 * fSin2Azim * fSinElev * fCosElev2 * fSinDist ) * coeffWeights_[13];
    coeffValues_[14] = ( fSqrt_5_8 * fCos3Azim * fCosElev3 * fSinDist ) * coeffWeights_[14];
    coeffValues_[15] = ( fSqrt_5_8 * fSin3Azim * fCosElev3 * fSinDist ) * coeffWeights_[15];
  }

  // Distance Encoding
  coeffValues_[16] = fCosDist * coeffWeights_[16];
}

//-------------------------------------------------------------------------------------------------

PolarCoord AmbComponent::GetPosition() const
{
  return position_;
}

//-------------------------------------------------------------------------------------------------

void AmbComponent::SetSpeakerRadius( float speakerRadius )
{
  _speakerRadius = speakerRadius;
  SetPosition( position_ );
}

//-------------------------------------------------------------------------------------------------

float AmbComponent::GetSpeakerRadius() const
{
  return _speakerRadius;
}

//-------------------------------------------------------------------------------------------------

bool AmbComponent::SetOrder( unsigned char order )
{
  switch( order )
  {
    case 0:
      nCoeff_ = 1;
      break;

    case 1:
      nCoeff_ = 4;
      break;

    case 2:
      nCoeff_ = 9;
      break;

    case 3:
      nCoeff_ = 16;
      break;

    default:
      return false;
  }

  order_ = order;

  SetPosition( position_ );
  return true;
}

//=================================================================================================
