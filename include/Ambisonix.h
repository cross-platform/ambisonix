#ifndef AMBISONIX_H
#define AMBISONIX_H

#include <vector>
#include <ambisonix/AmbSource.h>
#include <ambisonix/AmbSpeaker.h>
#include <ambisonix/AmbFormat.h>

//=================================================================================================

static float DegToRad( float degrees )
{
  return degrees * 3.14159265358979323846f / 180.f;
}

static float RadToDeg( float radians )
{
  return radians * 180.f / 3.14159265358979323846f;
}

//=================================================================================================

class Ambisonix
{
public:
  Ambisonix( unsigned short bufferSize = 128, unsigned long sampleRate = 44100 );
  virtual ~Ambisonix();

  void SetBufferSize( unsigned short bufferSize );
  void SetSampleRate( unsigned long sampleRate );

  unsigned short GetBufferSize() const;
  unsigned long GetSampleRate() const;

  void AddSourceCart( float x, float y, float z );
  void AddSourceSph( float azimuth, float elevation, float distance );
  bool MoveSourceCart( unsigned short sourceIndex, float x, float y, float z );
  bool MoveSourceSph( unsigned short sourceIndex, float azimuth, float elevation, float distance );
  void ClearSources();
  unsigned short GetSourceCount() const;

  void AddSpeakerCart( float x, float y, float z );
  void AddSpeakerSph( float azimuth, float elevation, float distance );
  bool MoveSpeakerCart( unsigned short speakerIndex, float x, float y, float z );
  bool MoveSpeakerSph( unsigned short speakerIndex, float azimuth, float elevation, float distance );
  void ClearSpeakers();
  unsigned short GetSpeakerCount() const;

  bool SetSourceInput( unsigned short sourceIndex, std::vector< float >& sourceInput );
  bool GetSpeakerOutput( unsigned short speakerIndex, std::vector< float >& speakerOutput );

  bool SetOrder( unsigned char order );

private:
  std::vector< AmbSource > _sources;
  std::vector< AmbSpeaker > _speakers;

  AmbFormat _bFormat;

  unsigned short _bufferSize;
  unsigned long _sampleRate;
  float _speakerRadius;

  bool _speakerOutputProcessed;

  unsigned char _order;

  static void _CartToSph( float& x_azimuth, float& y_elevation, float& z_distance );
  static void _ClampSph( float& azimuth, float& elevation );
};

//=================================================================================================

#endif // AMBISONIX_H
