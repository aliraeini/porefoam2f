#!/bin/bash

source $(dirname "${BASH_SOURCE[0]}")/../../script/bashrc
set -ex


export cornAngs=45
export RefineLevel=4  #< this should be less than one for cfMesh to work, check!
export endTime=0.001 #< consider increasing this to 0.01
export maxAlphaCo=15 # heterogeneous mesh, default is 30
export UD1s="1000"
export nProc=4
export oilFilldFracs=0.4

export meshTag=CfmCCF

# end of config


echo "prepare, remove everything old"
rm -rf  star${meshTag}*/  AllRunImageTwoPhase_star
sleep 0.5 # for files to get copied!
AllRunImageTwoPhase_star   copy-self # race condition in make -j2
echo "pre-process and run"
AllRunImageTwoPhase_star
AllRunImageTwoPhase_star
AllRunImageTwoPhase_star
