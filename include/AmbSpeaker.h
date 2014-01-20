#ifndef AMBSPEAKER_H
#define AMBSPEAKER_H

#include <AmbComponent.h>

//=================================================================================================

class AmbSpeaker : public AmbComponent
{
public:
  AmbSpeaker();
  virtual ~AmbSpeaker();

  void Process( AmbFormat& src, std::vector< float >& dest ) const;
};

//=================================================================================================

#endif // AMBSPEAKER_H
