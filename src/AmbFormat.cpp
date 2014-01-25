#include <ambisonix/AmbFormat.h>

//=================================================================================================

AmbFormat::AmbFormat( unsigned short bufferSize )
: _bufferSize( bufferSize )
{
  SetBufferSize( bufferSize );
}

//-------------------------------------------------------------------------------------------------

AmbFormat::~AmbFormat() {}

//=================================================================================================

bool AmbFormat::SetBufferSize( unsigned short bufferSize )
{
  _bufferSize = bufferSize;

  for( unsigned short i = 0; i < 17; i++ )
  {
    data[i].resize( bufferSize );
  }

  return true;
}

//-------------------------------------------------------------------------------------------------

unsigned short AmbFormat::GetBufferSize() const
{
  return _bufferSize;
}

//-------------------------------------------------------------------------------------------------

void AmbFormat::Clear()
{
  for( unsigned short i = 0; i < 17; i++ )
  {
    for( unsigned short j = 0; j < _bufferSize; j++ )
    {
      data[i][j] = 0;
    }
  }
}

//-------------------------------------------------------------------------------------------------

AmbFormat& AmbFormat::operator+( const AmbFormat& rhs )
{
  for( unsigned short i = 0; i < 17; i++ )
  {
    for( unsigned short j = 0; j < _bufferSize; j++ )
    {
      data[i][j] += rhs.data[i][j];
    }
  }

  return *this;
}

//-------------------------------------------------------------------------------------------------

AmbFormat& AmbFormat::operator+=( const AmbFormat& rhs )
{
  return *this + rhs;
}

//=================================================================================================
