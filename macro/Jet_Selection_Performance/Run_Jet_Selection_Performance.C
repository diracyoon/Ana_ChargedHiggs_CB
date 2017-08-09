void Run_Jet_Selection_Performance()
{
  Double_t cut_on_chi2 = 50;
  Int_t selection = 0;
  
  Jet_Selection_Performance* jet_selection_performance = new Jet_Selection_Performance("eps", cut_on_chi2);

  jet_selection_performance->Reconstruction_Result_Mass_2B(selection);
  jet_selection_performance->Reconstruction_Result_Mass_3B(selection);
  jet_selection_performance->Reconstruction_Result_Chi2_2B(selection);
  jet_selection_performance->Reconstruction_Result_Chi2_3B(selection);
  jet_selection_performance->Reconstruction_Result_Chi2_Piece_2B(selection);
  jet_selection_performance->Reconstruction_Result_Chi2_Piece_3B(selection);
  jet_selection_performance->Signal_Fraction_Vs_Chi2_2B(selection);
  jet_selection_performance->Signal_Fraction_Vs_Chi2_3B(selection);
      
  delete jet_selection_performance;
  
  return;
}
