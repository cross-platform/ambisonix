#ifndef AMBSOURCE_H
#define AMBSOURCE_H

#include "AmbComponent.h"

//=================================================================================================

class AmbSource : public AmbComponent
{
public:
  AmbSource( unsigned long sampleRate = 44100 );
  virtual ~AmbSource();

  bool SetSampleRate( unsigned long sampleRate );
  void SetPosition( PolarCoord position );

  void Process( std::vector< float >& src, AmbFormat& dest );

private:
  unsigned long _sampleRate;

  float _delayFrac;
  unsigned long _delayWhole;
  float _newDelayFrac;
  unsigned long _newDelayWhole;
  unsigned long _delayBufferSize;
  std::vector< float > _delayBuffer;
  unsigned long _inIndex;
  unsigned long _newInIndex;
  unsigned long _outAIndex;
  unsigned long _outBIndex;

  float _lastDistance;
  bool _distanceChanged;

  static void StretchBuffer( std::vector< float >& actAddr, unsigned long actLength, std::vector< float >& strAddr, unsigned long strLength );
};

//=================================================================================================

#endif // AMBSOURCE_H
