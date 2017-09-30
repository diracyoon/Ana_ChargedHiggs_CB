#include "Validation.h"

ClassImp(Validation);

//////////

Validation::Validation() : n_mc_sample(2)
{
  mc_sample_name = new TString[n_mc_sample];
  mc_sample_name[0] = "SKTT_powheg";
  mc_sample_name[1] = "SKWJets";
  
  fin_mc = new TFile*[n_mc_sample]; 
  for(Int_t i=0; i<n_mc_sample; i++)
    {
      TString fin_name = "./Root_Files/Validation_CH_MuJet_";
      fin_name += mc_sample_name[i];
      fin_name += "_cat_v8-0-7.root";

      fin_mc[i] = new TFile(fin_name);
    }      
  
  ntuple_mc = new TNtuple*[n_mc_sample];
  for(Int_t i=0;i<n_mc_sample; i++)
    {
      ntuple_mc[i] = (TNtuple*)fin_mc[i]->Get("tuple_variables");

      //met
      ntuple_mc[i]->SetBranchAddress("met", &met);

      //muon
      ntuple_mc[i]->SetBranchAddress("muon_eta", &muon_eta);
      ntuple_mc[i]->SetBranchAddress("muon_pt", &muon_pt);

      //jets
      ntuple_mc[i]->SetBranchAddress("jet0_eta", &jet0_eta);
      ntuple_mc[i]->SetBranchAddress("jet0_pt", &jet0_pt);
      ntuple_mc[i]->SetBranchAddress("jet1_eta", &jet1_eta);
      ntuple_mc[i]->SetBranchAddress("jet1_pt", &jet1_pt);
      ntuple_mc[i]->SetBranchAddress("jet2_eta", &jet2_eta);
      ntuple_mc[i]->SetBranchAddress("jet2_pt", &jet2_pt);
      ntuple_mc[i]->SetBranchAddress("jet3_eta", &jet3_eta);
      ntuple_mc[i]->SetBranchAddress("jet3_pt", &jet3_pt);
      
      //
      ntuple_mc[i]->SetBranchAddress("n_vertices", &n_vertices);

      //weight & scale factors
      ntuple_mc[i]->SetBranchAddress("weight", &weight);
    }
      
  //fin_data = new TFile("./Root_Files/");
}//Validation::Validation()

//////////

Validation::~Validation()
{
  delete[] mc_sample_name;
  
  for(Int_t i=0; i<n_mc_sample; i++)
    {
      fin_mc[i]->Close();
      delete fin_mc[i];
    }
  delete fin_mc;

  //delete fin_data;
}//Validation::~Validation()

//////////

void Validation::Eta(const TString& target_object)
{
  if(target_object=="MUON")
    {
    }
  else if(target_object=="LEADING_0_JET")
    {
    }
  else if(target_object=="LEADING_1_JET")
    {
    }
  else
    {
      cout << "Wrong input. Check target object first." << endl;
      return;
    }

  //fill histograms
  TH1D** histo_mc = new TH1D*[n_mc_sample];
  for(Int_t i=0; i<n_mc_sample; i++)
    {
      histo_mc[i] = new TH1D(mc_sample_name[i], mc_sample_name[i], 60, -3, 3);

      for(Int_t j=0; j<ntuple_mc[i]->GetEntries(); j++)
	{
	  ntuple_mc[i]->GetEntry(j);
	  cout << mc_sample_weight << endl;
	}
    }
  
  TCanvas can("can", "can", 800, 500);
  
  return;
}//void Validatation::Eta(const TString& target_object)

//////////
