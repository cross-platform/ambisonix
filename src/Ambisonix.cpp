#include "../include/Ambisonix.h"

//=================================================================================================

Ambisonix::Ambisonix( unsigned short bufferSize, unsigned long sampleRate )
: _bufferSize( bufferSize ),
  _sampleRate( sampleRate ),
  _speakerRadius( 0 ),
  _speakerOutputProcessed( false ),
  _order( 3 ) {}

//-------------------------------------------------------------------------------------------------

Ambisonix::~Ambisonix() {}

//=================================================================================================

void Ambisonix::SetBufferSize( unsigned short bufferSize )
{
  _bufferSize = bufferSize;

  _bFormat.SetBufferSize( _bufferSize );
}

//-------------------------------------------------------------------------------------------------

void Ambisonix::SetSampleRate( unsigned long sampleRate )
{
  _sampleRate = sampleRate;

  for( unsigned short i = 0; i < _sources.size(); i++ )
  {
    _sources[i].SetSampleRate( sampleRate );
  }
}

//-------------------------------------------------------------------------------------------------

unsigned short Ambisonix::GetBufferSize() const
{
  return _bufferSize;
}

//-------------------------------------------------------------------------------------------------

unsigned long Ambisonix::GetSampleRate() const
{
  return _sampleRate;
}

//-------------------------------------------------------------------------------------------------

void Ambisonix::AddSourceCart( float x, float y, float z )
{
  _CartToSph( x, y, z );
  AddSourceSph( x, y, z );
}

//-------------------------------------------------------------------------------------------------

void Ambisonix::AddSourceSph( float azimuth, float elevation, float distance )
{
  AmbSource newSource( _sampleRate );

  _ClampSph( azimuth, elevation );

  newSource.SetSpeakerRadius( _speakerRadius );
  newSource.SetOrder( _order );
  _sources.push_back( newSource );

  MoveSourceSph( _sources.size() - 1, azimuth, elevation, distance );
}

//-------------------------------------------------------------------------------------------------

bool Ambisonix::MoveSourceCart( unsigned short sourceIndex, float x, float y, float z )
{
  _CartToSph( x, y, z );
  return MoveSourceSph( sourceIndex, x, y, z );
}

//-------------------------------------------------------------------------------------------------

bool Ambisonix::MoveSourceSph( unsigned short sourceIndex, float azimuth, float elevation, float distance )
{
  if( sourceIndex >= _sources.size() )
  {
    return false;
  }

  _ClampSph( azimuth, elevation );

  PolarCoord inputPosition;

  inputPosition.azimuth = DegToRad( azimuth );
  inputPosition.elevation = DegToRad( elevation );
  inputPosition.distance = distance;

  _sources[sourceIndex].SetPosition( inputPosition );

  return true;
}

//-------------------------------------------------------------------------------------------------

void Ambisonix::ClearSources()
{
  _sources.clear();
}

//-------------------------------------------------------------------------------------------------

unsigned short Ambisonix::GetSourceCount() const
{
  return _sources.size();
}

//-------------------------------------------------------------------------------------------------

void Ambisonix::AddSpeakerCart( float x, float y, float z )
{
  _CartToSph( x, y, z );
  AddSpeakerSph( x, y, z );
}

//-------------------------------------------------------------------------------------------------

void Ambisonix::AddSpeakerSph( float azimuth, float elevation, float distance )
{
  AmbSpeaker newSpeaker;

  _ClampSph( azimuth, elevation );

  newSpeaker.SetSpeakerRadius( _speakerRadius );
  newSpeaker.SetOrder( _order );
  _speakers.push_back( newSpeaker );

  MoveSpeakerSph( _speakers.size() - 1, azimuth, elevation, distance );
}

//-------------------------------------------------------------------------------------------------

bool Ambisonix::MoveSpeakerCart( unsigned short speakerIndex, float x, float y, float z )
{
  _CartToSph( x, y, z );
  return MoveSpeakerSph( speakerIndex, x, y, z );
}

