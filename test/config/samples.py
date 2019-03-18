# 2017 Campaign

### DoubleX commissioing 2017

# v03
#datasuff ="-17Nov2017-v1_v03_20190222"
#qcdsuff ="_MuEnrichedPt5_TuneCP5_13TeV_pythia8_v03_20190222"
datasuff ="-17Nov2017-v1_v04_20190228"
qcdsuff ="_MuEnrichedPt5_TuneCP5_13TeV_pythia8_v04_20190228"
samples_data = [  'BTagMu_Run2017B'+datasuff,
                  'BTagMu_Run2017C'+datasuff,
                  'BTagMu_Run2017D'+datasuff,
                  'BTagMu_Run2017E'+datasuff,
                  'BTagMu_Run2017F'+datasuff,
                  ]
samples_qcd = [   'QCD_Pt-170to300'+qcdsuff,
                  'QCD_Pt-300to470'+qcdsuff,
                  'QCD_Pt-470to600'+qcdsuff,
                  'QCD_Pt-600to800'+qcdsuff,
                  'QCD_Pt-800to1000'+qcdsuff,
                  'QCD_Pt-1000toInf'+qcdsuff,
                  ]

xs_qcd = [  8292.98,
            797.35,
            56.59,
            25.10,
            4.71,
            1.62
            ]
# Create Info Dictionary
info = {
}

# Fill infos for each sample
for sample in samples_data:
  info[sample] = {
                'type'    : 'Data',
                'group'   : 'DATA',
                'subsample' : {
                    '0' : '2017/9_4_X_v04/'+sample,
                    }
                }
for sample, xs in zip(samples_qcd, xs_qcd):
  print sample, xs
  info[sample] = {
                'type'    : 'MC',
                'group'   : 'QCDMu+',
                'subsample' : {
                    '0' : '2017/9_4_X_v04/'+sample,
                    },
                'xs':{
                    '0' : xs,
                    }
      }


### DoubleX commissioing 2016

datasuff ="-17Jul2018-v1_9_4_X_v04"
qcdsuff ="_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8_9_4_X_v04"
qcdsuffext ="_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8Ext_9_4_X_v04"
samples_data = [  'BTagMu_Run2016B'+"-17Jul2018_ver2-v1_9_4_X_v04",
                  'BTagMu_Run2016C'+datasuff,
                  'BTagMu_Run2016D'+datasuff,
                  'BTagMu_Run2016E'+datasuff,
                  'BTagMu_Run2016F'+datasuff,
                  'BTagMu_Run2016G'+datasuff,
                  'BTagMu_Run2016H'+datasuff,
                  ]
samples_qcd = [   'QCD_Pt-170to300'+qcdsuff,
                  'QCD_Pt-300to470'+qcdsuff,
                  'QCD_Pt-470to600'+qcdsuff,
                  'QCD_Pt-600to800'+qcdsuff,
                  'QCD_Pt-800to1000'+qcdsuff,
                  'QCD_Pt-1000toInf'+qcdsuff,
                  ]
samples_qcd_ext = ['QCD_Pt-170to300'+qcdsuffext,
                  'QCD_Pt-300to470'+qcdsuffext,
                  'QCD_Pt-470to600'+qcdsuffext,
                  'QCD_Pt-600to800'+qcdsuffext,
                  'QCD_Pt-800to1000'+qcdsuffext,
                  'QCD_Pt-1000toInf'+qcdsuffext,
                  ]

xs_qcd = [  8654.49,
            797.35,
            79.02,
            25.095,
            4.707,
            1.621
            ]

# Fill infos for each sample
for sample in samples_data:
  info[sample] = {
                'type'    : 'Data',
                'group'   : 'DATA',
                'subsample' : {
                    '0' : '2016/9_4_X_v04/'+sample,
                    }
                }
