#!/bin/bash

source $(dirname "${BASH_SOURCE[0]}")/../../script/bashrc
set -ex


export cornAng1s=45
export RefineLevel=4  #< this should be less than one for cfMesh to work, check!
export endTime=0.001 #< consider increasing this to 0.01
export UD1s="1000"
export nProc=4
export oilFilldFracs=0.4


export meshTag=CfmCCF

# end of config


echo "prepare, remove everything old"
rm -rf  tri${meshTag}*/  AllRunImageTwoPhase_tri

AllRunImageTwoPhase_tri   copy-self
echo "pre-process and run"
AllRunImageTwoPhase_tri
AllRunImageTwoPhase_tri
AllRunImageTwoPhase_tri
echo "post-process"
