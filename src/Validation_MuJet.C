#include "Validation_MuJet.h"

ClassImp(Validation_MuJet);

//////////

Validation_MuJet::Validation_MuJet() : n_mc_sample(17)
{
  //set mc names
  mc_sample_name = new TString[n_mc_sample];
  mc_sample_name[0] = "SKDYJets";
  mc_sample_name[1] = "SKSingleTbar_t";
  mc_sample_name[2] = "SKSingleTbar_tW";
  mc_sample_name[3] = "SKSingleTbar_tW_noHadron";
  mc_sample_name[4] = "SKSingleTop_s";
  mc_sample_name[5] = "SKSingleTop_t";
  mc_sample_name[6] = "SKSingleTop_tW";
  mc_sample_name[7] = "SKSingleTop_tW_noHadron";
  mc_sample_name[8] = "SKttH_bb";
  mc_sample_name[9] = "SKttH_nonbb";
  mc_sample_name[10] = "SKTT_powheg";
  mc_sample_name[11] = "SKttW";
  mc_sample_name[12] = "SKttZ";
  mc_sample_name[13] = "SKWJets";
  mc_sample_name[14] = "SKWW";
  mc_sample_name[15] = "SKWZ";
  mc_sample_name[16] = "SKZZ";
  
  
  //open mc files and set branch address
  fin_mc = new TFile*[n_mc_sample]; 
  ntuple_mc = new TNtuple*[n_mc_sample];
  for(Int_t i=0; i<n_mc_sample; i++)
    {
      TString fin_name = "./Root_Files/Validation_CH_MuJet_";
      fin_name += mc_sample_name[i];
      fin_name += "_cat_v8-0-7.root";

      fin_mc[i] = new TFile(fin_name);

      SetBranchAddress(fin_mc[i], ntuple_mc[i]);
    } 
    
  //open data file and set branch address
  fin_data = new TFile("./Root_Files/Validation_CH_MuJet_data_SingleMuon_cat_v8-0-7.root");
  SetBranchAddress(fin_data, ntuple_data);
      
  fout = new TFile("Validation_MuJet_Results.root", "RECREATE");
}//Validation_MuJet::Validation_MuJet()

//////////

Validation_MuJet::~Validation_MuJet()
{
  delete[] mc_sample_name;

  delete[] ntuple_mc;
  
  for(Int_t i=0; i<n_mc_sample; i++)
    {
      fin_mc[i]->Close();
      delete fin_mc[i];
    }
  delete fin_mc;

  fin_data->Close();
  delete fin_data;
  
  fout->Close();
  delete fout;
}//Validation_MuJet::~Validation_MuJet()

//////////

void Validation_MuJet::Eta()
{
  //make histograms for mc
  TH1D**** histo_mc = NULL;
  Make_Histo_MC(histo_mc, "Eta");
    
  //fill histograms for mc
  for(Int_t i=0; i<n_mc_sample; i++)
    {
      for(Int_t j=0; j<ntuple_mc[i]->GetEntries(); j++)
  	{
	  ntuple_mc[i]->GetEntry(j);

	  if(met<20) continue;
	  if(muon_pt<30) continue; 
	  
  	  Int_t index_b_tag = Index_N_B_Tag();
          
    	  Double_t effective_weight = TOTAL_LUMINOSITY*weight*weight_by_trigger*trigger_sf*pileup_reweight*muon_id_sf*muon_iso_sf*muon_tracking_eff_sf*pileup_reweight*b_tag_reweight;
	  //what dose weight mean?
	  //weight_by_trigger = trigger bias?
	  	  
  	  histo_mc[index_b_tag][0][i]->Fill(muon_eta, effective_weight);
  	  histo_mc[index_b_tag][1][i]->Fill(jet0_eta, effective_weight);
  	  histo_mc[index_b_tag][2][i]->Fill(jet1_eta, effective_weight);
  	}//n_events
    }//n_mc_sample

  //make THStack
  THStack*** stack_mc = NULL;
  Make_Stack_MC(stack_mc, "Eta");
  
  //stack mc histo to THStack
  Stack_MC_Histo(histo_mc, stack_mc);

  //make histograms for data
  TH1D*** histo_data = NULL;
  Make_Histo_Data(histo_data, "Eta");
  
  //fill histogram data
  for(Int_t i=0; i<ntuple_data->GetEntries(); i++)
    {
      ntuple_data->GetEntry(i);

      Int_t index_b_tag = Index_N_B_Tag();
            
      histo_data[index_b_tag][0]->Fill(muon_eta);
      histo_data[index_b_tag][1]->Fill(jet0_eta);
      histo_data[index_b_tag][2]->Fill(jet1_eta);
    }//n_event
  
  //make graph for comparison
  TGraphErrors*** gr_comp = NULL;
  Make_Graph_Comparison(gr_comp, "Eta");

  //compare
  for(Int_t i=0; i<3; i++)
    {
     for(Int_t j=0; j<3; j++)
       {
	 Compare_MC_Data(histo_data[i][j], stack_mc[i][j], gr_comp[i][j]);
       }//n_object
    }//n_b_tag

  //save histograms, stacks
  Write(histo_mc, stack_mc, histo_data, gr_comp);
  
  //delete histograms, stacks
  Clear(histo_mc, stack_mc, histo_data, gr_comp);

  return;
}//void Validatation::Eta()

