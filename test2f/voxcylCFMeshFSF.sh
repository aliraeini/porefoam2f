#!/bin/bash

source $(dirname "${BASH_SOURCE[0]}")/../../script/bashrc
set -ex


export RefineLevel=0.90  #< this should be less than one for cfMesh to work, check!
export endTime=0.003 #< consider increasing this to 0.01
export maxAlphaCo=15 # heterogeneous mesh, default is 30
export UD1s="1000  0"
export nProc=4
export oilFilldFracs=0.4

export surfaceForceModel=FSF
export meshTag=CfmFSF

# end of config


echo "prepare, remove everything old"
rm -rf  voxyl${meshTag}*/  AllRunImageTwoPhaseCFMesh
sleep 0.5 # for files to get copied!
printf 'AllRunImageTwoPhaseCFMesh: '; which AllRunImageTwoPhaseCFMesh
AllRunImageTwoPhaseCFMesh   copy-self # race condition in make -j2
printf "\n\n\n  *** mesh generation *** \n\n\n"
AllRunImageTwoPhaseCFMesh voxyl.mhd
printf "\n\n\n  *** mesh decomposition *** \n\n\n"
AllRunImageTwoPhaseCFMesh voxyl.mhd
printf "\n\n\n  *** flow simulation *** \n\n\n"
AllRunImageTwoPhaseCFMesh voxyl.mhd
printf "\n\n\n  *** post-process *** \n\n\n"
mkdir -p stats
for cas in `ls voxyl${meshTag}*/log.interFaceFoam`; do
	nam=${cas%/log.interFaceFoam}
	cat $cas | grep Umax > ${nam}_summary.txt
	python3 ./post_process.py  ${nam}_summary.txt > stats/${nam}_stats.txt
done


echo "test"
ls voxyl${meshTag}*Uo0*/log.interFaceFoam
for cas in `ls voxyl${meshTag}*Uo0*/log.interFaceFoam`; do
	nam=${cas%/log.interFaceFoam}
	python3 ./post_process.py test_avg ${nam}_summary.txt Umax  0  0.004
done
ls voxyl${meshTag}*Uo1000*/log.interFaceFoam
for cas in `ls voxyl${meshTag}*Uo1000*/log.interFaceFoam`; do
	nam=${cas%/log.interFaceFoam}
	python3 ./post_process.py test_avg ${nam}_summary.txt Umax  0.04  1.2
done