for sample, sample_ext, xs in zip(samples_qcd, samples_qcd_ext, xs_qcd):
  print sample, xs
  info[sample] = {
                'type'    : 'MC',
                'group'   : 'QCDMu+',
                'subsample' : {
                    '0' : '2016/9_4_X_v04/'+sample,
                    '1' : '2016/9_4_X_v04/'+sample_ext,
                    },
                'xs':{
                    '0' : xs,
                    '1' : xs,
                    }
      }

if __name__ == "__main__":
  import pprint
  pprint.pprint(info, depth=3)



#### First attempt at producing comm plots - Jan10-2018
#
#info = {
#  # ------------------------------------ Data ------------------------------------------
#  'BTagMu':{
#    'type'    : 'Data',
#    'group'   : 'DATA',
#    'subsample' : {
#      '0' : 'BTagMu_Run2017B-17Nov2017-v1_MINIAOD',
#      '1' : 'BTagMu_Run2017C-17Nov2017-v1_MINIAOD',
#      '2' : 'BTagMu_Run2017D-17Nov2017-v1_MINIAOD',
#    }
#  },
#
#  'QCD_Pt-170to300_MuEnrichedPt5_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDMu+',
#    'subsample' : {
#      '0' : 'QCD_Pt_170to300_MuEnriched_v1_Jan3-2018',
#      #'1' : '',
#    },
#    'xs':{
#      '0' : 8292.982,#xs:118100, filter eff:0.07022 #McM
#      #'1' : 0.0,
#    }
#  },
#  'QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDMu+',
#    'subsample' : {
#      '0' : 'QCD_Pt_300to470_MuEnriched_v1_Jan3-2018',
#    },
#    'xs':{
#      '0' : 797.35269,#xs:7820.25, filter eff:0.10196 #McM
#    },
#  },
#  'QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDMu+',
#    'subsample' : {
#      '0' : 'QCD_Pt_470to600_MuEnriched_v1_Jan3-2018',
#    },
#    'xs':{
#      '0' : 56.588336,#xs:648.8, filter eff:0.08722 #McM
#    }
#  },
#  'QCD_Pt-600to800_MuEnrichedPt5_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDMu+',
#    'subsample' : {
#      '0' : '',
#    },
#    'xs':{
#      '0' : 25.09505908,#xs:187.109, filter eff:0.13412 #McM
#    }
#  },
#  'QCD_Pt-800to1000_MuEnrichedPt5_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDMu+',
#    'subsample' : {
#      '0' : '',
#    },
#    'xs':{
#      '0' : 4.707368272,#xs:, filter eff: #McM
#    }
#  },
#  'QCD_Pt-1000toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDMu+',
#    'subsample' : {
#      '0' : 'QCD_Pt_1000toInf_MuEnriched_v1_Jan3-2018',
#    },
#    'xs':{
#      '0' : 1.62131692,#xs:10.4305, filter eff:0.15544 #McM
#    }
#  },
#
#
#  'QCD_Pt_170to300_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDincl',
#    'subsample' : {
#      '0' : 'QCD_Pt_170to300_v1_Jan3-2018',
#    },
#    'xs':{
#      '0' : 19204300,#xs:19204300 #McM
#    }
#  },
#  'QCD_Pt_300to470_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDincl',
#    'subsample' : {
#      '0' : 'QCD_Pt_300to470_v1_Jan3-2018',
#    },
#    'xs':{
#      '0' : 7823,#xs:7823 #McM
#    },
#  },
#  'QCD_Pt_470to600_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDincl',
#    'subsample' : {
#      '0' : 'QCD_Pt_470to600_v1_Jan3-2018_v2',
#    },
#    'xs':{
#      '0' : 648.2,#xs:648.2 #McM
#    }
#  },
#  'QCD_Pt_600to800_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDincl',
#    'subsample' : {
#      '0' : 'QCD_Pt_600to800_v1_Jan3-2018',
#    },
#    'xs':{
#      '0' : 186.9,#xs:186.9 #McM
#    }
#  },
#  'QCD_Pt_800to1000_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDincl',
#    'subsample' : {
#      '0' : 'QCD_Pt_800to1000_v2_Jan3-2018',
#    },
#    'xs':{
#      '0' : 32.293,#xs:32.293 #McM
#    }
#  },
#  'QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDincl',
#    'subsample' : {
#      '0' : 'QCD_Pt_1000to1400_v1_Jan3-2018',
#    },
#    'xs':{
#      '0' : 9.4183,#xs:9.4183 #McM
#    }
#  },
#  'QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDincl',
#    'subsample' : {
#      '0' : 'QCD_Pt_1400to1800_v1_Jan3-2018',
#    },
#    'xs':{
#      '0' : 0.84265,#xs:0.84265 #McM
#    }
#  },
#  'QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDincl',
#    'subsample' : {
#      '0' : 'QCD_Pt_1800to2400_v1_Jan3-2018',
#    },
#    'xs':{
#      '0' : 0.114943,#xs:0.114943 #McM
#    }
#  },
#  'QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDincl',
#    'subsample' : {
#      '0' : 'QCD_Pt_2400to3200_v2_Jan3-2018',
#    },
#    'xs':{
#      '0' : 0.00682981,#xs:0.00682981 #McM
#    }
#  },
#  'QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8':{
#    'type'    : 'MC',
#    'group'   : 'QCDincl',
#    'subsample' : {
#      '0' : 'QCD_Pt_3200toInf_v1_Jan3-2018',
#    },
#    'xs':{
#      '0' : 0.000165445,#xs:0.000165445 #McM
#    }
#  },
#
#
#}

