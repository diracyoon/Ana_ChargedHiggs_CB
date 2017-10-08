#ifndef __Validation_MuJet_h__
#define __Validation_MuJet_h__

#include <iostream>

#include <TObject.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <THStack.h>
#include <TGraphErrors.h>
#include <TMath.h>

#include "Const_Def.h"

using namespace std;

class Validation_MuJet : public TObject
{
 public:
  Validation_MuJet();
  ~Validation_MuJet();

  void Eta();
  
 protected:
  const Int_t n_mc_sample;
  TString* mc_sample_name;
  
  TFile** fin_mc;
  TFile* fin_data;
  
  TFile* fout;
  
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
  Double_t n_b_tag;
  
  Double_t weight;
  
  Double_t top_pair_reweight;

  Double_t trigger_sf_down;
  Double_t trigger_sf;
  Double_t trigger_sf_up;

  Double_t weight_by_trigger;

  Double_t mc_weight;

  Double_t pileup_reweight_down;
  Double_t pileup_reweight;
  Double_t pileup_reweight_up;

  Double_t muon_id_sf_down;
  Double_t muon_id_sf;
  Double_t muon_id_sf_up;

  Double_t muon_iso_sf_down;
  Double_t muon_iso_sf;
  Double_t muon_iso_sf_up;

  Double_t muon_tracking_eff_sf;
  
  Double_t b_tag_reweight;
  
  void Clear(TH1D****& histo_mc, THStack***& stack_mc, TH1D***& histo_data, TGraphErrors***& gr_comp);
  void Compare_MC_Data(TH1D* histo_data, THStack* stack_mc, TGraphErrors* gr_comp);
  Int_t Index_N_B_Tag();
  void Make_Graph_Comparison(TGraphErrors***& gr_comp, const TString& target);
  void Make_Histo_Data(TH1D***& histo_data, const TString& target);
  void Make_Histo_MC(TH1D****& histo_mc, const TString& target);
  void Make_Stack_MC(THStack***& stack_mc, const TString& target);
  void Stack_MC_Histo(TH1D****& histo_mc, THStack***& stack_mc);
  void SetBranchAddress(TFile* fin, TNtuple*& ntuple);
  void Write(TH1D****& histo_mc, THStack***& stack_mc, TH1D***& histo_data, TGraphErrors***& gr_comp);
  
  ClassDef(Validation_MuJet, 1);
};

#endif /* __Validation_MuJet_h__ */
