#include "Fitter_Performance.h"

ClassImp(Fitter_Performance);

//////////

Fitter_Performance::Fitter_Performance(const TString& a_extension)
{
  extension = a_extension;
  
  for(Int_t i=0; i<3; i++)
    {
      TString type = "./";
      if(i==0) type += "No_Constraint/";
      else if(i==1) type += "Low_Mass/";
      else if(i==2) type += "High_Mass/";
      
      for(Int_t j=0; j<N_SAMPLE; j++)
	{
	  TString f_name = type;
	  if(j==0) f_name += "Fitter_Test_SKTT_powheg_cat_v8-0-7.root";
	  else f_name += "Fitter_Test_SKCHToCB_M" + signal_mass[j-1] + "_madgraph_13TeV_2016_cat_v8-0-7.root";

	  fin[i][j] = new TFile(f_name);
	}
    }
}//Fitter_Performance::Fitter_Performance()

//////////

Fitter_Performance::~Fitter_Performance()
{
  for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<N_SIGNAL+1; j++)
	{
	  fin[i][j]->Close();

	  delete fin[i][j];
	}
    }
}//Fitter_Performance::~Fitter_Performance()

//////////

void Fitter_Performance::Compare_B_Pt_For_Additional_Constraint()
{
  TGraph gr_comp_pt;
  TH1D* h_comp_pt[N_SAMPLE];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      h_comp_pt[i] = (TH1D*)fin[0][i]->Get("Two_B_Pt_Comparison");

      Double_t mean_pt = h_comp_pt[i]->GetMean();

      Double_t x_point;
      if(i==0) x_point = W_MASS;
      else x_point = signal_mass[i-1].Atof();

      gr_comp_pt.SetPoint(i, x_point, mean_pt);
    }
  
  TCanvas can_b_comp_pt("can_b_comp_pt", "can_b_comp_pt", 800, 500);
  can_b_comp_pt.Draw();
  can_b_comp_pt.SetGridy();
    
  gr_comp_pt.SetTitle("P_{T} Comparison between Jet 1 and Jet 2");
  gr_comp_pt.GetXaxis()->SetTitle("h^{#pm} signal mass [GeV]");
  gr_comp_pt.GetYaxis()->SetTitle("Prob. P^{Jet[1]}_{T}<P^{Jet[2]}_{T}");
  gr_comp_pt.GetYaxis()->SetRangeUser(0.3, 0.9);
  gr_comp_pt.GetYaxis()->SetTitleOffset(1.2);
  gr_comp_pt.Draw("AC*");
  
  can_b_comp_pt.Print("Comp_B_Pt_Constraint."+extension, extension);
  
  return;
}//void Fitter_Performance::Compare_B_Pt_For_Additional_Constraint()

//////////

void Fitter_Performance::Compare_Dijet_Mass_For_Additional_Constraint()
{
  TCanvas can_mass("can_mass", "", 1000, 1400);
  can_mass.Divide(2, 5);
  
  TH1D* h_mass[N_SAMPLE][3];
  THStack* h_stack[N_SAMPLE];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 3b-tagged events"; 
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-1].Atof();
	  h_name += " GeV sample, 3b-tagged events";
	}
      
      h_stack[i] = new THStack(h_name, h_name);
      
      for(Int_t j=0; j<3; j++)
	{
	  h_mass[i][j] = (TH1D*)fin[j][i]->Get("DiJetMass_3B");

	  if(j==0) h_mass[i][j]->SetTitle("No constraint");
	  else if(j==1) h_mass[i][j]->SetTitle("Low mass");
	  else if(j==2) h_mass[i][j]->SetTitle("High mass");

	  if(j==0) h_mass[i][j]->SetLineColor(1);
	  else if(j==1) h_mass[i][j]->SetLineColor(2);
	  else if(j==2) h_mass[i][j]->SetLineColor(4);
	  
	  h_stack[i]->Add(h_mass[i][j]);

	  can_mass.cd(i+1);

	  h_stack[i]->Draw("nostack");
	  h_stack[i]->GetXaxis()->SetTitle("M_{jj} [GeV]");
		  
	  can_mass.GetPad(i+1)->BuildLegend(0.75, 0.75, 0.95, 0.95, "");
	  can_mass.GetPad(i+1)->Modified();
	}
    }

  can_mass.Print("Comp_Mass_B_Pt_Constraint."+extension, extension);

  for(Int_t i=0; i<N_SAMPLE; i++){ delete h_stack[i]; }
  
  return;
}//void Fitter_Performance::Compare_Dijet_Mass_For_Additional_Constraint()

//////////

void Fitter_Performance::Compare_Jet_Match_Prob_For_Additional_Constraint()
{
  TGraph gr_prob[3];
  TH1D* h_origin_match[3][N_SIGNAL];
  for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<N_SIGNAL; j++)
	{
	  h_origin_match[i][j] = (TH1D*)fin[i][j+1]->Get("Jet_Permutation_Match_3B");

	  Double_t prob = h_origin_match[i][j]->GetMean();

	  gr_prob[i].SetPoint(j, signal_mass[j].Atof(), prob);
	}
    }

  TCanvas can_gr_prob("can_gr_prob", "can_gr_prob", 800, 500);
  can_gr_prob.SetGridy();
  can_gr_prob.Draw();
  
  gr_prob[0].SetTitle("Jet origin match Prob. w/ and wo/ the additional constraint on b-tagged jets");
  gr_prob[0].GetXaxis()->SetTitle("h^{#pm} signal mass [GeV]");
  gr_prob[0].GetYaxis()->SetRangeUser(0.0, 0.7);
  gr_prob[0].GetYaxis()->SetTitle("Jet origin match Prob.");
  gr_prob[0].GetYaxis()->SetTitleOffset(1.2);
  gr_prob[0].SetLineColor(1);
  gr_prob[0].SetMarkerColor(1);
  gr_prob[0].Draw("AC*");

  gr_prob[1].SetLineColor(2);
  gr_prob[1].SetMarkerColor(2);
  gr_prob[1].Draw("sameC*");

  gr_prob[2].SetLineColor(4);
  gr_prob[2].SetMarkerColor(4);
  gr_prob[2].Draw("sameC*");

  TLegend tl(0.10, 0.70, 0.35, 0.90);
  tl.AddEntry(&gr_prob[0], "No constraint", "pl");
  tl.AddEntry(&gr_prob[1], "Low mass", "pl");
  tl.AddEntry(&gr_prob[2], "High mass", "pl");
  tl.Draw("same");
    
  can_gr_prob.Print("Comp_Match_Prob_B_Pt_Constraint."+extension, extension);
  
  return;
}//void Fitter_Performance::Compare_Jet_Match_Prob_For_Additional_Constraint()