### Alice want just data with AK8Jet300 OR AK4Jet300, and another with just Dijet170 trigger(s).

# info = {
#   # ------------------------------------ Data ------------------------------------------
#   'BTagMu':{
#     'type'    : 'Data',
#     'group'   : 'DATA',
#     'subsample' : {
#       '0' : 'BTagMu_Run2017B-23Jun2017-v1_MINIAOD',
#       '1' : 'BTagMu_Run2017B-PromptReco-v1_MINIAOD',
#       '2' : 'BTagMu_Run2017B-PromptReco-v2_MINIAOD',
#       '3' : 'BTagMu_Run2017C-PromptReco-v1_MINIAOD',
#       '4' : 'Run2017C/BTagMu/Run2017C-PromptReco-v2_9_2_X',
#       '5' : 'Run2017C/BTagMu/Run2017C-PromptReco-v3_9_2_X',
#       '6' : 'BTagMu_Run2017C-PromptReco-v3_MINIAOD',
#       '7' : 'BTagMu_Run2017D-PromptReco-v1_MINIAOD',
#     }
#   },
#
# }

### producing Data/QCDMuEnriched 2017 v1 - ~last week of Oct 2017

# info = {
#   # ------------------------------------ Data ------------------------------------------
#   'BTagMu':{
#     'type'    : 'Data',
#     'group'   : 'DATA',
#     'subsample' : {
#       '0' : 'BTagMu_Run2017B-23Jun2017-v1_MINIAOD',
#       '1' : 'BTagMu_Run2017B-PromptReco-v1_MINIAOD',
#       '2' : 'BTagMu_Run2017B-PromptReco-v2_MINIAOD',
#       '3' : 'BTagMu_Run2017C-PromptReco-v1_MINIAOD',
#       '4' : 'Run2017C/BTagMu/Run2017C-PromptReco-v2_9_2_X',
#       '5' : 'Run2017C/BTagMu/Run2017C-PromptReco-v3_9_2_X',
#       '6' : 'BTagMu_Run2017C-PromptReco-v3_MINIAOD',
#       '7' : 'BTagMu_Run2017D-PromptReco-v1_MINIAOD',
#     }
#   },
# #   'QCD_Pt-120to170_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
# #     'type'    : 'MC',
# #     'group'   : 'QCD',
# #     'subsample' : {
# #       '0' : 'QCD_Pt_120-170_MuEnriched_mcJPcalib_12616_v4_full',
# #       '1' : 'QCD_Pt_120-170_MuEnriched_mcJPcalib_12616_backup_v4_full',
# #     },
# #     'xs':{
# #       '0' : 25190.51514,
# #       '1' : 25190.51514,
# #     }
# #   },
#   'QCD_Pt-170to300_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_170-300_MuEnriched_v1_Sep19-2017',
#       '1' : 'QCD_Pt_170-300_MuEnriched_v2_Sep19-2017',
#     },
#     'xs':{
#       '0' : 8654.49315,
#       '1' : 8654.49315,
#     }
#   },
#   'QCD_Pt-300to470_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_300-470_MuEnriched_v1_Sep19-2017',
#       '1' : 'QCD_Pt_300-470_MuEnriched_v2_Sep19-2017',
#     },
#     'xs':{
#       '0' : 797.3526900,
#       '1' : 797.3526900,
#     },
#   },
#   'QCD_Pt-470to600_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_470-600_MuEnriched_v1_Sep19-2017',
#     },
#     'xs':{
#       '0' : 79.02553776,
#     }
#   },
#   'QCD_Pt-600to800_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_600-800_MuEnriched_v1_Sep19-2017',
#       '1' : 'QCD_Pt_600-800_MuEnriched_v2_Sep19-2017',
#     },
#     'xs':{
#       '0' : 25.09505908,
#       '1' : 25.09505908,
#     }
#   },
#   'QCD_Pt-800to1000_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_800-1000_MuEnriched_v1_Sep19-2017',
#       '1' : 'QCD_Pt_800-1000_MuEnriched_v2_Sep19-2017',
#     },
#     'xs':{
#       '0' : 4.707368272,
#       '1' : 4.707368272,
#     }
#   },
#   'QCD_Pt-1000toInf_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_1000-Inf_MuEnriched_v1_Sep19-2017',
#     },
#     'xs':{
#       '0' : 1.621316920,
#     }
#   },
#
#
# }

