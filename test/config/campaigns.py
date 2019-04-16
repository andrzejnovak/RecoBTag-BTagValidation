import os
import paths
from samples import info as samples_info
# Should import more names from samples directly

def string(txt):
  ''' str wrapper since cmsRun/python requires string to have "". '''

  ''' RECIPE: If working with special flags e.g.
  'triggerSelection'          : string( ','.join(['HLT_BTagMu_AK8Jet300_Mu5' + "'", "'" + 'HLT_BTagMu_Jet300_Mu5']))
  '''
  return txt.join(["'", "'"])

##############################
##############################
taggers = ["DoubleB", "DDBvL", "DDCvL", "DeepAK8ZHbb"]
run_names = ['Run2016', 'Run2017', 'Run2018']

Comm_run = 'Run2018Comm'

samples18_data = [  'BTagMu_18',
                  ]
samples18_data_mcjp = [  'BTagMu_18mcjp',
                  ]
samples18_qcd = [   'QCD_Pt-170to300_MuEnrichedPt5_TuneCP5_13TeV_pythia8_18',
                    'QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8_18',
                    'QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8_18',
                    'QCD_Pt-600to800_MuEnrichedPt5_TuneCP5_13TeV_pythia8_18',
                    'QCD_Pt-800to1000_MuEnrichedPt5_TuneCP5_13TeV_pythia8_18',
                    'QCD_Pt-1000toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8_18',
                  ]

datasuff ="-17Nov2017-v1_v04_20190228"
qcdsuff ="_MuEnrichedPt5_TuneCP5_13TeV_pythia8_v04_20190228"
# v03
#datasuff ="-17Nov2017-v1_v03_20190222"
#qcdsuff ="_MuEnrichedPt5_TuneCP5_13TeV_pythia8_v03_20190222"
samples17_data = ['BTagMu_Run2017B'+datasuff,
                  'BTagMu_Run2017C'+datasuff,
                  'BTagMu_Run2017D'+datasuff,
                  'BTagMu_Run2017E'+datasuff,
                  'BTagMu_Run2017F'+datasuff,
                  ]
samples17_data_mcjp = samples17_data
samples17_qcd = [ 'QCD_Pt-170to300'+qcdsuff,
                  'QCD_Pt-300to470'+qcdsuff,
                  'QCD_Pt-470to600'+qcdsuff,
                  'QCD_Pt-600to800'+qcdsuff,
                  'QCD_Pt-800to1000'+qcdsuff,
                  'QCD_Pt-1000toInf'+qcdsuff,
                  ]

datasuff16 ="-17Jul2018-v1_9_4_X_v04"
qcdsuff16 ="_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8_9_4_X_v04"
samples16_data = ['BTagMu_Run2016B'+"-17Jul2018_ver2-v1_9_4_X_v04",
                  'BTagMu_Run2016C'+datasuff16,
                  'BTagMu_Run2016D'+datasuff16,
                  'BTagMu_Run2016E'+datasuff16,
                  'BTagMu_Run2016F'+datasuff16,
                  'BTagMu_Run2016G'+datasuff16,
                  'BTagMu_Run2016H'+datasuff16,
                  ]
samples16_data_mcjp = samples16_data
samples16_qcd = [ 'QCD_Pt-170to300'+qcdsuff16,
                  'QCD_Pt-300to470'+qcdsuff16,
                  'QCD_Pt-470to600'+qcdsuff16,
                  'QCD_Pt-600to800'+qcdsuff16,
                  'QCD_Pt-800to1000'+qcdsuff16,
                  'QCD_Pt-1000toInf'+qcdsuff16,
                  ]

# Source for config/general.py
info = {
}