//////////

void Fitter_Performance::Investigation_Fitter_Failure_2B()
{
  TGraph gr_reason_chi2_2b;
  TGraph gr_reason_bjet_config_2b;
  TGraph gr_gereuk_2b;
  
  TH1D* h_reason_chi2_2b[N_SAMPLE];
  TH1D* h_prob_2b[N_SAMPLE];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      TFile* fin_target = fin[0][i];
      
      h_reason_chi2_2b[i] = (TH1D*)fin_target->Get("Jet_Permutation_Match_Fail_Reason_2B");
      h_prob_2b[i] = (TH1D*)fin_target->Get("Jet_Permutation_Match_2B");
      
      Double_t gereuk_2b = (1-h_prob_2b[i]->GetMean())*h_reason_chi2_2b[i]->GetMean();

      if(i==0)
	{
	  cout << "Investigation_Fitter_Failure_2B"<< endl;
	  cout << endl;
	  cout << "2B event" << endl;
	  cout << "fail due to b-tagged jet config = " <<  1-h_reason_chi2_2b[i]->GetMean() << endl;
	  cout << "fail due to chi2 = " << h_reason_chi2_2b[i]->GetMean() << endl;
	  cout << "chicken ribs = " << gereuk_2b << endl;
	}
      else
	{
	  gr_reason_chi2_2b.SetPoint(gr_reason_chi2_2b.GetN(), signal_mass[i-1].Atof(), h_reason_chi2_2b[i]->GetMean());
	  gr_reason_bjet_config_2b.SetPoint(gr_reason_bjet_config_2b.GetN(), signal_mass[i-1].Atof(), 1-h_reason_chi2_2b[i]->GetMean());
	  gr_gereuk_2b.SetPoint(gr_gereuk_2b.GetN(), signal_mass[i-1].Atof(), gereuk_2b);	  
	}
    }
  
  //draw 2b events
  TCanvas can_2b("can_2b", "can_2b", 800, 500);
  can_2b.Draw();

  gr_reason_chi2_2b.SetTitle("Reason of fitter failure, space to improve the fitter 2B");
  gr_reason_chi2_2b.GetXaxis()->SetTitle("h^{#pm} signal mass [GeV}");
  gr_reason_chi2_2b.GetYaxis()->SetTitle("Fraction");
  gr_reason_chi2_2b.GetYaxis()->SetRangeUser(0, 1);
  
  gr_reason_chi2_2b.SetMarkerStyle(29);
  gr_reason_chi2_2b.SetMarkerColor(4);
  gr_reason_chi2_2b.SetLineColor(4);
  gr_reason_chi2_2b.Draw();
  
  gr_reason_bjet_config_2b.SetMarkerStyle(34);
  gr_reason_bjet_config_2b.SetMarkerColor(6);
  gr_reason_bjet_config_2b.SetLineColor(6);
  gr_reason_bjet_config_2b.Draw("samepl");

  gr_gereuk_2b.SetMarkerStyle(21);
  gr_gereuk_2b.SetMarkerColor(8);
  gr_gereuk_2b.SetLineColor(8);
  gr_gereuk_2b.Draw("samepl");
  
  TLegend tl_2b(0.15, 0.55, 0.35, 0.7);
  tl_2b.AddEntry(&gr_reason_bjet_config_2b, "Frac_{b-jet Config.}", "lp");
  tl_2b.AddEntry(&gr_reason_chi2_2b, "Frac_{#chi^{2} Eval.}", "lp");
  tl_2b.AddEntry(&gr_gereuk_2b, "Chicken ribs", "lp");
  tl_2b.Draw("same");
  
  can_2b.Print("Fitter_Fail_Reason_2B."+extension, extension);
 
  return;
}//void Fitter_Performance::Investigation_Fitter_Failure_2B()

//////////

