#ifndef __Fitter_H__
#define __Fitter_H__

#include <iostream>

#include <TObject.h>

using namespace std;

class Fitter : public TObject
{
 public:
  Fitter();
  ~Fitter();
   
 protected:

  ClassDef(Fitter, 1);
};

#endif /*__Fitter_H__*/
