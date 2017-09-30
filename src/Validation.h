#ifndef __Validation_h__
#define __Validation_h__

#include <iostream>

#include <TObject.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TCanvas.h>
#include <TH1D.h>

using namespace std;

class Validation : public TObject
{
 public:
  Validation();
  ~Validation();

  void Eta(const TString& target_object);
  
 protected:
  const Int_t n_mc_sample;
  TString* mc_sample_name;
  
  TFile** fin_mc;
  TFile* fin_data;

  TNtuple** ntuple_mc;
  TNtuple* ntuple_data;

  //ntuple variable
  Double_t met;

  Double_t muon_eta;
  Double_t muon_pt;

  Double_t jet0_eta;
  Double_t jet0_pt;
  Double_t jet1_eta;
  Double_t jet1_pt;
  Double_t jet2_eta;
  Double_t jet2_pt;
  Double_t jet3_eta;
  Double_t jet3_pt;

  Double_t n_vertices;

  Double_t weight;
  
  ClassDef(Validation, 1);
};

#endif /* __Validation_h__ */