# # 2016 Campaign

# info = {
#   # ------------------------------------ Data ------------------------------------------
#   'JetHT':{
#     'type'    : 'Data',
#     'group'   : 'DATA',
#     'subsample' : {
#       '0' : 'JetHT_Run2016B-23Sep2016-v3_MINIAOD_mcJPcalib_12616',
#       '1' : 'JetHT_Run2016C-23Sep2016-v1_MINIAOD_mcJPcalib_12616',
#       '2' : 'JetHT_Run2016D-23Sep2016-v1_MINIAOD_mcJPcalib_12616',
#       '3' : 'JetHT_Run2016E-23Sep2016-v1_MINIAOD_mcJPcalib_12616',
#       '4' : 'JetHT_Run2016F-23Sep2016-v1_MINIAOD_mcJPcalib_12616',
#       '5' : 'JetHT_Run2016G-23Sep2016-v1_MINIAOD_mcJPcalib_12616',
#       '6' : 'JetHT_Run2016H-PromptReco-v2_MINIAOD_mcJPcalib_12616',
#       '7' : 'JetHT_Run2016H-PromptReco-v3_MINIAOD_mcJPcalib_12616',
#     }
#   },

#   # -------------------------------------- MC ------------------------------------------
#   'QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_170-300_Inclusive_mcJPcalib_12616',
#     },
#     'xs':{
#       '0' : 1.172e+05, #cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/50000/02D7719D-01B5-E611-A239-A0000420FE80.root" maxEvents=-1
#     }
#   },

#   'QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_300-470_Inclusive_mcJPcalib_12616',
#     },
#     'xs':{
#       '0' : 7.760e+03, # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/008761B6-F5B4-E611-AC01-0CC47A706D26.root" maxEvents=-1
#     }
#   }

