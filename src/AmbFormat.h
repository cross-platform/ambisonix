#ifndef AMBFORMAT_H
#define AMBFORMAT_H

#include <vector>

//=================================================================================================

class AmbFormat
{
public:
  AmbFormat( unsigned short bufferSize = 128 );
  virtual ~AmbFormat();

  bool SetBufferSize( unsigned short bufferSize );
  unsigned short GetBufferSize() const;

  void Clear();

  AmbFormat& operator+( const AmbFormat& rhs );
  AmbFormat& operator+=( const AmbFormat& rhs );

  std::vector< float > data[17];

private:
  unsigned short _bufferSize;
};

//=================================================================================================

#endif // AMBFORMAT_H
