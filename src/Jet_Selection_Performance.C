#include "Jet_Selection_Performance.h"

ClassImp(Jet_Selection_Performance);

//////////

Jet_Selection_Performance::Jet_Selection_Performance(const TString& a_extension, const Double_t& a_cut_on_chi2)
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

  TH1D* h_chi2 = (TH1D*)fin[0][0][0]->Get("Chi2_2B");
  cut_on_chi2_bin = h_chi2->FindBin(cut_on_chi2);
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
     
      h_mass_2b[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_2B"))->ProjectionX("_px", 1, cut_on_chi2_bin)->Clone();
      h_mass_2b_ofs[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_2B_OFS"))->ProjectionX("_px", 1, cut_on_chi2_bin)->Clone();
      h_mass_2b_ts_off[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_2B_TS_OFF"))->ProjectionX("_px", 1, cut_on_chi2_bin)->Clone();
      h_mass_2b_tf[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_2B_TF"))->ProjectionX("_px", 1, cut_on_chi2_bin)->Clone();
     
      h_mass_2b_ofs[i]->SetLineColor(2);
      h_mass_2b_ts_off[i]->SetLineColor(3);
      h_mass_2b_tf[i]->SetLineColor(6);

      h_mass_2b_ofs[i]->SetName("TJ: S, OF: S");
      h_mass_2b_ts_off[i]->SetName("TJ: S, OF: F");
      h_mass_2b_tf[i]->SetName("TJ: F, OF: F");
      
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
      if(i==0) h_name = "SM t#bar{t} sample, 2b-tagged events, ";
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-1].Atof();
	  h_name += " GeV sample, 2b-tagged events, ";
	}
      h_name += selection_type[selection];
      h_name += ", #chi^{2}<";
      h_name += cut_on_chi2;
	
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

  can_mass_2b.Print("Mass_OFS_OFF_TF_Sep_2B_"+selection_type_print[selection]+"."+extension, extension);
  
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

      h_mass_3b[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_3B"))->ProjectionX("_px", 0, cut_on_chi2_bin)->Clone();
      h_mass_3b_ofs[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_3B_OFS"))->ProjectionX("_px", 0, cut_on_chi2_bin)->Clone();
      h_mass_3b_ts_off[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_3B_TS_OFF"))->ProjectionX("_px", 0, cut_on_chi2_bin)->Clone();
      h_mass_3b_tf[i] = (TH1D*)((TH2D*)fin_target->Get("DiJetMass_Chi2_3B_TF"))->ProjectionX("_px", 0, cut_on_chi2_bin)->Clone();

      h_mass_3b_ofs[i]->SetLineColor(2);
      h_mass_3b_ts_off[i]->SetLineColor(3);
      h_mass_3b_tf[i]->SetLineColor(6);

      h_mass_3b_ofs[i]->SetName("TJ: S, OF: S");
      h_mass_3b_ts_off[i]->SetName("TJ: S, OF: F");
      h_mass_3b_tf[i]->SetName("TJ: F, OF: F");
      
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
      if(i==0) h_name = "SM t#bar{t} sample, 3b-tagged events, low mass fitter, ";
      else if(i==1) h_name = "SM t#bar{t} sample, 3b-tagged events, high mass fitter, ";
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-2].Atof();
	  h_name += " GeV sample, 3b-tagged events, ";
	}
      h_name += selection_type[selection];
      h_name += ", #chi^{2}<";
      h_name += cut_on_chi2;
      
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

  can_mass_3b.Print("Mass_OFS_OFF_TF_Sep_3B_"+selection_type_print[selection]+"."+extension, extension);
    
  return;
}//void Jet_Selection_Performance::Reconstruction_Result_Mass_3B(const Int_t& selection)

//////////

void Jet_Selection_Performance::Reconstruction_Result_Chi2_2B(const Int_t& selection)
{
  TCanvas can_chi2_2b("can_chi2_2b", "can_chi2_2b", 1000, 1400);
  can_chi2_2b.Divide(2, 5);
  can_chi2_2b.Draw();

  TH1D* h_chi2_2b[N_SAMPLE];
  TH1D* h_chi2_2b_ofs[N_SAMPLE];
  TH1D* h_chi2_2b_ts_off[N_SAMPLE];
  TH1D* h_chi2_2b_tf[N_SAMPLE];
  TPaveStats* stats_chi2_2b_ofs[N_SAMPLE];
  TPaveStats* stats_chi2_2b_ts_off[N_SAMPLE];
  TPaveStats* stats_chi2_2b_tf[N_SAMPLE];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      can_chi2_2b.GetPad(i+1)->SetLogy();

      TFile* fin_target = Get_Target_File("2B", selection, i);
      
      h_chi2_2b[i] = (TH1D*)fin_target->Get("Chi2_2B");
      h_chi2_2b_ofs[i] = (TH1D*)fin_target->Get("Chi2_2B_OFS");
      h_chi2_2b_ts_off[i] = (TH1D*)fin_target->Get("Chi2_2B_TS_OFF");
      h_chi2_2b_tf[i] = (TH1D*)fin_target->Get("Chi2_2B_TF");

      h_chi2_2b_ofs[i]->SetLineColor(2);
      h_chi2_2b_ts_off[i]->SetLineColor(3);
      h_chi2_2b_tf[i]->SetLineColor(6);

      h_chi2_2b_ofs[i]->SetName("TJ: S, OF: S");
      h_chi2_2b_ts_off[i]->SetName("TJ: S, OF: F");
      h_chi2_2b_tf[i]->SetName("TJ: F, OF: F");
      
      can_chi2_2b.cd(i+1);

      h_chi2_2b_ofs[i]->Draw();
      can_chi2_2b.GetPad(i+1)->Update();
      stats_chi2_2b_ofs[i] = (TPaveStats*)h_chi2_2b_ofs[i]->FindObject("stats");

      h_chi2_2b_ts_off[i]->Draw();
      can_chi2_2b.GetPad(i+1)->Update();
      stats_chi2_2b_ts_off[i] = (TPaveStats*)h_chi2_2b_ts_off[i]->FindObject("stats");

      h_chi2_2b_tf[i]->Draw();
      can_chi2_2b.GetPad(i+1)->Update();
      stats_chi2_2b_tf[i] = (TPaveStats*)h_chi2_2b_tf[i]->FindObject("stats");

      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 2b-tagged events, ";
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-1].Atof();
	  h_name += " GeV sample, 2b-tagged events, ";
	}
      h_name += selection_type[selection];
      
      h_chi2_2b[i]->SetTitle(h_name);
      h_chi2_2b[i]->GetXaxis()->SetTitle("#chi^{2}");
      h_chi2_2b[i]->SetStats(kFALSE);
      
      h_chi2_2b[i]->Draw();
      h_chi2_2b_ofs[i]->Draw("same");
      h_chi2_2b_ts_off[i]->Draw("same");
      h_chi2_2b_tf[i]->Draw("same");

      stats_chi2_2b_ofs[i]->SetY2NDC(0.90);
      stats_chi2_2b_ofs[i]->SetY1NDC(0.70);
      stats_chi2_2b_ofs[i]->Draw("same");

      stats_chi2_2b_ts_off[i]->SetY2NDC(0.65);
      stats_chi2_2b_ts_off[i]->SetY1NDC(0.45);
      stats_chi2_2b_ts_off[i]->Draw("same");

      stats_chi2_2b_tf[i]->SetY2NDC(0.40);
      stats_chi2_2b_tf[i]->SetY1NDC(0.20);
      stats_chi2_2b_tf[i]->Draw("same");
    }
  
  can_chi2_2b.Print("Chi2_OFS_OFF_TF_2B_"+selection_type_print[selection]+"."+extension, extension);
  
  return;
}//void Jet_Selection_Performance::Reconstruction_Result_Chi2_2B(const Int_t& selection)