//////////

void Validation_MuJet::Clear(TH1D****& histo_mc, THStack***& stack_mc, TH1D***& histo_data, TGraphErrors***& gr_comp)
{
  //delete histo_mc
  for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<3; j++)
        {
          for(Int_t k=0; k<n_mc_sample; k++)
	    {
	      delete histo_mc[i][j][k];
	    }//n_mc_sample
	  delete[] histo_mc[i][j];
        }//n_object
      delete[] histo_mc[i];
    }//n_b_tag
  delete histo_mc;

  //delete stack_mc
  for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<3; j++)
	{
	  delete stack_mc[i][j];
	}//n_object
      delete[] stack_mc[i];
    }//n_b_tag
  delete[] stack_mc;
  
  //delete histo_data
  for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<3; j++)
        {
          delete histo_data[i][j];
        }//n_object
      delete[] histo_data[i];
    }//n_b_tag
  delete[] histo_data;

  //delete gr_comp
   for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<3; j++)
        {
	  delete gr_comp[i][j];
	}
      delete[] gr_comp[i];
    }
   delete[] gr_comp;
  
  return;
}//void Validation_MuJet::Clear(TH1D****& histo_mc, THStack***& stack_mc, TH1D***& histo_data)

//////////

void Validation_MuJet::Compare_MC_Data(TH1D* histo_data, THStack* stack_mc, TGraphErrors* gr_comp)
{
  for(Int_t i=0; i<histo_data->GetNbinsX(); i++)
    {
      Double_t content_data = histo_data->GetBinContent(i+1);
      Double_t error_data = histo_data->GetBinError(i+1);
      
      Double_t content_mc = ((TH1*)stack_mc->GetStack()->Last())->GetBinContent(i+1);
      Double_t error_mc = ((TH1*)stack_mc->GetStack()->Last())->GetBinError(i+1);
      
      if(1e-1<content_mc)
	{
	  Double_t bin_center = histo_data->GetBinCenter(i+1);

	  Double_t bin_half_width = histo_data->GetBinWidth(i+1)/2.;

	  Double_t ratio = content_data/content_mc;
	  
	  Double_t ratio_error = TMath::Power(error_data, 2.0)/TMath::Power(content_mc, 2.);
	  ratio_error += TMath::Power(content_data, 2.0)/TMath::Power(content_mc, 4.0)*TMath::Power(error_mc, 2.0);
	  ratio_error = TMath::Sqrt(ratio_error);

	  Int_t n_point = gr_comp->GetN();

	  gr_comp->SetPoint(n_point, bin_center, ratio);
	  gr_comp->SetPointError(n_point, bin_half_width, ratio_error);
	}
    }//bin number
  
  return;
}//void Validation_MuJet::Compare_MC_Data(TH1D* histo_data, TStack* stack_mc, TGraphErrors* gr_comp)

//////////

Int_t Validation_MuJet::Index_N_B_Tag()
{
  Int_t index_b_tag = 0 ;
  
  if(TMath::Abs(n_b_tag-1)<1E-5) index_b_tag = 0;
  else if(TMath::Abs(n_b_tag-2)<1E-5) index_b_tag = 1;
  else if(TMath::Abs(n_b_tag-3)<1E-5) index_b_tag = 2;

  return index_b_tag;
}//Int_t Validation_MuJet::Index_N_B_Tag()

