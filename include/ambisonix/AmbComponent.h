#ifndef AMBCOMPONENT_H
#define AMBCOMPONENT_H

#include <math.h>
#include <ambisonix/AmbFormat.h>

//=================================================================================================

struct PolarCoord
{
  float azimuth;
  float elevation;
  float distance;
};

//=================================================================================================

class AmbComponent
{
public:
  AmbComponent();
  virtual ~AmbComponent();

  virtual void SetPosition( PolarCoord position );
  PolarCoord GetPosition() const;

  void SetSpeakerRadius( float speakerRadius );
  float GetSpeakerRadius() const;

  bool SetOrder( unsigned char order );

protected:
  float coeffValues_[17];
  float coeffWeights_[17];
  PolarCoord position_;

  unsigned char order_;
  unsigned char nCoeff_;

  float _speakerRadius;
};

//=================================================================================================

#endif // AMBCOMPONENT_H
