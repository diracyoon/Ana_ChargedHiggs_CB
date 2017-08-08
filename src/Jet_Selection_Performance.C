#include "Jet_Selection_Performance.h"

ClassImp(Jet_Selection_Performance);

//////////

Jet_Selection_Performance::Jet_Selection_Performance(const TString& a_extension, const Int_t& a_cut_on_chi2)
{
  extension = a_extension;
  cut_on_chi2 = a_cut_on_chi2;
  
  for(Int_t i=0; i<N_SELECTION; i++)
    {
      TString selection;
      if(i==0) selection = "Pt_Leading";
      
      for(Int_t j=0; j<2; j++)
	{
	  TString type;
	  if(j==0) type = "Low_Mass";
	  else if(j==1) type = "High_Mass";
	  
	  for(Int_t k=0; k<N_SAMPLE; k++)
	    {
	      TString f_name;
	      if(k==0) f_name = "Jet_Selection_Test_0_SKTT_powheg_cat_v8-0-7.root";
	      else f_name = "Jet_Selection_Test_0_SKCHToCB_M" + signal_mass[k-1] + "_madgraph_13TeV_2016_cat_v8-0-7.root";

	      TString target_name = "./" + selection + "/" + type + "/" + f_name;

	      fin[i][j][k] = new TFile(target_name);
	    }//N_SAMPLE
	}//fitter type
    }//N_SELECTION
}//Jet_Selection_Performance::Jet_Selection_Performance(const TString& a_extension)

//////////

Jet_Selection_Performance::~Jet_Selection_Performance()
{
  for(Int_t i=0; i<N_SELECTION; i++)
    {
      for(Int_t j=0; j<2; j++)
	{
	  for(Int_t k=0; k<N_SAMPLE; k++)
	    {
	      fin[i][j][k]->Close();
	    }//N_SAMPLE
	}//fitter type
    }//N_SELECTION
}//Jet_Selection_Performance::~Jet_Selection_Performance()

//////////