//////////

void Jet_Selection_Performance::Reconstruction_Result_Chi2_3B(const Int_t& selection)
{
  TCanvas can_chi2_3b("can_chi2_3b", "can_chi2_3b", 1000, 1400);
  can_chi2_3b.Divide(2, 5);
  can_chi2_3b.Draw();

  TH1D* h_chi2_3b[N_SIGNAL+1];
  TH1D* h_chi2_3b_ofs[N_SIGNAL+1];
  TH1D* h_chi2_3b_ts_off[N_SIGNAL+1];
  TH1D* h_chi2_3b_tf[N_SIGNAL+1];
  TPaveStats* stats_chi2_3b_ofs[N_SIGNAL+1];
  TPaveStats* stats_chi2_3b_ts_off[N_SIGNAL+1];
  TPaveStats* stats_chi2_3b_tf[N_SIGNAL+1];
  for(Int_t i=0; i<N_SIGNAL+1; i++)
    {
      can_chi2_3b.GetPad(i+1)->SetLogy();

      TFile* fin_target = Get_Target_File("3B", selection, i);
      
      h_chi2_3b[i] = (TH1D*)fin_target->Get("Chi2_3B");
      h_chi2_3b_ofs[i] = (TH1D*)fin_target->Get("Chi2_3B_OFS");
      h_chi2_3b_ts_off[i] = (TH1D*)fin_target->Get("Chi2_3B_TS_OFF");
      h_chi2_3b_tf[i] = (TH1D*)fin_target->Get("Chi2_3B_TF");

      h_chi2_3b_ofs[i]->SetLineColor(2);
      h_chi2_3b_ts_off[i]->SetLineColor(3);
      h_chi2_3b_tf[i]->SetLineColor(6);

      h_chi2_3b_ofs[i]->SetName("TJ: S, OF: S");
      h_chi2_3b_ts_off[i]->SetName("TJ: S, OF: F");
      h_chi2_3b_tf[i]->SetName("TJ: F, OF: F");
            
      can_chi2_3b.cd(i+1);

      h_chi2_3b_ofs[i]->Draw();
      can_chi2_3b.GetPad(i+1)->Update();
      stats_chi2_3b_ofs[i] = (TPaveStats*)h_chi2_3b_ofs[i]->FindObject("stats");

      h_chi2_3b_ts_off[i]->Draw();
      can_chi2_3b.GetPad(i+1)->Update();
      stats_chi2_3b_ts_off[i] = (TPaveStats*)h_chi2_3b_ts_off[i]->FindObject("stats");

      h_chi2_3b_tf[i]->Draw();
      can_chi2_3b.GetPad(i+1)->Update();
      stats_chi2_3b_tf[i] = (TPaveStats*)h_chi2_3b_tf[i]->FindObject("stats");

      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 3b-tagged events, low mass fitter, ";
      else if(i==1) h_name = "SM t#bar{t} sample, 3b-tagged events, high mass fitter, ";
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-2].Atof();
	  h_name += " GeV sample, 3b-tagged events, ";
	}
      h_name += selection_type[selection];

      h_chi2_3b[i]->SetTitle(h_name);
      h_chi2_3b[i]->GetXaxis()->SetTitle("#chi^{2}");
      h_chi2_3b[i]->SetStats(kFALSE);

      h_chi2_3b[i]->Draw();
      h_chi2_3b_ofs[i]->Draw("same");
      h_chi2_3b_ts_off[i]->Draw("same");
      h_chi2_3b_tf[i]->Draw("same");

      stats_chi2_3b_ofs[i]->SetY2NDC(0.90);
      stats_chi2_3b_ofs[i]->SetY1NDC(0.70);
      stats_chi2_3b_ofs[i]->Draw("same");

      stats_chi2_3b_ts_off[i]->SetY2NDC(0.65);
      stats_chi2_3b_ts_off[i]->SetY1NDC(0.45);
      stats_chi2_3b_ts_off[i]->Draw("same");

      stats_chi2_3b_tf[i]->SetY2NDC(0.40);
      stats_chi2_3b_tf[i]->SetY1NDC(0.20);
      stats_chi2_3b_tf[i]->Draw("same");
    }

  can_chi2_3b.Print("Chi2_OFS_OFF_TF_3B_"+selection_type_print[selection]+"."+extension, extension);
    
  return;
}//void Jet_Selection_Performance::Reconstruction_Result_Chi2_3B(const Int_t& selection)

