#include <AmbSource.h>

static const unsigned short speedOfSound = 344;
static const unsigned short maxDistance = 150;

//=================================================================================================

AmbSource::AmbSource( unsigned long sampleRate )
: _sampleRate( sampleRate ),

  _delayFrac( 0.f ),
  _delayWhole( 0 ),
  _newDelayFrac( 0 ),
  _newDelayWhole( 0 ),
  _delayBufferSize( 0 ),
  _inIndex( 0 ),
  _newInIndex( 0 ),
  _outAIndex( 0 ),
  _outBIndex( 0 ),

  _lastDistance( 0.f ),
  _distanceChanged( false )
{
  SetSampleRate( _sampleRate );
}

//-------------------------------------------------------------------------------------------------

AmbSource::~AmbSource() {}

//=================================================================================================

bool AmbSource::SetSampleRate( unsigned long sampleRate )
{
  _sampleRate = sampleRate;

  _delayBufferSize = ( unsigned long ) ( ( float ) maxDistance / speedOfSound * _sampleRate + 0.5f );
  _delayBuffer.resize( _delayBufferSize );
  _delayBuffer.assign( _delayBufferSize, 0 );

  _delayFrac = position_.distance / speedOfSound * _sampleRate + 0.5f;
  _delayWhole = ( unsigned long ) _delayFrac;
  _delayFrac -= _delayWhole;

  _outAIndex = 0;
  _outBIndex = 1;
  _inIndex = ( _outBIndex + _delayWhole ) % _delayBufferSize;

  return true;
}

//-------------------------------------------------------------------------------------------------

void AmbSource::SetPosition( PolarCoord position )
{
  AmbComponent::SetPosition( position );

  if( !_distanceChanged && position_.distance != _lastDistance )
  {
    _distanceChanged = true;

    _lastDistance = position_.distance;

    _newDelayFrac = ( fabs( position_.distance ) / speedOfSound ) * _sampleRate;
    _newDelayWhole = ( unsigned long ) _newDelayFrac;
    _newDelayFrac -= _newDelayWhole;

    _newInIndex = ( _outBIndex + _newDelayWhole ) % _delayBufferSize;

    if( _newDelayWhole == _delayWhole )
    {
      _distanceChanged = false;
      _delayFrac = _newDelayFrac;
    }
  }
}

//-------------------------------------------------------------------------------------------------

void AmbSource::Process( std::vector< float >& src, AmbFormat& dest )
{
  unsigned short bufferSize = dest.GetBufferSize();

  if( src.size() != bufferSize )
  {
    src.resize( bufferSize );
  }

  // INPUT
  if( _distanceChanged )
  {
    // Moves away
    // ==========
    if( _newDelayWhole > _delayWhole )
    {
      unsigned long newBufferSize = bufferSize + ( _newDelayWhole - _delayWhole );
      std::vector< float > stretched;
      stretched.resize( newBufferSize );
      StretchBuffer( src, bufferSize, stretched, newBufferSize );

      for( unsigned long i = 0; i < newBufferSize; i++ )
      {
        _delayBuffer[_inIndex] = stretched[i];
        _inIndex = ( _inIndex + 1 ) % _delayBufferSize;
      }

      _distanceChanged = false;
      _delayFrac = _newDelayFrac;
      _delayWhole = _newDelayWhole;
      _inIndex = ( _newInIndex + bufferSize ) % _delayBufferSize;
    }

      // Moves closer
      // ============
    else if( _newDelayWhole < _delayWhole )
    {
      long newLength = bufferSize - ( _delayWhole - _newDelayWhole );

      // > Speed of sound
      // ================
      if( newLength < 0 )
      {
        // this is where the funky stuff happens
        _distanceChanged = false;
        _delayFrac = _newDelayFrac;
        _delayWhole = _newDelayWhole;
        _inIndex = ( _newInIndex + bufferSize ) % _delayBufferSize;
      }

        // <= Speed of sound
        // =================
      else if( newLength >= 0 )
      {
        if( newLength > 0 )
        {
          std::vector< float > stretched;
          stretched.resize( newLength );

          StretchBuffer( src, bufferSize, stretched, newLength );

          for( unsigned long i = 0; i < newLength; i++ )
          {
            _delayBuffer[_inIndex] = stretched[i];
            _inIndex = ( _inIndex + 1 ) % _delayBufferSize;
          }
        }

        _distanceChanged = false;
        _delayFrac = _newDelayFrac;
        _delayWhole = _newDelayWhole;
        _inIndex = ( _newInIndex + bufferSize ) % _delayBufferSize;
      }
    }
  }
  else
  {
    for( unsigned short i = 0; i < bufferSize; i++ )
    {
      _delayBuffer[_inIndex] = src[i];
      _inIndex = ( _inIndex + 1 ) % _delayBufferSize;
    }
  }

  // OUTPUT
  for( unsigned short i = 0; i < bufferSize; i++ )
  {
    float sample = ( _delayBuffer[_outAIndex] * ( 1.f - _delayFrac ) ) +
                   ( _delayBuffer[_outBIndex] * _delayFrac );

    //Source is outside speaker array
    if( fabs( position_.distance ) > _speakerRadius )
    {
      sample *= _speakerRadius / fabs( position_.distance );
    }

    for( unsigned short j = 0; j < nCoeff_; j++ )
    {
      dest.data[j][i] += sample * coeffValues_[j];
    }
    dest.data[16][i] += sample * coeffValues_[16];

    _outAIndex = ( _outAIndex + 1 ) % _delayBufferSize;
    _outBIndex = ( _outBIndex + 1 ) % _delayBufferSize;
  }
}

//=================================================================================================

void AmbSource::StretchBuffer( std::vector< float >& actBuffer, unsigned long actLength, std::vector< float >& strBuffer, unsigned long strLength )
{
  if( actLength != strLength )
  {
    for( unsigned long i = 0; i < strLength; i++ )
    {
      float frac = ( i / ( float ) ( strLength - 1 ) ) * ( actLength - 1 );
      unsigned long whole = ( unsigned long ) frac;
      frac -= whole;

      if( whole != actLength - 1 )
      {
        strBuffer[i] = ( ( 1.0f - frac ) * actBuffer[ whole ] ) + ( frac * actBuffer[ whole + 1 ] );
      }
      else
      {
        strBuffer[i] = actBuffer[actLength - 1];
      }
    }
  }
  else if( actLength == strLength )
  {
    actBuffer = strBuffer;
  }
}

//=================================================================================================