void Jet_Selection_Performance::Reconstruction_Result_Mass_2B(const Int_t& selection)
{
  TCanvas can_mass_2b("can_mass_2b", "can_mass_2b", 1000, 1400);
  can_mass_2b.Divide(2, 5);
  can_mass_2b.Draw();
  
  TH1D* h_mass_2b[N_SAMPLE];
  TH1D* h_mass_2b_ofs[N_SAMPLE];
  TH1D* h_mass_2b_ts_off[N_SAMPLE];
  TH1D* h_mass_2b_tf[N_SAMPLE];
  TPaveStats* stats_mass_2b_ofs[N_SAMPLE];
  TPaveStats* stats_mass_2b_ts_off[N_SAMPLE];
  TPaveStats* stats_mass_2b_tf[N_SAMPLE];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      TFile* fin_target = Get_Target_File("2B", selection, i);
     
      h_mass_2b[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_2B"))->ProjectionX("_px", 0, cut_on_chi2)->Clone();
      h_mass_2b_ofs[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_2B_OFS"))->ProjectionX("_px", 0, cut_on_chi2)->Clone();
      h_mass_2b_ts_off[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_2B_TS_OFF"))->ProjectionX("_px", 0, cut_on_chi2)->Clone();
      h_mass_2b_tf[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_2B_TF"))->ProjectionX("_px", 0, cut_on_chi2)->Clone();
     
      h_mass_2b_ofs[i]->SetLineColor(2);
      h_mass_2b_ts_off[i]->SetLineColor(3);
      h_mass_2b_tf[i]->SetLineColor(6);

      h_mass_2b_ofs[i]->SetName("OF: S, TJ: S");
      h_mass_2b_ts_off[i]->SetName("OF: F, TJ: S");
      h_mass_2b_tf[i]->SetName("OF: F, TJ: F");
      
      can_mass_2b.cd(i+1);

      h_mass_2b_ofs[i]->Draw();
      can_mass_2b.GetPad(i+1)->Update();
      stats_mass_2b_ofs[i] = (TPaveStats*)h_mass_2b_ofs[i]->FindObject("stats");

      h_mass_2b_ts_off[i]->Draw();
      can_mass_2b.GetPad(i+1)->Update();
      stats_mass_2b_ts_off[i] = (TPaveStats*)h_mass_2b_ts_off[i]->FindObject("stats");

      h_mass_2b_tf[i]->Draw();
      can_mass_2b.GetPad(i+1)->Update();
      stats_mass_2b_tf[i] = (TPaveStats*)h_mass_2b_tf[i]->FindObject("stats");

      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 2b-tagged events, " + selection_type[selection];
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-1].Atof();
	  h_name += " GeV sample, 2b-tagged events, " + selection_type[selection];
	}
      h_mass_2b[i]->SetTitle(h_name);
      h_mass_2b[i]->GetXaxis()->SetTitle("M_{jj} [GeV]");
      h_mass_2b[i]->SetStats(kFALSE);
      
      h_mass_2b[i]->Draw();
      h_mass_2b_ofs[i]->Draw("same");
      h_mass_2b_ts_off[i]->Draw("same");
      h_mass_2b_tf[i]->Draw("same");

      stats_mass_2b_ofs[i]->SetY2NDC(0.90);
      stats_mass_2b_ofs[i]->SetY1NDC(0.70);
      stats_mass_2b_ofs[i]->Draw("same");

      stats_mass_2b_ts_off[i]->SetY2NDC(0.65);
      stats_mass_2b_ts_off[i]->SetY1NDC(0.45);
      stats_mass_2b_ts_off[i]->Draw("same");

      stats_mass_2b_tf[i]->SetY2NDC(0.40);
      stats_mass_2b_tf[i]->SetY1NDC(0.20);
      stats_mass_2b_tf[i]->Draw("same");
    }//N_SAMPLE

  can_mass_2b.Print("Mass_OFS_OFF_TF_Sep_2B."+extension, extension);
  
  return;
}//void Jet_Selection_Performance::Reconstruction_Result_Neutrino()

//////////

void Jet_Selection_Performance::Reconstruction_Result_Mass_3B(const Int_t& selection)
{
  TCanvas can_mass_3b("can_mass_3b", "can_mass_3b", 1000, 1400);
  can_mass_3b.Divide(2, 5);
  can_mass_3b.Draw();
  
  TH1D* h_mass_3b[N_SAMPLE+1];
  TH1D* h_mass_3b_ofs[N_SAMPLE+1];
  TH1D* h_mass_3b_ts_off[N_SAMPLE+1];
  TH1D* h_mass_3b_tf[N_SAMPLE+1];
  TPaveStats* stats_mass_3b_ofs[N_SAMPLE+1];
  TPaveStats* stats_mass_3b_ts_off[N_SAMPLE+1];
  TPaveStats* stats_mass_3b_tf[N_SAMPLE+1];
  for(Int_t i=0; i<N_SAMPLE+1; i++)
    {
      TFile* fin_target = Get_Target_File("3B", selection, i);

      h_mass_3b[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_3B"))->ProjectionX("_px", 0, cut_on_chi2)->Clone();
      h_mass_3b_ofs[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_3B_OFS"))->ProjectionX("_px", 0, cut_on_chi2)->Clone();
      h_mass_3b_ts_off[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_3B_TS_OFF"))->ProjectionX("_px", 0, cut_on_chi2)->Clone();
      h_mass_3b_tf[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_3B_TF"))->ProjectionX("_px", 0, cut_on_chi2)->Clone();

      h_mass_3b_ofs[i]->SetLineColor(2);
      h_mass_3b_ts_off[i]->SetLineColor(3);
      h_mass_3b_tf[i]->SetLineColor(6);

      h_mass_3b_ofs[i]->SetName("OF: S, TJ: S");
      h_mass_3b_ts_off[i]->SetName("OF: F, TJ: S");
      h_mass_3b_tf[i]->SetName("OF: F, TJ: F");
      
      can_mass_3b.cd(i+1);

      h_mass_3b_ofs[i]->Draw();
      can_mass_3b.GetPad(i+1)->Update();
      stats_mass_3b_ofs[i] = (TPaveStats*)h_mass_3b_ofs[i]->FindObject("stats");

      h_mass_3b_ts_off[i]->Draw();
      can_mass_3b.GetPad(i+1)->Update();
      stats_mass_3b_ts_off[i] = (TPaveStats*)h_mass_3b_ts_off[i]->FindObject("stats");

      h_mass_3b_tf[i]->Draw();
      can_mass_3b.GetPad(i+1)->Update();
      stats_mass_3b_tf[i] = (TPaveStats*)h_mass_3b_tf[i]->FindObject("stats");

      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 3b-tagged events, " + selection_type[selection];
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-1].Atof();
	  h_name += " GeV sample, 3b-tagged events, " + selection_type[selection];
	}
      h_mass_3b[i]->SetTitle(h_name);
      h_mass_3b[i]->GetXaxis()->SetTitle("M_{jj} [GeV]");
      h_mass_3b[i]->SetStats(kFALSE);
      
      h_mass_3b[i]->Draw();
      h_mass_3b_ofs[i]->Draw("same");
      h_mass_3b_ts_off[i]->Draw("same");
      h_mass_3b_tf[i]->Draw("same");

      stats_mass_3b_ofs[i]->SetY2NDC(0.90);
      stats_mass_3b_ofs[i]->SetY1NDC(0.70);
      stats_mass_3b_ofs[i]->Draw("same");

      stats_mass_3b_ts_off[i]->SetY2NDC(0.65);
      stats_mass_3b_ts_off[i]->SetY1NDC(0.45);
      stats_mass_3b_ts_off[i]->Draw("same");

      stats_mass_3b_tf[i]->SetY2NDC(0.40);
      stats_mass_3b_tf[i]->SetY1NDC(0.20);
      stats_mass_3b_tf[i]->Draw("same");
    }

  can_mass_3b.Print("Mass_OFS_OFF_TF_Sep_3B."+extension, extension);
  
  return;
}//void Jet_Selection_Performance::Reconstruction_Result_Mass_3B(const Int_t& selection)

//////////

TFile* Jet_Selection_Performance::Get_Target_File(const TString& n_b, const Int_t& selection, const Int_t& index)
{
  if(n_b=="2B") return fin[selection][0][index]; 
  else if(n_b=="3B")
    {
      if(index==0) return fin[selection][0][0];//low mass fitter, tt
      else if(index==1) return fin[selection][1][0];//high mass fitter, tt
      else if(index<5) return fin[selection][0][index-1];//charged higgs sample, mass 90, 100, and 110
      else return fin[selection][1][index-1];//charged higgs sample, mass 120, 125, 130, 140, and 150
    }
  
  return NULL;
}//TFile* Jet_Selection_Performance::Get_Target_File(const TString& n_b, const Int_t& selection, const Int_t& index

//////////