//////////

void Jet_Selection_Performance::Reconstruction_Result_Chi2_Piece_2B(const Int_t& selection)
{
  TCanvas* can_piece_2b;

  TH1D* h_chi2_2b_piece[N_SAMPLE][N_CHI2_PIECE];
  TH1D* h_chi2_2b_piece_ofs[N_SAMPLE][N_CHI2_PIECE];
  TH1D* h_chi2_2b_piece_ts_off[N_SAMPLE][N_CHI2_PIECE];
  TH1D* h_chi2_2b_piece_tf[N_SAMPLE][N_CHI2_PIECE];
  TPaveStats* stats_chi2_2b_piece_ofs[N_SAMPLE][N_CHI2_PIECE];
  TPaveStats* stats_chi2_2b_piece_ts_off[N_SAMPLE][N_CHI2_PIECE];
  TPaveStats* stats_chi2_2b_piece_tf[N_SAMPLE][N_CHI2_PIECE];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      can_piece_2b = new TCanvas("can_piece_2b", "can_piece_2b", 1000, 1200);
      can_piece_2b->Divide(2, 5);

      TFile* fin_target = Get_Target_File("2B", selection, i);
      
      for(Int_t j=0; j<N_CHI2_PIECE; j++)
	{
	  can_piece_2b->GetPad(j+1)->SetLogy();

	  TString h_name = "Chi2_Piece_2B_" + to_string(j);

	  h_chi2_2b_piece[i][j] = (TH1D*)fin_target->Get(h_name);
	  h_chi2_2b_piece_ofs[i][j] = (TH1D*)fin_target->Get(h_name+"_OFS");
	  h_chi2_2b_piece_ts_off[i][j] = (TH1D*)fin_target->Get(h_name+"_TS_OFF");
	  h_chi2_2b_piece_tf[i][j] = (TH1D*)fin_target->Get(h_name+"_TF");

	  h_chi2_2b_piece_ofs[i][j]->SetLineColor(2);
	  h_chi2_2b_piece_ts_off[i][j]->SetLineColor(3);
	  h_chi2_2b_piece_tf[i][j]->SetLineColor(6);

	  h_chi2_2b_piece_ofs[i][j]->SetName("TJ: S, OF: S");
	  h_chi2_2b_piece_ts_off[i][j]->SetName("TJ: S, OF: F");
	  h_chi2_2b_piece_tf[i][j]->SetName("TJ: F, OF: F");
	  	  
	  can_piece_2b->cd(j+1);

	  h_chi2_2b_piece_ofs[i][j]->Draw();
	  can_piece_2b->GetPad(j+1)->Update();
	  stats_chi2_2b_piece_ofs[i][j] = (TPaveStats*)h_chi2_2b_piece_ofs[i][j]->FindObject("stats");

	  h_chi2_2b_piece_ts_off[i][j]->Draw();
	  can_piece_2b->GetPad(j+1)->Update();
	  stats_chi2_2b_piece_ts_off[i][j] = (TPaveStats*)h_chi2_2b_piece_ts_off[i][j]->FindObject("stats");

	  h_chi2_2b_piece_tf[i][j]->Draw();
	  can_piece_2b->GetPad(j+1)->Update();
	  stats_chi2_2b_piece_tf[i][j] = (TPaveStats*)h_chi2_2b_piece_tf[i][j]->FindObject("stats");

	  if(j<4) h_name = "#chi^{2} from P_{T} of Jet" + to_string(j);
	  else if(j==4) h_name = "#chi^{2} from P_{T} of lepton";
	  else if(j==5) h_name = "#chi^{2} from x component of extra";
	  else if(j==6) h_name = "#chi^{2} from y component of extra";
	  else if(j==7) h_name = "#chi^{2} from leptonic W";
	  else if(j==8) h_name = "#chi^{2} from leptonic t";
	  else if(j==9) h_name = "#chi^{2} from hadronic t";

	  h_chi2_2b_piece[i][j]->SetTitle(h_name);
	  h_chi2_2b_piece[i][j]->GetXaxis()->SetTitle("#chi^{2}");
	  h_chi2_2b_piece[i][j]->SetStats(kFALSE);
	  
	  h_chi2_2b_piece[i][j]->Draw();
	  h_chi2_2b_piece_ofs[i][j]->Draw("same");
	  h_chi2_2b_piece_ts_off[i][j]->Draw("same");
	  h_chi2_2b_piece_tf[i][j]->Draw("same");

	  stats_chi2_2b_piece_ofs[i][j]->SetY2NDC(0.90);
	  stats_chi2_2b_piece_ofs[i][j]->SetY1NDC(0.70);
	  stats_chi2_2b_piece_ofs[i][j]->Draw("same");

	  stats_chi2_2b_piece_ts_off[i][j]->SetY2NDC(0.65);
	  stats_chi2_2b_piece_ts_off[i][j]->SetY1NDC(0.45);
	  stats_chi2_2b_piece_ts_off[i][j]->Draw("same");

	  stats_chi2_2b_piece_tf[i][j]->SetY2NDC(0.40);
	  stats_chi2_2b_piece_tf[i][j]->SetY1NDC(0.20);
	  stats_chi2_2b_piece_tf[i][j]->Draw("same");
	}//N_CHI2_PIECE
      
      TString name = "Chi2_Piece_";
      if(i==0) name += "SM_TT";
      else name += "CH_" + signal_mass[i-1];
      name += "_OFS_OFF_TF_Sep_2B_";
      name += selection_type_print[selection]+".";
           
      can_piece_2b->Print(name+extension, extension);

      delete can_piece_2b;
    }//N_SAMPLE
  
  return;
}//void Jet_Selection_Performance::Reconstruction_Result_Chi2_Piece_2B(const Int_t& selection)

