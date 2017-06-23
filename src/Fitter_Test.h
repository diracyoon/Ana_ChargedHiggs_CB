#ifndef __Fitter_Test_H__
#define __Fitter_Test_H__

#include <iostream>

#include <TObject.h>

using namespace std;

class Fitter_Test : public TObject
{
 public:
  Fitter_Test();
  ~Fitter_Test();
   
 protected:

  ClassDef(Fitter_Test, 1);
};

#endif /*__Fitter_Test_H__*/
