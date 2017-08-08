#ifndef __Jet_Selection_Performance_h__
#define __Jet_Selection_Performance_h__

#include <iostream>

#include <TObject.h>
#include <TFile.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TPaveStats.h>
#include <TCanvas.h>

#include "Const_Def.h"

using namespace std;

class Jet_Selection_Performance : public TObject
{
 public:
  Jet_Selection_Performance(const TString& a_extension, const Int_t& a_cut_on_chi2=50);
  ~Jet_Selection_Performance();

  void Reconstruction_Result_Mass_2B(const Int_t& selection);
  void Reconstruction_Result_Mass_3B(const Int_t& selection);
  
 protected:
  TString extension;
  TString signal_mass[N_SIGNAL] = {"090", "100", "110", "120", "125", "130", "140", "150"};
  TString selection_type[N_SELECTION] = {"P_{t} leading"};
  
  Int_t cut_on_chi2;
  
  //first index: 0 pt leading 
  //second index: low and high mass fitter
  //third index: tt and charged higgs samples
  TFile* fin[N_SELECTION][2][N_SAMPLE];

  TFile* Get_Target_File(const TString& n_b, const Int_t& selection, const Int_t& index);
  
  ClassDef(Jet_Selection_Performance, 1);
};

#endif /* __Jet_Selection_Performance_h__ */