void Fitter_Performance::Investigation_Fitter_Failure_3B()
{
  TGraph gr_reason_chi2_3b;
  TGraph gr_reason_bjet_config_3b;
  TGraph gr_gereuk_3b;

  TH1D* h_reason_chi2_3b[N_SAMPLE+1];
  TH1D* h_prob_3b[N_SAMPLE+1];
  for(Int_t i=0; i<N_SAMPLE+1; i++)
    {
      TFile* fin_target = Get_Target_File(i);

      h_reason_chi2_3b[i] = (TH1D*)fin_target->Get("Jet_Permutation_Match_Fail_Reason_3B");
      h_prob_3b[i] = (TH1D*)fin_target->Get("Jet_Permutation_Match_3B");

      Double_t gereuk_3b = (1-h_prob_3b[i]->GetMean())*h_reason_chi2_3b[i]->GetMean();
      
      if(i==0)
	{
	  cout << "Investigation_Fitter_Failure_3B" << endl;
	  cout << endl;
	  cout << "3B event low fitter" << endl;
	  cout << "fail due to b-tagged jet config = " <<  1-h_reason_chi2_3b[i]->GetMean() << endl;
	  cout << "fail due to chi2 = " << h_reason_chi2_3b[i]->GetMean() << endl;
	  cout << "chicken ribs = " << gereuk_3b << endl;
	}
      else if(i==1)
	{
	  cout << endl;
	  cout << "3B event high fitter" << endl;
	  cout << "fail due to b-tagged jet config = "<< 1-h_reason_chi2_3b[i]->GetMean() << endl;
	  cout << "fail due to chi2 = " << h_reason_chi2_3b[i]->GetMean() << endl;
	  cout << "chicken ribs = " << gereuk_3b << endl;
	}
      else
	{
	  gr_reason_bjet_config_3b.SetPoint(gr_reason_bjet_config_3b.GetN(), signal_mass[i-2].Atof(), 1-h_reason_chi2_3b[i]->GetMean());
	  gr_reason_chi2_3b.SetPoint(gr_reason_chi2_3b.GetN(), signal_mass[i-2].Atof(), h_reason_chi2_3b[i]->GetMean());
	  gr_gereuk_3b.SetPoint(gr_gereuk_3b.GetN(), signal_mass[i-2].Atof(), gereuk_3b);
	}
    }
  
  TCanvas can_3b("can_3b", "can_3b", 800, 500);
  can_3b.Draw();

  gr_reason_chi2_3b.SetTitle("Reason of fitter failure, space to improve the fitter 3B");
  gr_reason_chi2_3b.GetXaxis()->SetTitle("h^{#pm} signal mass [GeV}");
  gr_reason_chi2_3b.GetYaxis()->SetTitle("Fraction");
  gr_reason_chi2_3b.GetYaxis()->SetRangeUser(0, 1);
  
  gr_reason_chi2_3b.SetMarkerStyle(29);
  gr_reason_chi2_3b.SetMarkerColor(4);
  gr_reason_chi2_3b.SetLineColor(4);
  gr_reason_chi2_3b.Draw();

  gr_reason_bjet_config_3b.SetMarkerStyle(34);
  gr_reason_bjet_config_3b.SetMarkerColor(6);
  gr_reason_bjet_config_3b.SetLineColor(6);
  gr_reason_bjet_config_3b.Draw("samepl");

  gr_gereuk_3b.SetMarkerStyle(21);
  gr_gereuk_3b.SetMarkerColor(8);
  gr_gereuk_3b.SetLineColor(8);
  gr_gereuk_3b.Draw("samepl");

  TLegend tl_3b(0.15, 0.55, 0.35, 0.7);
  tl_3b.AddEntry(&gr_reason_bjet_config_3b, "Frac_{b-jet Config.}", "lp");
  tl_3b.AddEntry(&gr_reason_chi2_3b, "Frac_{#chi^{2} Eval.}", "lp");
  tl_3b.AddEntry(&gr_gereuk_3b, "Chicken ribs", "lp");
  tl_3b.Draw("same");
    
  can_3b.Print("Fitter_Fail_Reason_3B."+extension, extension);
  
  return;
}//void Fitter_Performance::Investigation_Fitter_Failure_3B()

//////////

void Fitter_Performance::Reconstruction_Result_Chi2_2B()
{
  TCanvas can_chi2_2b("can_chi2_2b", "can_chi2_2b", 1000, 1400);
  can_chi2_2b.Divide(2, 5);
  can_chi2_2b.Draw();

  TH1D* h_chi2_2b[N_SAMPLE];
  TH1D* h_chi2_2b_ofs[N_SAMPLE];
  TH1D* h_chi2_2b_off[N_SAMPLE];
  TPaveStats* stats_chi2_2b_ofs[N_SAMPLE];
  TPaveStats* stats_chi2_2b_off[N_SAMPLE];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      can_chi2_2b.GetPad(i+1)->SetLogy();

      h_chi2_2b[i] = (TH1D*)fin[0][i]->Get("Chi2_2B");
      h_chi2_2b_ofs[i] = (TH1D*)fin[0][i]->Get("Chi2_2B_OFS");
      h_chi2_2b_off[i] = (TH1D*)fin[0][i]->Get("Chi2_2B_OFF");

      h_chi2_2b_ofs[i]->SetLineColor(2);
      h_chi2_2b_off[i]->SetLineColor(8);

      h_chi2_2b_ofs[i]->SetName("Origin finding succeed");
      h_chi2_2b_off[i]->SetName("Origin finding fail");
      
      can_chi2_2b.cd(i+1);

      h_chi2_2b_ofs[i]->Draw();
      can_chi2_2b.GetPad(i+1)->Update();
      stats_chi2_2b_ofs[i] = (TPaveStats*)h_chi2_2b_ofs[i]->FindObject("stats");

      h_chi2_2b_off[i]->Draw();
      can_chi2_2b.GetPad(i+1)->Update();
      stats_chi2_2b_off[i] = (TPaveStats*)h_chi2_2b_off[i]->FindObject("stats");

      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 2b-tagged events";
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-1].Atof();
	  h_name += " GeV sample, 2b-tagged events";
	}
      
      h_chi2_2b[i]->SetTitle(h_name);
      h_chi2_2b[i]->GetXaxis()->SetTitle("#chi^{2}");
      h_chi2_2b[i]->SetStats(kFALSE);
      h_chi2_2b[i]->Draw();

      h_chi2_2b_ofs[i]->Draw("same");
      h_chi2_2b_off[i]->Draw("same");

      stats_chi2_2b_ofs[i]->SetY2NDC(0.90);
      stats_chi2_2b_ofs[i]->SetY1NDC(0.70);
      stats_chi2_2b_ofs[i]->Draw("same");

      stats_chi2_2b_off[i]->SetY2NDC(0.65);
      stats_chi2_2b_off[i]->SetY1NDC(0.45);
      stats_chi2_2b_off[i]->Draw("same");
    }

  can_chi2_2b.Print("Chi2_OFS_OFF_Sep_2B."+extension, extension);
  
  return;
}//void Fitter_Performance::Reconstruction_Result_Chi2_2B()

