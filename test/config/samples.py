# Create Info Dictionary
info = {
}

# 2018 Campaign
qcdsuff18 = '_MuEnrichedPt5_TuneCP5_13TeV_pythia8'
samples18_qcd = [ 'QCD_Pt-170to300'+qcdsuff18,
                  'QCD_Pt-300to470'+qcdsuff18,
                  'QCD_Pt-470to600'+qcdsuff18,
                  'QCD_Pt-600to800'+qcdsuff18,
                  'QCD_Pt-800to1000'+qcdsuff18,
                  'QCD_Pt-1000toInf'+qcdsuff18,
                  ]

xs_qcd18 =[ 8292.982,
          797.35,
          56.59,
          25.10,
          4.71,
          1.62
          ]

info['BTagMu_18'] = {
      'type'    : 'Data',
      'group'   : 'DATA',
      'subsample' : {
          '0' : '2018/10_2_X_9bc62860/BTagMu/'+'Run2018A-17Sep2018-v1',
          '1' : '2018/10_2_X_9bc62860/BTagMu/'+'Run2018B-17Sep2018-v1',
          '2' : '2018/10_2_X_9bc62860/BTagMu/'+'Run2018C-17Sep2018-v1',
          '3' : '2018/10_2_X_9bc62860/BTagMu/'+'Run2018D-PromptReco-v2',
          }
      }

info['BTagMu_18mcjp'] = {
      'type'    : 'Data',
      'group'   : 'DATA',
      'subsample' : {
          '0' : '2018/10_2_X_9bc62860__JPCalibMC_to_data/BTagMu/'+'Run2018A-17Sep2018-v1',
          '1' : '2018/10_2_X_9bc62860__JPCalibMC_to_data/BTagMu/'+'Run2018B-17Sep2018-v1',
          '2' : '2018/10_2_X_9bc62860__JPCalibMC_to_data/BTagMu/'+'Run2018C-17Sep2018-v1',
          '3' : '2018/10_2_X_9bc62860__JPCalibMC_to_data/BTagMu/'+'Run2018D-PromptReco-v2',
          }
      }

for sample, xs in zip(samples18_qcd, xs_qcd18):
  info[sample+'_18'] = {
                'type'    : 'MC',
                'group'   : 'QCDMu+',
                'subsample' : {
                    '0' : '2018/10_2_X_9bc62860/'+sample,
                    },
                'xs':{
                    '0' : xs,
                    }
      }

# 2017 Campaign
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

# 2016 Campaign
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