//////////

void Validation_MuJet::Make_Graph_Comparison(TGraphErrors***& gr_comp, const TString& target)
{
  gr_comp = new TGraphErrors**[3];
  for(Int_t i=0; i<3; i++)
    {
      TString b_tag = to_string(i+1) + "B";
      
      gr_comp[i] = new TGraphErrors*[3];
      for(Int_t j=0; j<3; j++)
        {
	  TString object;
          if(j==0) object = "Muon";
          else if(j==1) object = "Jet0";
          else if(j==2) object = "Jet1";
	  
          gr_comp[i][j] = new TGraphErrors();

	  TString gr_name = target + "_" + b_tag + "_" + object + "_Comparision";

	  gr_comp[i][j]->SetTitle(gr_name);
	  gr_comp[i][j]->SetName(gr_name);
        }//n_object
    }//n_b_tag

  return;
}//void Validation_MuJet::Make_Graph_Comparison(TGraphErrors***& gr_comp, const TString& target)

//////////

void Validation_MuJet::Make_Histo_Data(TH1D***& histo_data, const TString& target)
{
  histo_data = new TH1D**[3];
  for(Int_t i=0; i<3; i++)
    {
      TString b_tag = to_string(i+1) + "B";

      histo_data[i] = new TH1D*[3];
      for(Int_t j=0; j<3; j++)
        {
          TString object;
          if(j==0) object = "Muon";
          else if(j==1) object = "Jet0";
          else if(j==2) object = "Jet1";

          TString histo_name = target + "_" + b_tag + "_" + object + "_Data";

          histo_data[i][j] = new TH1D(histo_name, histo_name, 60, -3, 3);
        }//n_object
    }//n_b_tag

  return;
}//void Validation_MuJet::Make_Histo_Data(TH1D***& histo_data, const TString& target)

//////////

void Validation_MuJet::Make_Histo_MC(TH1D****& histo_mc, const TString& target)
{
  histo_mc= new TH1D***[3];
  for(Int_t i=0; i<3; i++)
    {
      TString b_tag = to_string(i+1) + "B";
      
      histo_mc[i] = new TH1D**[3];
      for(Int_t j=0; j<3; j++)
        {
          TString object;
          if(j==0) object = "Muon";
          else if(j==1) object = "Jet0";
          else if(j==2) object = "Jet1";
	  
          histo_mc[i][j] = new TH1D*[n_mc_sample];
          for(Int_t k=0; k<n_mc_sample; k++)
            {
              TString histo_name = target + "_" + b_tag + "_" + object + "_" + mc_sample_name[k];

              histo_mc[i][j][k] = new TH1D(histo_name, histo_name, 60, -3, 3);
            }//n_object
	}//n_b_tag
    }//n_mc_sample

  return;
}//void Validation_MuJet::MakeHisto_MC(TH1D****& histo_mc, const TString& target)

//////////

void Validation_MuJet::Make_Stack_MC(THStack***& stack_mc, const TString& target)
{
  stack_mc = new THStack**[3];
  for(Int_t i=0; i<3; i++)
    {
      TString b_tag = to_string(i+1) + "B";
      
      stack_mc[i] = new THStack*[3];
      for(Int_t j=0; j<3; j++)
        {
          TString object;
          if(j==0) object = "Muon";
          else if(j==1) object = "Jet0";
          else if(j==2) object = "Jet1";
	  
          TString stack_name = target + "_" + b_tag + "_" + object + "_Stack";
	  
          stack_mc[i][j] = new THStack(stack_name, stack_name);
	}//n_object
    }//n_bag
	  
}//void Validation_MuJet::Make_Stack_MC(THStack***& stack_mc, const TString& target)

//////////

void Validation_MuJet::Stack_MC_Histo(TH1D****& histo_mc, THStack***& stack_mc)
{
  for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<3; j++)
	{
	  for(Int_t k=0; k<n_mc_sample; k++)
	    {
	      stack_mc[i][j]->Add(histo_mc[i][j][k]);
	    }//n_mc_sample	  
	}//n_object
    }//n_b_tag
  
  return;
}//void Validation_MuJet::Stack_MC_Histo(TH1D****& histo_mc, THStack***& stack_mc)

