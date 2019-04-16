#!/bin/bash

export SCRAM_ARCH=<SCRAM_ARCH>
export X509_USER_PROXY=<X509_USER_PROXY_file>

cd <CMSSW_dir>
eval `scramv1 runtime -sh`

cd -
cp <X509_USER_PROXY_path> .
ls <X509_USER_PROXY_path>
cp /.automount/home/home__home1/institut_3a/novak/krb5cc_novak . 
eosfusebind -g krb5 krb5cc_novak
TT=<input_files>[0] 
TT=(${TT//;/ }) 
TT=(${TT//,/ }) 
ls $TT
kdestroy
cmsRun <path_batch_file_wo_ext>.py
rm <X509_USER_PROXY_file>