#   'QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_470-600_Inclusive_mcJPcalib_12616',
#       '1' : 'QCD_Pt_470-600_Inclusive_mcJPcalib_12616_backup'
#     },
#     'xs':{
#       '0' : 6.417e+02*0.5, # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/022C3683-D4AB-E611-AC4D-3417EBE70078.root" maxEvents=-1
#       '1' : 6.407e+02*0.5  # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/110000/0217976D-B8B4-E611-8345-001E67E0061C.root" maxEvents=-1
#     }
#   },

#   'QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_600-800_Inclusive_mcJPcalib_12616',
#       '1' : 'QCD_Pt_600-800_Inclusive_mcJPcalib_12616_backup'
#     },
#     'xs':{
#       '0' : 1.852e+02*0.5, # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/0048131D-3CB3-E611-813A-001E67DFFB31.root" maxEvents=-1
#       '1' : 1.857e+02*0.5  # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/110000/0085453C-7DB6-E611-B5D4-002590D0AFCA.root" maxEvents=-1
#     }
#   },

#   'QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_800-1000_Inclusive_mcJPcalib_12616',
#       '1' : 'QCD_Pt_800-1000_Inclusive_mcJPcalib_12616_ext1'
#     },
#     'xs':{
#       '0' : 3.208e+01*0.5, # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/14E8104C-9AB0-E611-86C7-001E673CFC91.root" maxEvents=-1
#       '1' : 3.204e+01*0.5  # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/110000/000D6738-6EB1-E611-8451-0CC47A4D76B8.root" maxEvents=-1
#     }
#   },

#   'QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_1000-1400_Inclusive_mcJPcalib_12616',
#       '1' : 'QCD_Pt_1000-1400_Inclusive_mcJPcalib_12616_ext1'
#     },
#     'xs':{
#       '0' : 9.354*0.5, # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/002BD1CD-24B5-E611-A29C-0025907254C8.root" maxEvents=-1
#       '1' : 9.390*0.5  # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/60000/000B9FF0-11B6-E611-9083-008CFA0A5844.root" maxEvents=-1
#     }
#   },

#   'QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_1400-1800_Inclusive_mcJPcalib_12616',
#       '1' : 'QCD_Pt_1400-1800_Inclusive_mcJPcalib_12616_ext1'
#     },
#     'xs':{
#       '0' : 8.406e-01*0.5, # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/110000/021F2782-BEB1-E611-8581-0025905A48E4.root" maxEvents=-1
#       '1' : 8.360e-01*0.5  # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/110000/A2C23801-9DAE-E611-8369-008CFA0A58B0.root" maxEvents=-1
#     }
#   },

#   'QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_1800-2400_Inclusive_mcJPcalib_12616',
#     },
#     'xs':{
#       '0' : 1.124e-01, # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/544D528B-03B3-E611-B70A-B083FED4263D.root" maxEvents=-1
#     }
#   },

#   'QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_2400-3200_Inclusive_mcJPcalib_12616',
#     },
#     'xs':{
#       '0' : 6.762e-03, # cmsRun ana.py inputFiles="/store/mc/RunIISummer16MiniAODv2/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/110000/060A4CEC-A4B1-E611-921A-0CC47A7C3434.root" maxEvents=-1
#     }
#   },
# }