//////////

void Fitter_Performance::Reconstruction_Result_Chi2_3B()
{
  TCanvas can_chi2_3b("can_chi2_2b", "can_chi2_2b", 1000, 1400);
  can_chi2_3b.Divide(2, 5);
  can_chi2_3b.Draw();

  TH1D* h_chi2_3b[N_SAMPLE+1];
  TH1D* h_chi2_3b_ofs[N_SAMPLE+1];
  TH1D* h_chi2_3b_off[N_SAMPLE+1];
  TPaveStats* stats_chi2_3b_ofs[N_SAMPLE+1];
  TPaveStats* stats_chi2_3b_off[N_SAMPLE+1];
  for(Int_t i=0; i<N_SAMPLE+1; i++)
    {
      can_chi2_3b.GetPad(i+1)->SetLogy();

      TFile* fin_target = Get_Target_File(i);
      
      h_chi2_3b[i] = (TH1D*)fin_target->Get("Chi2_3B");
      h_chi2_3b_ofs[i] = (TH1D*)fin_target->Get("Chi2_3B_OFS");
      h_chi2_3b_off[i] = (TH1D*)fin_target->Get("Chi2_3B_OFF");

      h_chi2_3b_ofs[i]->SetLineColor(2);
      h_chi2_3b_off[i]->SetLineColor(8);

      h_chi2_3b_ofs[i]->SetName("Origin finding succeed");
      h_chi2_3b_off[i]->SetName("Origin finding fail");

      can_chi2_3b.cd(i+1);

      h_chi2_3b_ofs[i]->Draw();
      can_chi2_3b.GetPad(i+1)->Update();
      stats_chi2_3b_ofs[i] = (TPaveStats*)h_chi2_3b_ofs[i]->FindObject("stats");

      h_chi2_3b_off[i]->Draw();
      can_chi2_3b.GetPad(i+1)->Update();
      stats_chi2_3b_off[i] = (TPaveStats*)h_chi2_3b_off[i]->FindObject("stats");

      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 3b-tagged events, low mass fitter";
      else if(i==1) h_name = "SM t#bar{t} sample, 3b-tagged events, high mass fitter";
      else if(i<5)
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-2].Atof();
	  h_name += " GeV sample, 3b-tagged events, low mass fitter";
	}
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-2].Atof();
	  h_name += " GeV sample, 3b-tagged events, high mass fitter";
	}
      
      h_chi2_3b[i]->SetTitle(h_name);
      h_chi2_3b[i]->GetXaxis()->SetTitle("M_{jj} [GeV]");
      h_chi2_3b[i]->SetStats(kFALSE);
      h_chi2_3b[i]->Draw();

      h_chi2_3b_ofs[i]->Draw("same");
      h_chi2_3b_off[i]->Draw("same");

      stats_chi2_3b_ofs[i]->SetY2NDC(0.90);
      stats_chi2_3b_ofs[i]->SetY1NDC(0.70);
      stats_chi2_3b_ofs[i]->Draw("same");

      stats_chi2_3b_off[i]->SetY2NDC(0.65);
      stats_chi2_3b_off[i]->SetY1NDC(0.45);
      stats_chi2_3b_off[i]->Draw("same");
    }

  can_chi2_3b.Print("Chi2_OFS_OFF_Sep_3B."+extension, extension);

  return;
}//void Fitter_Performance::Reconstruction_Result_Chi2_3B()

//////////

void Fitter_Performance::Reconstruction_Result_Chi2_Piece_2B()
{
  TCanvas* can_piece_2b;

  TH1D* h_chi2_2b_piece[N_SAMPLE][N_CHI2_PIECE];
  TH1D* h_chi2_2b_piece_ofs[N_SAMPLE][N_CHI2_PIECE];
  TH1D* h_chi2_2b_piece_off[N_SAMPLE][N_CHI2_PIECE];
  TPaveStats* stats_chi2_2b_piece_ofs[N_SAMPLE][N_CHI2_PIECE];
  TPaveStats* stats_chi2_2b_piece_off[N_SAMPLE][N_CHI2_PIECE];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      can_piece_2b = new TCanvas("can_piece_2b", "can_piece_2b", 1000, 1200);
      can_piece_2b->Divide(2, 5);

      for(Int_t j=0; j<N_CHI2_PIECE; j++)
	{
	  can_piece_2b->GetPad(j+1)->SetLogy();

	  TString h_name = "Chi2_Piece_2B_" + to_string(j);

	  h_chi2_2b_piece[i][j] = (TH1D*)fin[0][i]->Get(h_name);
	  h_chi2_2b_piece_ofs[i][j] = (TH1D*)fin[0][i]->Get(h_name+"_OFS");
	  h_chi2_2b_piece_off[i][j] = (TH1D*)fin[0][i]->Get(h_name+"_OFF");

	  h_chi2_2b_piece_ofs[i][j]->SetName("Origin finding succeed");
	  h_chi2_2b_piece_off[i][j]->SetName("Origin finding fail");
	  
	  h_chi2_2b_piece_ofs[i][j]->SetLineColor(2);
	  h_chi2_2b_piece_off[i][j]->SetLineColor(8);
	  
	  can_piece_2b->cd(j+1);
	  
	  h_chi2_2b_piece_ofs[i][j]->Draw();
	  can_piece_2b->GetPad(j+1)->Update();
	  stats_chi2_2b_piece_ofs[i][j] = (TPaveStats*)h_chi2_2b_piece_ofs[i][j]->FindObject("stats");
	  
	  h_chi2_2b_piece_off[i][j]->Draw();
	  can_piece_2b->GetPad(j+1)->Update();
	  stats_chi2_2b_piece_off[i][j] = (TPaveStats*)h_chi2_2b_piece_off[i][j]->FindObject("stats");

	  if(j<4) h_name = "#chi^{2} from P_{T} of Jet" + to_string(j);
	  else if(j==4) h_name = "#chi^{2} from P_{T} of lepton";
	  else if(j==5) h_name = "#chi^{2} from x component of extra";
	  else if(j==6) h_name = "#chi^{2} from y component of extra";
	  else if(j==7) h_name = "#chi^{2} from leptonic W";
	  else if(j==8) h_name = "#chi^{2} from leptonic t";
	  else if(j==9) h_name = "#chi^{2} from hadronic t";
	  h_name += " 2B";
	  
	  h_chi2_2b_piece[i][j]->SetTitle(h_name);
	  h_chi2_2b_piece[i][j]->GetXaxis()->SetTitle("#chi^{2}");
	  
	  h_chi2_2b_piece[i][j]->Draw();
	  h_chi2_2b_piece_ofs[i][j]->Draw("same");
	  h_chi2_2b_piece_off[i][j]->Draw("same");

	  stats_chi2_2b_piece_ofs[i][j]->SetY2NDC(0.95);
	  stats_chi2_2b_piece_ofs[i][j]->SetY1NDC(0.75);
	  stats_chi2_2b_piece_ofs[i][j]->Draw("same");

	  stats_chi2_2b_piece_off[i][j]->SetY2NDC(0.7);
	  stats_chi2_2b_piece_off[i][j]->SetY1NDC(0.50);
	  stats_chi2_2b_piece_off[i][j]->Draw("same");
	}//N_CHI2_PIECE
      
      TString name = "Chi2_Piece_";
      if(i==0) name += "SM_TT";
      else name += "CH_" + signal_mass[i-1];
      name += "_OFS_OFF_Sep_2B.";
      
      can_piece_2b->Print(name+extension, extension);
      
      delete can_piece_2b;
    }//N_SAMPLE
  
  return;
}//void Fitter_Performance::Reconstruction_Result_Chi2_Piece_2B()