# For each Run
for run_name, samples_data, samples_data_mcjp, samples_qcd in zip(run_names, [samples16_data, samples17_data, samples18_data], [samples16_data_mcjp, samples17_data_mcjp, samples18_data_mcjp], [samples16_qcd, samples17_qcd, samples18_qcd]):
  # For each tagger in campaign
  for tagger in taggers:
    # Fill basic dict
    run_name_tagger = run_name + tagger
    SF_output = run_name+'_'+tagger+'{}.root' # Keep {} for automatic sys name formatin
    info[run_name_tagger] = {
      # Dictionary of all samples with their list of subsamples. They are defined in samples.py
      'samples' :{},
      # Name of the root final with final histograms
      'final_output' : SF_output.format(""),
      # Dictionary of all variables that need to be changed for each campaign
      'btagvalidation_cfg'  : { },
    }

    # Fill samples and configs
    for name in samples_data + samples_qcd:
          info[run_name_tagger]['samples'][name] =  samples_info[name]['subsample'].keys()
          if name in samples_data: _runOnData = True
          else: _runOnData = False
          info[run_name_tagger]['btagvalidation_cfg'][name] = {
                'DEBUG'                      : False
                ,'runOnData'                 : _runOnData
                ,'fatJetPtMin'               : 350.
                ,'triggerSelection'          : string( ','.join(['HLT_BTagMu_AK8Jet300_Mu5' + "'", "'" + 'HLT_BTagMu_Jet300_Mu5']))
                ,'FilePUDistData'            : string( os.path.join( paths.main, 'aux', 'RunII2017Rereco_RunBCDEF_v1v2topUp_25ns_PUXsec69200nb_Feb8-2018.root'))
                ,'applyFatJetMuonTaggingV2'  : True  # For now necessary to run SF templates, should automate TODO
                ,'fatJetDoubleTagging'       : True  # For now necessary to run SF templates, should automate TODO
                ,'useSoftDropSubjets'        : True  # For now necessary to run SF templates, should automate TODO
                ,'produceDDXSFtemplates'     : True
                ,'chooseDDXtagger'           : string(tagger)
                }
          # Year specifics
          if "2016" in run_name:
            info[run_name_tagger]['btagvalidation_cfg'][name]['FilePUDistData'] = string( os.path.join( paths.main, 'aux', 'RunII2016BCDEFGH_PUXsec69200nb.root'))
            info[run_name_tagger]['btagvalidation_cfg'][name]['FilePUDistMC'] = string( os.path.join( paths.main, 'aux', 'PUDistMC_2016_25ns_PUDistMC_2016_25ns_Moriond17MC_PoissonOOTPU.root'))
          elif "2017" in run_name:
            info[run_name_tagger]['btagvalidation_cfg'][name]['FilePUDistData'] = string( os.path.join( paths.main, 'aux', 'RunII2017BCDEF_17Nov2017_25ns_PUXsec69200nb.root'))
            info[run_name_tagger]['btagvalidation_cfg'][name]['FilePUDistMC'] = string( os.path.join( paths.main, 'aux', 'PUDistMC_2017_25ns_WinterMC_PUScenarioV1_PoissonOOTPU.root'))
          elif "2018" in run_name:
            info[run_name_tagger]['btagvalidation_cfg'][name]['FilePUDistData'] = string( os.path.join( paths.main, 'aux', 'PUDistMC_2018_25ns_JuneProjectionFull18_PoissonOOTPU.root'))
            info[run_name_tagger]['btagvalidation_cfg'][name]['FilePUDistMC'] = string( os.path.join( paths.main, 'aux', 'RunII2018ABCD_25ns_PUXsec69200nb.root'))
                

    # To clone dictionary items
    from copy import deepcopy
    # Add systematics campaigns by cloning and only modding the proper 
    sys_name = "_BFRAGUP"
    # Clone default SF config with sys_name 
    info[run_name_tagger+sys_name] = deepcopy(info[run_name_tagger])
    # Change final_output name to include sys
    info[run_name_tagger+sys_name]['final_output'] = SF_output.format(sys_name)
    # Add sys specific configs
    for name in samples_data + samples_qcd:
      info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['doBFrag'] = True
      info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['doBFragUp'] = True

    sys_name = "_BFRAGDOWN"
    info[run_name_tagger+sys_name] = deepcopy(info[run_name_tagger])
    info[run_name_tagger+sys_name]['final_output'] = SF_output.format(sys_name)
    for name in samples_data + samples_qcd:
      info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['doBFrag'] = True
      info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['doBFragDown'] = True

    sys_name = "_PUUP"
    info[run_name_tagger+sys_name] = deepcopy(info[run_name_tagger])
    info[run_name_tagger+sys_name]['final_output'] = SF_output.format(sys_name)
    for name in samples_data + samples_qcd:
      if "2016" in run_name:
        info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['FilePUDistData'] = string( os.path.join( paths.main, 'aux', 'RunII2016BCDEFGH_PUXsec72450nb.root'))
      elif "2017" in run_name:
        info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['FilePUDistData'] = string( os.path.join( paths.main, 'aux', 'RunII2017BCDEF_17Nov2017_25ns_PUXsec72450nb.root'))
      elif "2018" in run_name:
        info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['FilePUDistData'] = string( os.path.join( paths.main, 'aux', 'RunII2018ABCD_25ns_PUXsec72450nb.root'))

    sys_name = "_PUDOWN"
    info[run_name_tagger+sys_name] = deepcopy(info[run_name_tagger])
    info[run_name_tagger+sys_name]['final_output'] = SF_output.format(sys_name)
    for name in samples_data + samples_qcd:
      if "2016" in run_name:
        info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['FilePUDistData'] = string( os.path.join( paths.main, 'aux', 'RunII2016BCDEFGH_PUXsec65550nb.root'))
      elif "2017" in run_name:
        info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['FilePUDistData'] = string( os.path.join( paths.main, 'aux', 'RunII2017BCDEF_17Nov2017_25ns_PUXsec65550nb.root'))
      elif "2018" in run_name:
        info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['FilePUDistData'] = string( os.path.join( paths.main, 'aux', 'RunII2018ABCD_25ns_PUXsec65550nb.root'))


    sys_name = "_CFRAG"
    info[run_name_tagger+sys_name] = deepcopy(info[run_name_tagger])
    info[run_name_tagger+sys_name]['final_output'] = SF_output.format(sys_name)
    for name in samples_data + samples_qcd:
      info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['doCFrag'] = True

    sys_name = "_CDFRAG"
    info[run_name_tagger+sys_name] = deepcopy(info[run_name_tagger])
    info[run_name_tagger+sys_name]['final_output'] = SF_output.format(sys_name)
    for name in samples_data + samples_qcd:
      info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['doCDFrag'] = True

    sys_name = "_K0L"
    info[run_name_tagger+sys_name] = deepcopy(info[run_name_tagger])
    info[run_name_tagger+sys_name]['final_output'] = SF_output.format(sys_name)
    for name in samples_data + samples_qcd:
      info[run_name_tagger+sys_name]["btagvalidation_cfg"][name]['doK0L'] = True

    sys_name = "_MCJP"
    info[run_name_tagger+sys_name] = deepcopy(info[run_name_tagger])
    info[run_name_tagger+sys_name]['final_output'] = SF_output.format(sys_name)
    for reg_data, mcjp_data in zip(samples_data, samples_data_mcjp):
      del info[run_name_tagger+sys_name]['samples'][reg_data] 
      info[run_name_tagger+sys_name]['samples'][mcjp_data] =  samples_info[mcjp_data]['subsample'].keys()
      info[run_name_tagger+sys_name]['btagvalidation_cfg'][mcjp_data] = info[run_name_tagger]['btagvalidation_cfg'][reg_data]
      del info[run_name_tagger+sys_name]['btagvalidation_cfg'][reg_data]


# FIll Comm Run
# for name in samples_data + samples_qcd:
#       info[Comm_run]['samples'][name] = ['0']
#       if name in samples_data: _runOnData = True
#       else: _runOnData = False
#       info[Comm_run]['btagvalidation_cfg'][name] = {
#             'DEBUG'                      : False
#             ,'runOnData'                 : _runOnData
#             ,'fatJetPtMin'               : 350.
#             ,'triggerSelection'          : string( ','.join(['HLT_BTagMu_AK8Jet300_Mu5' + "'", "'" + 'HLT_BTagMu_Jet300_Mu5']))
#             ,'FilePUDistData'            : string( os.path.join( paths.main, 'aux', 'RunII2017Rereco_RunBCDEF_v1v2topUp_25ns_PUXsec69200nb_Feb8-2018.root'))
#             ,'applyFatJetMuonTaggingV2'  : True  # For now necessary to run SF templates
#             ,'fatJetDoubleTagging'       : True  # For now necessary to run SF templates
#             ,'useSoftDropSubjets'        : True  # For now necessary to run SF templates
#             ,'produceDeepDoubleXCommissioning' : False
#             }

if __name__ == "__main__":
  import pprint
  pprint.pprint(info, depth=1)

