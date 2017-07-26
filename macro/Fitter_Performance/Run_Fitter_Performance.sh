#!/bin/bash

root -l -b<<EOF
gSystem->Load("libGpad.so");
gSystem->Load("/home/isyoon/Ana_ChargedHiggs_CB/lib/libAna_ChargedHiggs_CB.so");
.x Run_Fitter_Performance.C()
EOF