//////////

void Fitter_Performance::Reconstruction_Result_Chi2_Piece_3B()
{
  TCanvas* can_piece_3b;

  for(Int_t i=0; i<N_SAMPLE+1; i++)
    {
      can_piece_3b = new TCanvas("can_piece_2b", "can_piece_2b", 1000, 1200);
      can_piece_3b->Divide(2, 5);

      TFile* fin_target = Get_Target_File(i);
      
      TH1D* h_chi2_3b_piece[N_SAMPLE+1][N_CHI2_PIECE];
      TH1D* h_chi2_3b_piece_ofs[N_SAMPLE+1][N_CHI2_PIECE];
      TH1D* h_chi2_3b_piece_off[N_SAMPLE+1][N_CHI2_PIECE];
      TPaveStats* stats_chi2_3b_piece_ofs[N_SAMPLE+1][N_CHI2_PIECE];
      TPaveStats* stats_chi2_3b_piece_off[N_SAMPLE+1][N_CHI2_PIECE];
      for(Int_t j=0; j<N_CHI2_PIECE; j++)
	{
	  can_piece_3b->GetPad(j+1)->SetLogy();

	  TString h_name = "Chi2_Piece_3B_" + to_string(j);
	  
	  h_chi2_3b_piece[i][j] = (TH1D*)fin_target->Get(h_name);
	  h_chi2_3b_piece_ofs[i][j] = (TH1D*)fin_target->Get(h_name+"_OFS");
	  h_chi2_3b_piece_off[i][j] = (TH1D*)fin_target->Get(h_name+"_OFF");

	  h_chi2_3b_piece_ofs[i][j]->SetLineColor(2);
	  h_chi2_3b_piece_off[i][j]->SetLineColor(8);

	  can_piece_3b->cd(j+1);

	  h_chi2_3b_piece_ofs[i][j]->Draw();
	  can_piece_3b->GetPad(j+1)->Update();
	  stats_chi2_3b_piece_ofs[i][j] = (TPaveStats*)h_chi2_3b_piece_ofs[i][j]->FindObject("stats");

	  h_chi2_3b_piece_off[i][j]->Draw();
	  can_piece_3b->GetPad(j+1)->Update();
	  stats_chi2_3b_piece_off[i][j] = (TPaveStats*)h_chi2_3b_piece_off[i][j]->FindObject("stats");

	  if(j<4) h_name = "#chi^{2} from P_{T} of Jet" + to_string(j);
	  else if(j==4) h_name = "#chi^{2} from P_{T} of lepton";
	  else if(j==5) h_name = "#chi^{2} from x component of extra";
	  else if(j==6) h_name = "#chi^{2} from y component of extra";
	  else if(j==7) h_name = "#chi^{2} from leptonic W";
	  else if(j==8) h_name = "#chi^{2} from leptonic t";
	  else if(j==9) h_name = "#chi^{2} from hadronic t";
	  h_name += " 3B";
	  
	  h_chi2_3b_piece[i][j]->SetTitle(h_name);
	  h_chi2_3b_piece[i][j]->GetXaxis()->SetTitle("#chi^{2}");
	  
	  h_chi2_3b_piece[i][j]->Draw();
	  h_chi2_3b_piece_ofs[i][j]->Draw("same");
	  h_chi2_3b_piece_off[i][j]->Draw("same");

	  stats_chi2_3b_piece_ofs[i][j]->SetY2NDC(0.95);
	  stats_chi2_3b_piece_ofs[i][j]->SetY1NDC(0.75);
	  stats_chi2_3b_piece_ofs[i][j]->Draw("same");

	  stats_chi2_3b_piece_off[i][j]->SetY2NDC(0.7);
	  stats_chi2_3b_piece_off[i][j]->SetY1NDC(0.50);
	  stats_chi2_3b_piece_off[i][j]->Draw("same");
	}
      
      TString name = "Chi2_Piece_";
      if(i==0) name += "SM_TT_Low_Mass";
      else if(i==1) name += "SM_TT_High_Mass";
      else name += "CH_" + signal_mass[i-2];
      name += "_OFS_OFF_Sep_3B.";

      can_piece_3b->Print(name+extension, extension);

      delete can_piece_3b;
    }//N_SAMPLE+1
  return;
}//void Fitter_Performance::Reconstruction_Result_Chi2_Piece_3B()