//-------------------------------------------------------------------------------------------------

bool Ambisonix::MoveSpeakerSph( unsigned short speakerIndex, float azimuth, float elevation, float distance )
{
  if( speakerIndex >= _speakers.size() )
  {
    return false;
  }

  _ClampSph( azimuth, elevation );

  PolarCoord speakerPosition;

  speakerPosition.azimuth = DegToRad( azimuth );
  speakerPosition.elevation = DegToRad( elevation );
  speakerPosition.distance = distance;

  _speakers[speakerIndex].SetPosition( speakerPosition );

  // update speaker radius
  _speakerRadius = 0;
  for( unsigned short i = 0; i < _speakers.size(); i++ )
  {
    if( _speakers[i].GetPosition().distance > _speakerRadius )
    {
      _speakerRadius = _speakers[i].GetPosition().distance;
    }
  }

  for( unsigned short i = 0; i < _sources.size(); i++ )
  {
    _sources[i].SetSpeakerRadius( _speakerRadius );
  }

  for( unsigned short i = 0; i < _speakers.size(); i++ )
  {
    _speakers[i].SetSpeakerRadius( _speakerRadius );
  }

  return true;
}

//-------------------------------------------------------------------------------------------------

void Ambisonix::ClearSpeakers()
{
  _speakers.clear();
  _speakerRadius = 0;
}

//-------------------------------------------------------------------------------------------------

unsigned short Ambisonix::GetSpeakerCount() const
{
  return _speakers.size();
}

//-------------------------------------------------------------------------------------------------

bool Ambisonix::SetSourceInput( unsigned short sourceIndex, std::vector< float >& sourceInput )
{
  if( sourceIndex >= _sources.size() )
  {
    return false;
  }

  if( _speakerOutputProcessed )
  {
    _bFormat.Clear();
    _speakerOutputProcessed = false;
  }

  _sources[sourceIndex].Process( sourceInput, _bFormat );

  return true;
}

//-------------------------------------------------------------------------------------------------

bool Ambisonix::GetSpeakerOutput( unsigned short speakerIndex, std::vector< float >& speakerOutput )
{
  if( speakerIndex >= _speakers.size() )
  {
    return false;
  }

  _speakers[speakerIndex].Process( _bFormat, speakerOutput );
  _speakerOutputProcessed = true;

  for( unsigned short i = 0; i < speakerOutput.size(); i++ )
  {
    speakerOutput[i] /= _speakers.size();
  }

  return true;
}

//-------------------------------------------------------------------------------------------------

bool Ambisonix::SetOrder( unsigned char order )
{
  if( order > 3 )
  {
    return false;
  }

  for( unsigned long i = 0; i < _sources.size(); i++ )
  {
    _sources[i].SetOrder( order );
  }

  for( unsigned long i = 0; i < _speakers.size(); i++ )
  {
    _speakers[i].SetOrder( order );
  }

  _order = order;
  return true;
}

//=================================================================================================

void Ambisonix::_CartToSph( float& x_azimuth, float& y_elevation, float& z_distance )
{
  float x, y, z;
  x = x_azimuth;
  y = y_elevation;
  z = z_distance;

  x_azimuth = 90 - RadToDeg( atan2( y, x ) );

  if( x < 0 && y <= 0 )				//3rd quadrant
    x_azimuth -= 360;

  y_elevation = RadToDeg( atan2( z, sqrt( x * x + y * y ) ) );
  z_distance = sqrt( x * x + y * y + z * z );
}

//-------------------------------------------------------------------------------------------------

void Ambisonix::_ClampSph( float& azimuth, float& elevation )
{
  while( azimuth > 180 )
  {
    azimuth -= 360;
  }
  while( azimuth <= -180 )
  {
    azimuth += 360;
  }

  if( elevation > 90 )
  {
    elevation = 90;
  }
  else if( elevation < -90 )
  {
    elevation = -90;
  }
}

//=================================================================================================