//////////

void Validation_MuJet::SetBranchAddress(TFile* fin, TNtuple*& ntuple)
{
  ntuple = (TNtuple*)fin->Get("tuple_variables");
  
  //met
  ntuple->SetBranchAddress("met", &met);
  
  //muon
  ntuple->SetBranchAddress("muon_eta", &muon_eta);
  ntuple->SetBranchAddress("muon_pt", &muon_pt);

  //jets
  ntuple->SetBranchAddress("jet0_eta", &jet0_eta);
  ntuple->SetBranchAddress("jet0_pt", &jet0_pt);
  ntuple->SetBranchAddress("jet1_eta", &jet1_eta);
  ntuple->SetBranchAddress("jet1_pt", &jet1_pt);
  ntuple->SetBranchAddress("jet2_eta", &jet2_eta);
  ntuple->SetBranchAddress("jet2_pt", &jet2_pt);
  ntuple->SetBranchAddress("jet3_eta", &jet3_eta);
  ntuple->SetBranchAddress("jet3_pt", &jet3_pt);
  
  //
  ntuple->SetBranchAddress("n_vertices", &n_vertices);
  ntuple->SetBranchAddress("n_b_tag", &n_b_tag);
  
  //weight & scale factors
  ntuple->SetBranchAddress("weight", &weight);
  ntuple->SetBranchAddress("top_pair_reweight", &top_pair_reweight);

  ntuple->SetBranchAddress("trigger_sf_down", &trigger_sf_down);
  ntuple->SetBranchAddress("trigger_sf", &trigger_sf);
  ntuple->SetBranchAddress("trigger_sf_up", &trigger_sf_up);

  ntuple->SetBranchAddress("weight_by_trigger", &weight_by_trigger);

  ntuple->SetBranchAddress("mc_weight", &mc_weight);

  ntuple->SetBranchAddress("pileup_reweight_down", &pileup_reweight_down);
  ntuple->SetBranchAddress("pileup_reweight", &pileup_reweight);
  ntuple->SetBranchAddress("pileup_reweight_up", &pileup_reweight_up);
    
  ntuple->SetBranchAddress("muon_id_sf_down", &muon_id_sf_down);
  ntuple->SetBranchAddress("muon_id_sf", &muon_id_sf);
  ntuple->SetBranchAddress("muon_id_sf_up", &muon_id_sf_up);

  ntuple->SetBranchAddress("muon_iso_sf_down", &muon_iso_sf_down);
  ntuple->SetBranchAddress("muon_iso_sf", &muon_iso_sf);
  ntuple->SetBranchAddress("muon_iso_sf_up", &muon_iso_sf_up);

  ntuple->SetBranchAddress("muon_tracking_eff_sf", &muon_tracking_eff_sf);

  ntuple->SetBranchAddress("b_tag_sf", &b_tag_reweight);
  
  return;
}//void Validation_MuJet::SetBranchAddress(TFIle* fin, TNtuple* ntuple)

//////////

void Validation_MuJet::Write(TH1D****& histo_mc, THStack***& stack_mc, TH1D***& histo_data, TGraphErrors***& gr_comp)
{
  //save histograms for mc
  for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<3; j++)
	{
	  for(Int_t k=0; k<n_mc_sample; k++)
	    {
	      fout->cd();
	      histo_mc[i][j][k]->Write();
	    }//n_mc_sample
	}//n_object
    }//n_b_tag

  //save stacks for mc
  for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<3; j++)
        {
	  fout->cd();
	  stack_mc[i][j]->Write();
	}//n_object
    }//n_b_tag
  
  //save histograms for data
   for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<3; j++)
        {
	  fout->cd();
	  histo_data[i][j]->Write();
        }//n_object
    }//n_b_tag

   //save graph for comparison
    for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<3; j++)
        {
          fout->cd();
	  gr_comp[i][j]->Write();
	}//n_object
    }//n_b_tag
   
  return;
}//void Validation_MuJet::Write(TH1D****& histo_mc, THStack***& stack_mc, TH1D***& histo_data)

//////////