//////////

void Fitter_Performance::Reconstruction_Result_Neutrino()
{
  TCanvas* can;
  for(Int_t i=0; i<N_SAMPLE+1; i++)
    {
      can = new TCanvas("can", "can", 1600, 1000);
      can->Divide(2, 2);
      
      TFile* fin_target = Get_Target_File(i);

      can->cd(1);
      TH2D* h_met_truth_measured = (TH2D*)fin_target->Get("MET_Truth_Vs_Measured");
      h_met_truth_measured->SetTitle("Comparison with truth and measured neutrino P_{T}"); 
      h_met_truth_measured->GetXaxis()->SetTitle("#nu_{P_{T}}(Truth) [GeV]");
      h_met_truth_measured->GetXaxis()->SetTitleOffset(1.2);
      h_met_truth_measured->GetYaxis()->SetTitle("#slash{E}_{T}(Measured)-#nu_{P_{T}}(Truth) [GeV]");
      h_met_truth_measured->GetYaxis()->SetTitleOffset(1.2);
      h_met_truth_measured->Draw("COLZ");

      can->GetPad(1)->Update();
      TPaveStats* ps_met_truth_measured = (TPaveStats*)h_met_truth_measured->FindObject("stats");
      ps_met_truth_measured->SetOptStat(1110);
      ps_met_truth_measured->SetX1NDC(0.7);
      ps_met_truth_measured->SetX2NDC(0.9);
      ps_met_truth_measured->SetY1NDC(0.7);
      ps_met_truth_measured->SetY2NDC(0.9);
      
      can->cd(2);
      TH2D* h_met_truth_fitted = (TH2D*)fin_target->Get("MET_Truth_Vs_Fitted");
      h_met_truth_fitted->SetTitle("Comparison with truth and fitted neutrino P_{T}");
      h_met_truth_fitted->GetXaxis()->SetTitle("#nu_{P_{T}}(Truth) [GeV]");
      h_met_truth_fitted->GetXaxis()->SetTitleOffset(1.2);
      h_met_truth_fitted->GetYaxis()->SetTitle("#slash{E}_{T}(Fitted)-#nu_{P_{T}}(Truth) [GeV]");
      h_met_truth_fitted->GetYaxis()->SetTitleOffset(1.2);
      h_met_truth_fitted->Draw("COLZ");

      can->GetPad(2)->Update();
      TPaveStats* ps_met_truth_fitted = (TPaveStats*)h_met_truth_fitted->FindObject("stats");
      ps_met_truth_fitted->SetOptStat(1110);
      ps_met_truth_fitted->SetX1NDC(0.7);
      ps_met_truth_fitted->SetX2NDC(0.9);
      ps_met_truth_fitted->SetY1NDC(0.7);
      ps_met_truth_fitted->SetY2NDC(0.9);
            
      can->cd(4);
      TH2D* h_pz_truth_fitted = (TH2D*)fin_target->Get("Pz_Truth_Vs_Fitted");
      h_pz_truth_fitted->SetTitle("Comparison with truth and fitted neutrino P_{z}");
      h_pz_truth_fitted->GetXaxis()->SetTitle("#nu_{P_{z}}(Truth) [GeV]");
      h_pz_truth_fitted->GetYaxis()->SetTitleOffset(1.2);
      h_pz_truth_fitted->GetYaxis()->SetTitle("#nu_{P_{z}}(Fitted)-#nu_{P_{z}}(Truth) [GeV]");
      h_pz_truth_fitted->GetYaxis()->SetTitleOffset(1.2);
      h_pz_truth_fitted->Draw("COLZ");

      can->GetPad(4)->Update();
      TPaveStats* ps_pz_truth_fitted = (TPaveStats*)h_pz_truth_fitted->FindObject("stats");
      ps_pz_truth_fitted->SetOptStat(1110);
      ps_pz_truth_fitted->SetX1NDC(0.7);
      ps_pz_truth_fitted->SetX2NDC(0.9);
      ps_pz_truth_fitted->SetY1NDC(0.7);
      ps_pz_truth_fitted->SetY2NDC(0.9);      
      
      TString can_name = "Neutrino_";
      if(i==0) can_name += "SM_TT_Low_Mass";
      else if(i==1) can_name += "SM_TT_High_Mass";
      else
	{
	  can_name += "CH_";
	  can_name += signal_mass[i-2];
	}

      can->Print(can_name+"."+extension, extension);

      delete can;
    }

  //compare neutrino p_z origin find succeed and fail
  for(Int_t i=0; i<N_SAMPLE+1; i++)
    {
      can = new TCanvas("can", "can", 1600, 500);
      can->Divide(2, 1);

      TFile* fin_target = Get_Target_File(i);
      
      can->cd(1);
      TH2D* h_pz_truth_fitted_ofs = (TH2D*)fin_target->Get("Pz_Truth_Vs_Fitted_OFS");
      h_pz_truth_fitted_ofs->SetTitle("Comparison with truth and fitted neutrino P_{z}, b");
      h_pz_truth_fitted_ofs->GetXaxis()->SetTitle("#nu_{P_{z}}(Truth) [GeV]");
      h_pz_truth_fitted_ofs->GetYaxis()->SetTitleOffset(1.2);
      h_pz_truth_fitted_ofs->GetYaxis()->SetTitle("#nu_{P_{z}}(Fitted)-#nu_{P_{z}}(Truth) [GeV]");
      h_pz_truth_fitted_ofs->GetYaxis()->SetTitleOffset(1.2);
      h_pz_truth_fitted_ofs->Draw("COLZ");

      can->GetPad(1)->Update();
      TPaveStats* ps_pz_truth_fitted_ofs = (TPaveStats*)h_pz_truth_fitted_ofs->FindObject("stats");
      ps_pz_truth_fitted_ofs->SetOptStat(1110);
      ps_pz_truth_fitted_ofs->SetX1NDC(0.7);
      ps_pz_truth_fitted_ofs->SetX2NDC(0.9);
      ps_pz_truth_fitted_ofs->SetY1NDC(0.7);
      ps_pz_truth_fitted_ofs->SetY2NDC(0.9);

      can->cd(2);
      TH2D* h_pz_truth_fitted_off = (TH2D*)fin_target->Get("Pz_Truth_Vs_Fitted_OFF");
      h_pz_truth_fitted_off->SetTitle("Comparison with truth and fitted neutrino P_{z}");
      h_pz_truth_fitted_off->GetXaxis()->SetTitle("#nu_{P_{z}}(Truth) [GeV]");
      h_pz_truth_fitted_off->GetYaxis()->SetTitleOffset(1.2);
      h_pz_truth_fitted_off->GetYaxis()->SetTitle("#nu_{P_{z}}(Fitted)-#nu_{P_{z}}(Truth) [GeV]");
      h_pz_truth_fitted_off->GetYaxis()->SetTitleOffset(1.2);
      h_pz_truth_fitted_off->Draw("COLZ");

      can->GetPad(2)->Update();
      TPaveStats* ps_pz_truth_fitted_off = (TPaveStats*)h_pz_truth_fitted_off->FindObject("stats");
      ps_pz_truth_fitted_off->SetOptStat(1110);
      ps_pz_truth_fitted_off->SetX1NDC(0.7);
      ps_pz_truth_fitted_off->SetX2NDC(0.9);
      ps_pz_truth_fitted_off->SetY1NDC(0.7);
      ps_pz_truth_fitted_off->SetY2NDC(0.9);

      TString can_name = "Neutrino_Pz_";
      if(i==0) can_name += "SM_TT_Low_Mass";
      else if(i==1) can_name += "SM_TT_High_Mass";
      else
	{
	  can_name += "CH_";
	  can_name += signal_mass[i-2];
	}
      can_name += "_OFS_OFF_Sep";

      can->Print(can_name+"."+extension, extension);
      
      delete can;
    }
  
  return;
}//void Fitter_Performance::Reconstruction_Result_Neutrino()