//////////

void Jet_Selection_Performance::Reconstruction_Result_Chi2_Piece_3B(const Int_t& selection)
{
  TCanvas* can_piece_3b;

  TH1D* h_chi2_3b_piece[N_SAMPLE+1][N_CHI2_PIECE];
  TH1D* h_chi2_3b_piece_ofs[N_SAMPLE+1][N_CHI2_PIECE];
  TH1D* h_chi2_3b_piece_ts_off[N_SAMPLE+1][N_CHI2_PIECE];
  TH1D* h_chi2_3b_piece_tf[N_SAMPLE+1][N_CHI2_PIECE];
  TPaveStats* stats_chi2_3b_piece_ofs[N_SAMPLE+1][N_CHI2_PIECE];
  TPaveStats* stats_chi2_3b_piece_ts_off[N_SAMPLE+1][N_CHI2_PIECE];
  TPaveStats* stats_chi2_3b_piece_tf[N_SAMPLE+1][N_CHI2_PIECE];
  for(Int_t i=0; i<N_SAMPLE+1; i++)
    {
      can_piece_3b = new TCanvas("can_piece_3b", "can_piece_3b", 1000, 1200);
      can_piece_3b->Divide(2, 5);

      TFile* fin_target = Get_Target_File("3B", selection, i);
      
      for(Int_t j=0; j<N_CHI2_PIECE; j++)
	{
	  can_piece_3b->GetPad(j+1)->SetLogy();
	  
	  TString h_name = "Chi2_Piece_3B_" + to_string(j);
	  
	  h_chi2_3b_piece[i][j] = (TH1D*)fin_target->Get(h_name);
	  h_chi2_3b_piece_ofs[i][j] = (TH1D*)fin_target->Get(h_name+"_OFS");
	  h_chi2_3b_piece_ts_off[i][j] = (TH1D*)fin_target->Get(h_name+"_TS_OFF");
	  h_chi2_3b_piece_tf[i][j] = (TH1D*)fin_target->Get(h_name+"_TF");

	  h_chi2_3b_piece_ofs[i][j]->SetLineColor(2);
	  h_chi2_3b_piece_ts_off[i][j]->SetLineColor(3);
	  h_chi2_3b_piece_tf[i][j]->SetLineColor(6);

	  h_chi2_3b_piece_ofs[i][j]->SetName("TJ: S, OF: S");
	  h_chi2_3b_piece_ts_off[i][j]->SetName("TJ: S, OF: F");
	  h_chi2_3b_piece_tf[i][j]->SetName("TJ: F, OF: F");
	  	  
	  can_piece_3b->cd(j+1);

	  h_chi2_3b_piece_ofs[i][j]->Draw();
	  can_piece_3b->GetPad(j+1)->Update();
	  stats_chi2_3b_piece_ofs[i][j] = (TPaveStats*)h_chi2_3b_piece_ofs[i][j]->FindObject("stats");

	  h_chi2_3b_piece_ts_off[i][j]->Draw();
	  can_piece_3b->GetPad(j+1)->Update();
	  stats_chi2_3b_piece_ts_off[i][j] = (TPaveStats*)h_chi2_3b_piece_ts_off[i][j]->FindObject("stats");

	  h_chi2_3b_piece_tf[i][j]->Draw();
	  can_piece_3b->GetPad(j+1)->Update();
	  stats_chi2_3b_piece_tf[i][j] = (TPaveStats*)h_chi2_3b_piece_tf[i][j]->FindObject("stats");

	  if(j<4) h_name = "#chi^{2} from P_{T} of Jet" + to_string(j);
	  else if(j==4) h_name = "#chi^{2} from P_{T} of lepton";
	  else if(j==5) h_name = "#chi^{2} from x component of extra";
	  else if(j==6) h_name = "#chi^{2} from y component of extra";
	  else if(j==7) h_name = "#chi^{2} from leptonic W";
	  else if(j==8) h_name = "#chi^{2} from leptonic t";
	  else if(j==9) h_name = "#chi^{2} from hadronic t";

	  h_chi2_3b_piece[i][j]->SetTitle(h_name);
          h_chi2_3b_piece[i][j]->GetXaxis()->SetTitle("#chi^{2}");
	  h_chi2_3b_piece[i][j]->SetStats(kFALSE);
	    
	  h_chi2_3b_piece[i][j]->Draw();
	  h_chi2_3b_piece_ofs[i][j]->Draw("same");
	  h_chi2_3b_piece_ts_off[i][j]->Draw("same");
	  h_chi2_3b_piece_tf[i][j]->Draw("same");

	  stats_chi2_3b_piece_ofs[i][j]->SetY2NDC(0.90);
	  stats_chi2_3b_piece_ofs[i][j]->SetY1NDC(0.70);
	  stats_chi2_3b_piece_ofs[i][j]->Draw("same");

	  stats_chi2_3b_piece_ts_off[i][j]->SetY2NDC(0.65);
	  stats_chi2_3b_piece_ts_off[i][j]->SetY1NDC(0.45);
	  stats_chi2_3b_piece_ts_off[i][j]->Draw("same");

	  stats_chi2_3b_piece_tf[i][j]->SetY2NDC(0.40);
	  stats_chi2_3b_piece_tf[i][j]->SetY1NDC(0.20);
	  stats_chi2_3b_piece_tf[i][j]->Draw("same");
	}//N_CHI2_PIECE	  

      TString name = "Chi2_Piece_";
      if(i==0) name += "SM_TT_Low_Mass";
      else if(i==1) name += "SM_TT_High_Mass";
      else name += "CH_" + signal_mass[i-2];
      name += "_OFS_OFF_TF_Sep_3B_";
      name += selection_type_print[selection]+".";
      
      can_piece_3b->Print(name+extension, extension);

      delete can_piece_3b;
    }//N_SAMPLE+1
  
  return;
}//void Jet_Selection_Performance::Reconstruction_Result_Chi2_Piece_3B(const Int_t& selection)

