#!/bin/bash

ANA_DIR=/home/isyoon/Ana_ChargedHiggs_CB/macro/Fitter_Performance
NOTE_DIR=/home/isyoon/Documents/ChargedHiggs_CB/Kinematic_Fitter_Note/Figs

mv ${ANA_DIR}/Comp_B_Pt_Constraint.eps ${NOTE_DIR}/FitterConditions

mv ${ANA_DIR}/Comp_Match_Prob_B_Pt_Constraint.eps ${NOTE_DIR}/FitterPerformances
mv ${ANA_DIR}/Comp_Mass_B_Pt_Constraint.eps ${NOTE_DIR}/FitterPerformances
mv ${ANA_DIR}/Chi2_OFS_OFF_Sep_2B.eps ${NOTE_DIR}/FitterPerformances
mv ${ANA_DIR}/Chi2_OFS_OFF_Sep_3B.eps ${NOTE_DIR}/FitterPerformances
mv ${ANA_DIR}/Mass_OFS_OFF_Sep_2B.eps ${NOTE_DIR}/FitterPerformances
mv ${ANA_DIR}/Mass_OFS_OFF_Sep_3B.eps ${NOTE_DIR}/FitterPerformances
mv ${ANA_DIR}/Jet_Origin_Match_Prob.eps ${NOTE_DIR}/FitterPerformances
mv ${ANA_DIR}/Neutrino_SM_TT_Low_Mass.eps ${NOTE_DIR}/FitterPerformances
mv ${ANA_DIR}/Neutrino_SM_TT_High_Mass.eps ${NOTE_DIR}/FitterPerformances
mv ${ANA_DIR}/Neutrino_CH_125.eps ${NOTE_DIR}/FitterPerformances

mv ${ANA_DIR}/Fitter_Fail_Reason_2B.eps ${NOTE_DIR}/Investigation
mv ${ANA_DIR}/Fitter_Fail_Reason_3B.eps ${NOTE_DIR}/Investigation

mv ${ANA_DIR}/Chi2_Piece_SM_TT_OFS_OFF_Sep_2B.eps ${NOTE_DIR}/Appendices
mv ${ANA_DIR}/Chi2_Piece_CH_125_OFS_OFF_Sep_2B.eps ${NOTE_DIR}/Appendices
mv ${ANA_DIR}/Chi2_Piece_SM_TT_Low_Mass_OFS_OFF_Sep_3B.eps ${NOTE_DIR}/Appendices
mv ${ANA_DIR}/Chi2_Piece_SM_TT_High_Mass_OFS_OFF_Sep_3B.eps ${NOTE_DIR}/Appendices
mv ${ANA_DIR}/Chi2_Piece_CH_125_OFS_OFF_Sep_3B.eps ${NOTE_DIR}/Appendices
mv ${ANA_DIR}/Neutrino_Pz_SM_TT_Low_Mass_OFS_OFF_Sep.eps ${NOTE_DIR}/Appendices
mv ${ANA_DIR}/Neutrino_Pz_CH_125_OFS_OFF_Sep.eps ${NOTE_DIR}/Appendices