//////////

void Fitter_Performance::Reconstruction_Result_Mass_2B()
{
  TCanvas can_mass_2b("can_mass_2b", "can_mass_2b", 1000, 1400);
  can_mass_2b.Divide(2, 5);
  can_mass_2b.Draw();

  TH1D* h_mass_2b[N_SAMPLE];
  TH1D* h_mass_2b_ofs[N_SAMPLE];
  TH1D* h_mass_2b_off[N_SAMPLE];
  TPaveStats* stats_mass_2b_ofs[N_SAMPLE];
  TPaveStats* stats_mass_2b_off[N_SAMPLE];
  for(Int_t i=0; i<N_SAMPLE; i++)
    {
      h_mass_2b[i] = (TH1D*)fin[0][i]->Get("DiJetMass_2B");
      h_mass_2b_ofs[i] = (TH1D*)fin[0][i]->Get("DiJetMass_2B_OFS");
      h_mass_2b_off[i] = (TH1D*)fin[0][i]->Get("DiJetMass_2B_OFF");
            
      h_mass_2b_ofs[i]->SetLineColor(2);
      h_mass_2b_off[i]->SetLineColor(8);

      h_mass_2b_ofs[i]->SetName("Origin finding succeed");
      h_mass_2b_off[i]->SetName("Origin finding fail");
      
      can_mass_2b.cd(i+1);

      h_mass_2b_ofs[i]->Draw();
      can_mass_2b.GetPad(i+1)->Update();
      stats_mass_2b_ofs[i] = (TPaveStats*)h_mass_2b_ofs[i]->FindObject("stats");

      h_mass_2b_off[i]->Draw();
      can_mass_2b.GetPad(i+1)->Update();
      stats_mass_2b_off[i] = (TPaveStats*)h_mass_2b_off[i]->FindObject("stats");

      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 2b-tagged events";
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-1].Atof();
	  h_name += " GeV sample, 2b-tagged events";
	}
      h_mass_2b[i]->SetTitle(h_name);
      h_mass_2b[i]->GetXaxis()->SetTitle("M_{jj} [GeV]");
      h_mass_2b[i]->SetStats(kFALSE);
      h_mass_2b[i]->Draw();

      h_mass_2b_ofs[i]->Draw("same");
      h_mass_2b_off[i]->Draw("same");

      stats_mass_2b_ofs[i]->SetY2NDC(0.90);
      stats_mass_2b_ofs[i]->SetY1NDC(0.70);
      stats_mass_2b_ofs[i]->Draw("same");

      stats_mass_2b_off[i]->SetY2NDC(0.65);
      stats_mass_2b_off[i]->SetY1NDC(0.45);
      stats_mass_2b_off[i]->Draw("same");
    }//loop over n_sample
  
  can_mass_2b.Print("Mass_OFS_OFF_Sep_2B."+extension, extension);

  return;
}//void Fitter_Performance::Reconstruction_Result_Mass_2B()

//////////