//////////

void Jet_Selection_Performance::Signal_Fraction_Vs_Chi2_2B(const Int_t& selection)
{
  TCanvas can_chi2_dep_2b("can_chi2_dep_2b", "can_chi2_dep_2b", 1000, 1400);
  can_chi2_dep_2b.Divide(2, 5);
  can_chi2_dep_2b.Draw();

  TH2D* h_mass_chi2_2b[N_SAMPLE];
  TH2D* h_mass_chi2_2b_ofs[N_SAMPLE];
  TH2D* h_mass_chi2_2b_ts_off[N_SAMPLE];
  TH2D* h_mass_chi2_2b_tf[N_SAMPLE];
  TGraph gr_chi2_dep_2b_ofs[N_SAMPLE];
  TGraph gr_chi2_dep_2b_ts_off[N_SAMPLE];
  TGraph gr_chi2_dep_2b_tf[N_SAMPLE];
  TLegend tl[N_SAMPLE];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      TFile* fin_target = Get_Target_File("2B", selection, i);
      
      h_mass_chi2_2b[i] = (TH2D*)fin_target->Get("DiJetMass_Chi2_2B");
      h_mass_chi2_2b_ofs[i] = (TH2D*)fin_target->Get("DiJetMass_Chi2_2B_OFS");
      h_mass_chi2_2b_ts_off[i] = (TH2D*)fin_target->Get("DiJetMass_Chi2_2B_TS_OFF");
      h_mass_chi2_2b_tf[i] = (TH2D*)fin_target->Get("DiJetMass_Chi2_2B_TF");

      for(Int_t j=0; j<50; j++)
	{
	  Double_t chi2_cut = 49.99 - j;
	  Int_t chi2_cut_bin = h_mass_chi2_2b[i]->ProjectionY()->FindBin(chi2_cut);

	  Double_t count = h_mass_chi2_2b[i]->ProjectionX("_px", 1, chi2_cut_bin)->Integral();
	  Double_t count_ofs = h_mass_chi2_2b_ofs[i]->ProjectionX("_px", 1, chi2_cut_bin)->Integral();
	  Double_t count_ts_off = h_mass_chi2_2b_ts_off[i]->ProjectionX("_px", 1, chi2_cut_bin)->Integral();
	  Double_t count_tf = h_mass_chi2_2b_tf[i]->ProjectionX("_px", 1, chi2_cut_bin)->Integral();

	  Double_t ratio_ofs = count_ofs/count;
	  Double_t ratio_ts_off = count_ts_off/count;
	  Double_t ratio_tf = count_tf/count;
	  //Double_t ratio_signal = (count_ofs+count_ts_off)/count_tf;

	  gr_chi2_dep_2b_ofs[i].SetPoint(j, chi2_cut, ratio_ofs);
	  gr_chi2_dep_2b_ts_off[i].SetPoint(j, chi2_cut, ratio_ts_off);
	  gr_chi2_dep_2b_tf[i].SetPoint(j, chi2_cut, ratio_tf);
	}

      can_chi2_dep_2b.cd(i+1);

      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 2b-tagged events, ";
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-1].Atof();
	  h_name += " GeV sample, 2b-tagged events, ";
	}
      h_name += selection_type[selection];
      
      gr_chi2_dep_2b_ofs[i].SetTitle(h_name);
      gr_chi2_dep_2b_ofs[i].GetXaxis()->SetTitle("Cut #chi^{2}<");
      gr_chi2_dep_2b_ofs[i].GetYaxis()->SetTitle("Frac.");
      gr_chi2_dep_2b_ofs[i].GetYaxis()->SetRangeUser(0, 1.1);

      gr_chi2_dep_2b_ofs[i].SetMarkerStyle(20);
      gr_chi2_dep_2b_ofs[i].SetMarkerColor(2);
      gr_chi2_dep_2b_ofs[i].SetLineColor(2);

      gr_chi2_dep_2b_ts_off[i].SetMarkerStyle(21);
      gr_chi2_dep_2b_ts_off[i].SetMarkerColor(3);
      gr_chi2_dep_2b_ts_off[i].SetLineColor(3);

      gr_chi2_dep_2b_tf[i].SetMarkerStyle(29);
      gr_chi2_dep_2b_tf[i].SetMarkerColor(6);
      gr_chi2_dep_2b_tf[i].SetLineColor(6);

      gr_chi2_dep_2b_ofs[i].Draw("ACP");
      gr_chi2_dep_2b_ts_off[i].Draw("samep");
      gr_chi2_dep_2b_tf[i].Draw("samep");

      tl[i].SetNColumns(3);
      tl[i].SetX1(0.1);
      tl[i].SetX2(0.7);
      tl[i].SetY1(0.8);
      tl[i].SetY2(0.9);
      tl[i].AddEntry(&gr_chi2_dep_2b_ofs[i], "TJ: S, OF: S", "lp");
      tl[i].AddEntry(&gr_chi2_dep_2b_ts_off[i], "TJ: S, OF: F", "lp");
      tl[i].AddEntry(&gr_chi2_dep_2b_tf[i], "TJ: F, OF: F", "lp");
      tl[i].Draw("same");
    }

  can_chi2_dep_2b.Print("Signal_Fraction_Vs_Chi2_2B_"+selection_type_print[selection]+"."+extension, extension);
  
  return;
}//void Jet_Selection_Performance::Signal_Fraction_Vs_Chi2_2B(const Int_t& selection)

