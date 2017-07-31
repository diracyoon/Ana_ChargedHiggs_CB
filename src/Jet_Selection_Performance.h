#ifndef __Jet_Selection_Performance_H__
#define __Jet_Selection_Performance_H__

#include <TFile.h>
#include <TString.h>

using namespace std;

class Jet_Selection_Performance : public TObject
{
 public:
  Jet_Selection_Performance(const TString& a_extension);
  ~Jet_Selection_Performance();
  
 protected:
  TString extension;
  
  ClassDef(Jet_Selection_Performance, 1);
};

#endif /* __Jet_Selection_Performance_H__ */
