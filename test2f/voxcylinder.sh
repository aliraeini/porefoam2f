#!/bin/bash

source $(dirname "${BASH_SOURCE[0]}")/../../script/bashrc
set -ex


export RefineLevel=0.90  #< this should be less than one for cfMesh to work, check!
export endTime=0.003 #< consider increasing this to 0.01
export UD1s="1000 0"
export nProc=4
export oilFilldFracs=0.4

export meshTag=SnpCCF

export smoothMeshNIter=6
export smoothMeshBoundary=yes
export smoothMeshInternal=yes
export smoothMeshRelax=0.1

# end of config


echo "prepare, remove everything old"
rm -rf  voxyl${meshTag}*/  AllRunImageTwoPhase
sleep 0.5 # for files to get copied!
printf 'AllRunImageTwoPhase: '; which AllRunImageTwoPhase
AllRunImageTwoPhase   copy-self # race condition in make -j2
printf "\n\n\n  *** mesh generation *** \n\n\n"
AllRunImageTwoPhase voxyl.mhd
printf "\n\n\n  *** mesh decomposition *** \n\n\n"
AllRunImageTwoPhase voxyl.mhd
printf "\n\n\n  *** flow simulation *** \n\n\n"
AllRunImageTwoPhase voxyl.mhd
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
	python3 ./post_process.py test_avg ${nam}_summary.txt Umax  0  0.0015
done
ls voxyl${meshTag}*Uo1000*/log.interFaceFoam
for cas in `ls voxyl${meshTag}*Uo1000*/log.interFaceFoam`; do
	nam=${cas%/log.interFaceFoam}
	python3 ./post_process.py test_avg ${nam}_summary.txt Umax  0.0026  0.1
done