//////////

void Jet_Selection_Performance::Signal_Fraction_Vs_Chi2_3B(const Int_t& selection)
{
  TCanvas can_chi2_dep_3b("can_chi2_dep_3b", "can_chi2_dep_3b", 1000, 1400);
  can_chi2_dep_3b.Divide(2, 5);
  can_chi2_dep_3b.Draw();

  TH2D* h_mass_chi2_3b[N_SAMPLE+1];
  TH2D* h_mass_chi2_3b_ofs[N_SAMPLE+1];
  TH2D* h_mass_chi2_3b_ts_off[N_SAMPLE+1];
  TH2D* h_mass_chi2_3b_tf[N_SAMPLE+1];
  TGraph gr_chi2_dep_3b_ofs[N_SAMPLE+1];
  TGraph gr_chi2_dep_3b_ts_off[N_SAMPLE+1];
  TGraph gr_chi2_dep_3b_tf[N_SAMPLE+1];
  TLegend tl[N_SAMPLE+1];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      TFile* fin_target = Get_Target_File("3B", selection, i);
      
      h_mass_chi2_3b[i] = (TH2D*)fin_target->Get("DiJetMass_Chi2_3B");
      h_mass_chi2_3b_ofs[i] = (TH2D*)fin_target->Get("DiJetMass_Chi2_3B_OFS");
      h_mass_chi2_3b_ts_off[i] = (TH2D*)fin_target->Get("DiJetMass_Chi2_3B_TS_OFF");
      h_mass_chi2_3b_tf[i] = (TH2D*)fin_target->Get("DiJetMass_Chi2_3B_TF");

      for(Int_t j=0; j<50; j++)
	{
	  Double_t chi2_cut = 49.99 - j;
	  Int_t chi2_cut_bin = h_mass_chi2_3b[i]->ProjectionY()->FindBin(chi2_cut);

	  Double_t count = h_mass_chi2_3b[i]->ProjectionX("_px", 0, chi2_cut_bin)->Integral();
	  Double_t count_ofs = h_mass_chi2_3b_ofs[i]->ProjectionX("_px", 0, chi2_cut_bin)->Integral();
	  Double_t count_ts_off = h_mass_chi2_3b_ts_off[i]->ProjectionX("_px", 0, chi2_cut_bin)->Integral();
	  Double_t count_tf = h_mass_chi2_3b_tf[i]->ProjectionX("_px", 1, chi2_cut_bin)->Integral();

	  Double_t ratio_ofs = count_ofs/count;
	  Double_t ratio_ts_off = count_ts_off/count;
	  Double_t ratio_tf = count_tf/count;
	  //Double_t ratio_signal = (count_ofs+count_ts_off)/count_tf;

	  gr_chi2_dep_3b_ofs[i].SetPoint(j, chi2_cut, ratio_ofs);
	  gr_chi2_dep_3b_ts_off[i].SetPoint(j, chi2_cut, ratio_ts_off);
	  gr_chi2_dep_3b_tf[i].SetPoint(j, chi2_cut, ratio_tf);
	}//chi2

      can_chi2_dep_3b.cd(i+1);

      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 3b-tagged events, low mass fitter, ";
      else if(i==1) h_name = "SM t#bar{t} sample, 3b-tagged events, high mass fitter, ";
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-2].Atof();
	  h_name += " GeV sample, 3b-tagged events, ";
	}
      h_name += selection_type[selection];

      gr_chi2_dep_3b_ofs[i].SetTitle(h_name);
      gr_chi2_dep_3b_ofs[i].GetXaxis()->SetTitle("Cut #chi^{2}<");
      gr_chi2_dep_3b_ofs[i].GetYaxis()->SetTitle("Frac.");
      gr_chi2_dep_3b_ofs[i].GetYaxis()->SetRangeUser(0, 1.1);

      gr_chi2_dep_3b_ofs[i].SetMarkerStyle(20);
      gr_chi2_dep_3b_ofs[i].SetMarkerColor(2);
      gr_chi2_dep_3b_ofs[i].SetLineColor(2);

      gr_chi2_dep_3b_ts_off[i].SetMarkerStyle(21);
      gr_chi2_dep_3b_ts_off[i].SetMarkerColor(3);
      gr_chi2_dep_3b_ts_off[i].SetLineColor(3);

      gr_chi2_dep_3b_tf[i].SetMarkerStyle(29);
      gr_chi2_dep_3b_tf[i].SetMarkerColor(6);
      gr_chi2_dep_3b_tf[i].SetLineColor(6);

      gr_chi2_dep_3b_ofs[i].Draw("ACP");
      gr_chi2_dep_3b_ts_off[i].Draw("samep");
      gr_chi2_dep_3b_tf[i].Draw("samep");

      tl[i].SetNColumns(3);
      tl[i].SetX1(0.1);
      tl[i].SetX2(0.7);
      tl[i].SetY1(0.8);
      tl[i].SetY2(0.9);
      tl[i].AddEntry(&gr_chi2_dep_3b_ofs[i], "TJ: S, OF: S", "lp");
      tl[i].AddEntry(&gr_chi2_dep_3b_ts_off[i], "TJ: S, OF: F", "lp");
      tl[i].AddEntry(&gr_chi2_dep_3b_tf[i], "TJ: F, OF: F", "lp");
      tl[i].Draw("same");
    }

  can_chi2_dep_3b.Print("Signal_Fraction_Vs_Chi2_3B_"+selection_type_print[selection]+"."+extension, extension);
  
  return;
}//void Jet_Selection_Performance::Signal_Fraction_Vs_Chi2_2B(const Int_t& selection)

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