# info = {
#   # ------------------------------------ Data ------------------------------------------
#   'BTagMu':{
#     'type'    : 'Data',
#     'group'   : 'DATA',
#     'subsample' : {
#       '0' : 'BTagMu_Run2016B-23Sep2016-v3_MINIAOD_mcJPcalib_12616_v4',
#       '1' : 'BTagMu_Run2016C-23Sep2016-v1_MINIAOD_mcJPcalib_12616_v4',
#       '2' : 'BTagMu_Run2016D-23Sep2016-v1_MINIAOD_mcJPcalib_12616_v4',
#       '3' : 'BTagMu_Run2016E-23Sep2016-v1_MINIAOD_mcJPcalib_12616_v4',
#       '4' : 'BTagMu_Run2016F-23Sep2016-v1_MINIAOD_mcJPcalib_12616_v4',
#       '5' : 'BTagMu_Run2016G-23Sep2016-v1_MINIAOD_mcJPcalib_12616_v4_full',
#       '6' : 'BTagMu_Run2016H-PromptReco-v2_MINIAOD_mcJPcalib_12616_v4',
#       '7' : 'BTagMu_Run2016H-PromptReco-v3_MINIAOD_mcJPcalib_12616_v4',
#     }
#   },
#   # -------------------------------------- MC ------------------------------------------
#   'QCD_Pt-120to170_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_120-170_MuEnriched_mcJPcalib_12616_v4_full',
#       '1' : 'QCD_Pt_120-170_MuEnriched_mcJPcalib_12616_backup_v4_full',
#     },
#     'xs':{
#       '0' : 25190.51514,
#       '1' : 25190.51514,
#     }
#   },
#   'QCD_Pt-170to300_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_170-300_MuEnriched_mcJPcalib_12616_v4_full',
#       '1' : 'QCD_Pt_170-300_MuEnriched_mcJPcalib_12616_backup_v4',
#       '2' : 'QCD_Pt_170-300_MuEnriched_mcJPcalib_12616_ext1_v4',
#     },
#     'xs':{
#       '0' : 8654.49315,
#       '1' : 8654.49315,
#       '2' : 8654.49315,
#     }
#   },
#   'QCD_Pt-300to470_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_300-470_MuEnriched_mcJPcalib_12616_v4_full',
#       '1' : 'QCD_Pt_300-470_MuEnriched_mcJPcalib_12616_ext1_v4_full',
#       '2' : 'QCD_Pt_300-470_MuEnriched_mcJPcalib_12616_ext2_v4_full',
#     },
#     'xs':{
#       '0' : 797.3526900,
#       '1' : 797.3526900,
#       '2' : 797.3526900,
#     },
#     # --- btagvalidation_cfg ---
#     'btagvalidation_cfg': {
#       #'doJECUncert'             : True,
#     }
#   },
#   'QCD_Pt-470to600_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_470-600_MuEnriched_mcJPcalib_12616_v4_full',
#       '1' : 'QCD_Pt_470-600_MuEnriched_mcJPcalib_12616_ext1_v4_full',
#       '2' : 'QCD_Pt_470-600_MuEnriched_mcJPcalib_12616_ext2_v4_full',
#     },
#     'xs':{
#       '0' : 79.02553776,
#       '1' : 79.02553776,
#       '2' : 79.02553776,
#     }
#   },
#   'QCD_Pt-600to800_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_600-800_MuEnriched_mcJPcalib_12616_v4_full',
#       '1' : 'QCD_Pt_600-800_MuEnriched_mcJPcalib_12616_ext1_v4_full',
#       '2' : 'QCD_Pt_600-800_MuEnriched_mcJPcalib_12616_backup_v4_full',
#     },
#     'xs':{
#       '0' : 25.09505908,
#       '1' : 25.09505908,
#       '2' : 25.09505908,
#     }
#   },
#   'QCD_Pt-800to1000_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_800-1000_MuEnriched_mcJPcalib_12616_v4_full',
#       '1' : 'QCD_Pt_800-1000_MuEnriched_mcJPcalib_12616_ext1_v4_full',
#       '2' : 'QCD_Pt_800-1000_MuEnriched_mcJPcalib_12616_ext2_v4_full',
#     },
#     'xs':{
#       '0' : 4.707368272,
#       '1' : 4.707368272,
#       '2' : 4.707368272,
#     }
#   },
#   'QCD_Pt-1000toInf_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8':{
#     'type'    : 'MC',
#     'group'   : 'QCD',
#     'subsample' : {
#       '0' : 'QCD_Pt_1000-Inf_MuEnriched_mcJPcalib_12616_v4_full',
#       '1' : 'QCD_Pt_1000-Inf_MuEnriched_mcJPcalib_12616_v4_full_ext1',
#     },
#     'xs':{
#       '0' : 1.621316920,
#       '1' : 1.621316920,
#     }
#   },
# }
