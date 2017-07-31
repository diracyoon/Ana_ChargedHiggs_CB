#ifndef __Fitter_Performance_H__
#define __Fitter_Performance_H__

#include <iostream>

#include <TObject.h>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <THStack.h>
#include <TPaveStats.h>
#include <TLegend.h>

#include "Const_Def.h"

using namespace std;

class Fitter_Performance : public TObject
{
 public:
  Fitter_Performance(const TString& a_extension="png");
  ~Fitter_Performance();

  void Compare_B_Pt_For_Additional_Constraint();
  void Compare_Dijet_Mass_For_Additional_Constraint();
  void Compare_Jet_Match_Prob_For_Additional_Constraint();
  void Investigation_Fitter_Failure_2B();
  void Investigation_Fitter_Failure_3B();
  void Reconstruction_Result_Chi2_2B();
  void Reconstruction_Result_Chi2_3B();
  void Reconstruction_Result_Chi2_Piece_2B();
  void Reconstruction_Result_Chi2_Piece_3B();
  void Reconstruction_Result_Neutrino();
  void Reconstruction_Result_Mass_2B();
  void Reconstruction_Result_Mass_3B();
  void Reconstruction_Result_Jet_Origin_Match_Prob();
  
 protected:
  TString extension;
  TString signal_mass[N_SIGNAL] = {"090", "100", "110", "120", "125", "130", "140", "150"};

  //first index: no constraint, low mass, and high mass
  //second index: tt sample, charged higgs samples
  TFile* fin[3][N_SAMPLE];

  TFile* Get_Target_File(const Int_t& index); 
  
  ClassDef(Fitter_Performance, 1);
};

#endif /*__Fitter_Performance_H__*/
