#!/bin/bash

root -l -b<<EOF
gSystem->Load("/home/isyoon/Ana_ChargedHiggs_CB/lib/libAna_ChargedHiggs_CB.so");
.x Run_Fitter_Test.C()
EOF