void Fitter_Performance::Reconstruction_Result_Mass_3B()
{
  TCanvas can_mass_3b("can_mass_3b", "can_mass_3b", 1000, 1400);
  can_mass_3b.Divide(2, 5);
  can_mass_3b.Draw();

  TH1D* h_mass_3b[N_SAMPLE+1];
  TH1D* h_mass_3b_ofs[N_SAMPLE+1];
  TH1D* h_mass_3b_off[N_SAMPLE+1];
  TPaveStats* stats_mass_3b_ofs[N_SAMPLE+1];
  TPaveStats* stats_mass_3b_off[N_SAMPLE+1];
  for(Int_t i=0; i<N_SAMPLE+1; i++)
    {
      TFile* fin_target = Get_Target_File(i);
           
      h_mass_3b[i] = (TH1D*)fin_target->Get("DiJetMass_3B");
      h_mass_3b_ofs[i] = (TH1D*)fin_target->Get("DiJetMass_3B_OFS");
      h_mass_3b_off[i] = (TH1D*)fin_target->Get("DiJetMass_3B_OFF");
      
      h_mass_3b_ofs[i]->SetLineColor(2);
      h_mass_3b_off[i]->SetLineColor(8);

      h_mass_3b_ofs[i]->SetName("Origin finding succeed");
      h_mass_3b_off[i]->SetName("Origin finding fail");
            
      can_mass_3b.cd(i+1);

      h_mass_3b_ofs[i]->Draw();
      can_mass_3b.GetPad(i+1)->Update();
      stats_mass_3b_ofs[i] = (TPaveStats*)h_mass_3b_ofs[i]->FindObject("stats");

      h_mass_3b_off[i]->Draw();
      can_mass_3b.GetPad(i+1)->Update();
      stats_mass_3b_off[i] = (TPaveStats*)h_mass_3b_off[i]->FindObject("stats");

      TString h_name;
      if(i==0) h_name = "SM t#bar{t} sample, 3b-tagged events, low mass fitter";
      else if(i==1) h_name = "SM t#bar{t} sample, 3b-tagged events, high mass fitter";
      else if(i<5)
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-2].Atof();
	  h_name += " sample, 3b-tagged events, low mass fitter";
	}
      else
	{
	  h_name = "h^{#pm} ";
	  h_name += signal_mass[i-2].Atof();
	  h_name += " GeV sample, 3b-tagged events, high mass fitter";
	}
      h_mass_3b[i]->SetTitle(h_name);
      h_mass_3b[i]->GetXaxis()->SetTitle("M_{jj} [GeV]");
      h_mass_3b[i]->SetStats(kFALSE);
      h_mass_3b[i]->Draw();

      h_mass_3b_ofs[i]->Draw("same");
      h_mass_3b_off[i]->Draw("same");

      stats_mass_3b_ofs[i]->SetY2NDC(0.90);
      stats_mass_3b_ofs[i]->SetY1NDC(0.70);
      stats_mass_3b_ofs[i]->Draw("same");

      stats_mass_3b_off[i]->SetY2NDC(0.65);
      stats_mass_3b_off[i]->SetY1NDC(0.45);
      stats_mass_3b_off[i]->Draw("same");
      
    }
  can_mass_3b.Print("Mass_OFS_OFF_Sep_3B."+extension, extension);
  
  return;
}//void Fitter_Performance::Reconstruction_Result_Mass_3B()

//////////

void Fitter_Performance::Reconstruction_Result_Jet_Origin_Match_Prob()
{
  TGraph gr_prob_2b;
  TGraph gr_prob_3b;
  TH1D* h_prob_2b[N_SIGNAL];
  TH1D* h_prob_3b[N_SIGNAL];
  for(Int_t i=0; i<N_SIGNAL; i++)
    {
      TFile* fin_target = Get_Target_File(i+2);
      
      h_prob_2b[i] = (TH1D*)fin_target->Get("Jet_Permutation_Match_2B");
      h_prob_3b[i] = (TH1D*)fin_target->Get("Jet_Permutation_Match_3B");

      gr_prob_2b.SetPoint(i, signal_mass[i].Atof(), h_prob_2b[i]->GetMean());
      gr_prob_3b.SetPoint(i, signal_mass[i].Atof(), h_prob_3b[i]->GetMean());
    }

  TCanvas can("can", "can", 800, 500);
  can.Draw();
  
  gr_prob_2b.SetTitle("Jet origin match Prob.");
  gr_prob_2b.GetXaxis()->SetTitle("M_{h^{#pm}} [GeV]");
  gr_prob_2b.GetYaxis()->SetTitle("Jet origin match Prob. [%]");
  gr_prob_2b.GetYaxis()->SetTitleOffset(1.2);
  gr_prob_2b.GetYaxis()->SetRangeUser(0, 0.7);
  
  gr_prob_2b.SetMarkerStyle(20);
  gr_prob_2b.SetMarkerColor(1);
  gr_prob_2b.SetLineColor(1);
  gr_prob_2b.Draw();

  gr_prob_3b.SetMarkerStyle(21);
  gr_prob_3b.SetMarkerColor(2);
  gr_prob_3b.SetLineColor(2);
  gr_prob_3b.Draw("samepl");

  TLegend tl(0.1, 0.7, 0.4, 0.9);
  tl.AddEntry(&gr_prob_2b, "Jet origin match Prob. 2B", "lp");
  tl.AddEntry(&gr_prob_3b, "Jet origin match Prob. 3B", "lp");
  tl.Draw("same");
  
  can.Print("Jet_Origin_Match_Prob."+extension, extension);
  
  return;
}//void Fitter_Performance::Reconstruction_Result_Jet_Origin_Match_Prob()

//////////

TFile* Fitter_Performance::Get_Target_File(const Int_t& index)
{
  if(index==0)      return fin[1][0];//return low mass fitter, tt sample
  else if(index==1) return fin[2][0];//return high mass fitter, tt sample
  else if(index<5)  return fin[1][index-1];//return low mass fitter, charged higgs sample, mass 90, 100, and 110
  else              return fin[2][index-1];//return high mass fitter, charged higgs sample, mass 120, 125, 130, 140, and 150

  return NULL;
}//TFile* Fitter_Performance::Get_Target_File(const Int_t& index)

//////////
