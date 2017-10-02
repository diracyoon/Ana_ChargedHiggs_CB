#include "Validation.h"

ClassImp(Validation);

//////////

Validation::Validation() : n_mc_sample(2)
{
  //set mc names
  mc_sample_name = new TString[n_mc_sample];
  mc_sample_name[0] = "SKTT_powheg";
  mc_sample_name[1] = "SKWJets";

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
      
  fout = new TFile("Validation_Results.root", "RECREATE");
  
  //fin_data = new TFile("./Root_Files/");
}//Validation::Validation()

//////////

Validation::~Validation()
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
  
  //delete fin_data;
}//Validation::~Validation()

//////////

void Validation::Eta()
{
  //make histograms for mc
  TH1D*** histo_muon_mc = new TH1D**[3];
  TH1D*** histo_jet0_mc = new TH1D**[3];
  TH1D*** histo_jet1_mc = new TH1D**[3];
  for(Int_t i=0; i<3; i++)
    {
      TString b_tag = to_string(i+1) + "B";
   
      histo_muon_mc[i] = new TH1D*[n_mc_sample];
      histo_jet0_mc[i] = new TH1D*[n_mc_sample];
      histo_jet1_mc[i] = new TH1D*[n_mc_sample];
      
      for(Int_t j=0; j<n_mc_sample; j++)
	{
	  TString histo_muon_name = "Muon_Eta_" + b_tag + "_" + mc_sample_name[j];
	  histo_muon_mc[i][j] = new TH1D(histo_muon_name, histo_muon_name, 60, -3, 3);

	  TString histo_jet0_name = "Jet0_Eta_" + b_tag + "_" + mc_sample_name[j];
	  histo_jet0_mc[i][j] = new TH1D(histo_jet0_name, histo_jet0_name, 60, -3, 3);

	  TString histo_jet1_name = "Jet1_Eta_" + b_tag + "_" + mc_sample_name[j];
	  histo_jet1_mc[i][j] = new TH1D(histo_jet1_name, histo_jet0_name, 60, -3, 3);
	}
    }

  
  //fill histograms for mc
  for(Int_t i=0; i<n_mc_sample; i++)
    {
      for(Int_t j=0; j<ntuple_mc[i]->GetEntries(); j++)
      	{
      	  ntuple_mc[i]->GetEntry(j);
	  
	  Double_t effective_weight = weight;

	  Int_t index_b_tag = 0;;
	  if(TMath::Abs(n_b_tag-1)<1E-5) index_b_tag = 0;
	  else if(TMath::Abs(n_b_tag-2)<1E-5) index_b_tag = 1;
	  else if(TMath::Abs(n_b_tag-3)<1E-5) index_b_tag = 2;

	  histo_muon_mc[index_b_tag][i]->Fill(muon_eta, effective_weight);
      	}
  
      for(Int_t j=0; j<3; j++)
	{
	  fout->cd();
	  histo_muon_mc[j][i]->Write();
	  histo_jet0_mc[j][i]->Write();
	  histo_jet1_mc[j][i]->Write();
	}
    }

  //make histograms for data
  TH1D** histo_muon_data = new TH1D*[3]; 
  TH1D** histo_jet0_data = new TH1D*[3];
  TH1D** histo_jet1_data = new TH1D*[3];
  for(Int_t i=0; i<3; i++)
    {
      TString b_tag = to_string(i+1) + "B";
      
      TString histo_muon_name = "Muon_Eta_" + b_tag + "_Data";
      histo_muon_data[i] = new TH1D(histo_muon_name, histo_muon_name, 60, -3, 3);

      TString histo_jet0_name = "Jet0_Eta_" + b_tag + "_Data";
      histo_jet0_data[i] = new TH1D(histo_jet0_name, histo_jet0_name, 60, -3, 3);

      TString histo_jet1_name = "Jet1_Eta" + b_tag + "_Data";
      histo_jet1_data[i] = new TH1D(histo_jet1_name, histo_jet1_name, 60, -3, 3);
    }
  
  //fill histogram data
  for(Int_t i=0; i<ntuple_data->GetEntries(); i++)
    {
      ntuple_data->GetEntry(i);

      Int_t index_b_tag = 0;;
      if(TMath::Abs(n_b_tag-1)<1E-5) index_b_tag = 0;
      else if(TMath::Abs(n_b_tag-2)<1E-5) index_b_tag = 1;
      else if(TMath::Abs(n_b_tag-3)<1E-5) index_b_tag = 2;
      
      histo_muon_data[index_b_tag]->Fill(muon_eta, weight);
    }

  for(Int_t i=0; i<3; i++)
    {
      fout->cd();
      histo_muon_data[i]->Write();
      histo_jet0_data[i]->Write();
      histo_jet1_data[i]->Write();
    }

  
  
  //TCanvas can("can", "can", 800, 500);

  //delete
  for(Int_t i=0; i<3; i++)
    {
      for(Int_t j=0; j<n_mc_sample; j++)
	{
	  delete histo_muon_mc[i][j]; 
	  delete histo_jet0_mc[i][j];
	  delete histo_jet1_mc[i][j];
	}

      delete[] histo_muon_mc[i];
      delete[] histo_jet0_mc[i];
      delete[] histo_jet1_mc[i];
    }

  delete[] histo_muon_mc;
  delete[] histo_jet0_mc;
  delete[] histo_jet1_mc;

  for(Int_t i=0; i<3; i++)
    {
     delete histo_muon_data[i];    
     delete histo_jet0_data[i];
     delete histo_jet1_data[i];
    }

  delete[] histo_muon_data;
  delete[] histo_jet0_data;
  delete[] histo_jet1_data;
  
  return;
}//void Validatation::Eta()

//////////

void Validation::Compare()
{
  return;
}

//////////

void Validation::SetBranchAddress(TFile* fin, TNtuple*& ntuple)
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

  return;
}//void Validation::SetBranchAddress(TFIle* fin, TNtuple* ntuple)

//////////
