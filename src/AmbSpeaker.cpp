#include <AmbSpeaker.h>

//=================================================================================================

AmbSpeaker::AmbSpeaker() {}

//-------------------------------------------------------------------------------------------------

AmbSpeaker::~AmbSpeaker() {}

//=================================================================================================

void AmbSpeaker::Process( AmbFormat& src, std::vector< float >& dest ) const
{
  unsigned short bufferSize = src.GetBufferSize();

  if( dest.size() != bufferSize )
  {
    dest.resize( bufferSize );
  }

  dest.assign( dest.size(), 0 );

  for( unsigned short i = 0; i < bufferSize; i++ )
  {
    for( unsigned short j = 0; j < nCoeff_; j++ )
    {
      dest[i] += src.data[j][i] * coeffValues_[j];
    }
    dest[i] += src.data[16][i] * coeffValues_[16];
  }
}

//=================================================================================================
