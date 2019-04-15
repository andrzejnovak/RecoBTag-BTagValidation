import os
import paths

def string(txt):
  ''' str wrapper since cmsRun/python requires string to have "". '''

  ''' RECIPE: If working with special flags e.g.
  'triggerSelection'          : string( ','.join(['HLT_BTagMu_AK8Jet300_Mu5' + "'", "'" + 'HLT_BTagMu_Jet300_Mu5']))
  '''
  return txt.join(["'", "'"])

##############################

SF_run = 'Run2018SF'
Comm_run = 'Run2018Comm'

samples_data = [  'BTagMu',
                  #'BTagMu_Run2018F-17Nov2018-v1_v04_20190228'
                  ]
samples_qcd = [   'QCD_Pt-170to300_MuEnrichedPt5_TuneCP5_13TeV_pythia8',
                    'QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8',
                    'QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8',
                    'QCD_Pt-600to800_MuEnrichedPt5_TuneCP5_13TeV_pythia8',
                    'QCD_Pt-800to1000_MuEnrichedPt5_TuneCP5_13TeV_pythia8',
                    'QCD_Pt-1000toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8',
                  ]
# Source for config/general.py
info = {
  SF_run: {
    # Dictionary of all samples with their list of subsamples. They are defined in samples.py
    'samples' :{},
    # Name of the root final with final histograms
    'final_output' : 'Run2017_QCDMuEnriched_DoubleMuonTaggedFatJets_histograms_btagval.root',
    # Dictionary of all variables that need to be changed for each campaign
    'btagvalidation_cfg'  : { },
  },
  Comm_run: {
    # Dictionary of all samples with their list of subsamples. They are defined in samples.py
    'samples' :{},
    # Name of the root final with final histograms
    #'final_output' : 'Run2016_QCDMuEnriched_DoubleMuonTaggedFatJets_histograms_btagval.root',
    'final_output' : 'Run2018_QCDMuEnriched_DoubleMuonTaggedFatJets_histograms_btagval.root',
    # Dictionary of all variables that need to be changed for each campaign
    'btagvalidation_cfg'  : { },
  },
}

# FIll SF Run
for name in samples_data + samples_qcd:
      info[SF_run]['samples'][name] = ['0']
      if name in samples_data: _runOnData = True
      else: _runOnData = False
      info[SF_run]['btagvalidation_cfg'][name] = {
            'DEBUG'                      : False
            ,'runOnData'                 : _runOnData
            ,'fatJetPtMin'               : 350.
            ,'triggerSelection'          : string( ','.join(['HLT_BTagMu_AK8Jet300_Mu5' + "'", "'" + 'HLT_BTagMu_Jet300_Mu5']))
#            ,'FilePUDistData'            : string( os.path.join( paths.main, 'aux', 'RunII2018Rereco_RunBCDEF_v1v2topUp_25ns_PUXsec69200nb_Feb8-2018.root'))
            ,'FilePUDistData'            : string( os.path.join( paths.main, 'aux', 'RunII2016BCDEFGH_PUXsec69200nb.root'))
            ,'produceDDXSFtemplates'     : True
            ,'chooseDDXtagger'           : string('DDBvL')
            }


# FIll Comm Run
for name in samples_data + samples_qcd:
      info[Comm_run]['samples'][name] = ['0']
      if name in samples_data: _runOnData = True
      else: _runOnData = False
      info[Comm_run]['btagvalidation_cfg'][name] = {
            'DEBUG'                      : False
            ,'runOnData'                 : _runOnData
            ,'fatJetPtMin'               : 350.
            ,'doFatJetPtReweighting'     : True
            ,'applyFatJetMuonTaggingV2'  : True
            ,'triggerSelection'          : string( ','.join(['HLT_BTagMu_AK8Jet300_Mu5' + "'", "'" + 'HLT_BTagMu_Jet300_Mu5']))
            #,'FilePUDistMC'            : string( os.path.join( paths.main, 'aux', 'PUDistMC_2016_25ns_PUDistMC_2016_25ns_Moriond17MC_PoissonOOTPU.root'))
            #,'FilePUDistData'            : string( os.path.join( paths.main, 'aux', 'RunII2016BCDEFGH_PUXsec69200nb.root'))
            ,'FilePUDistMC'            : string( os.path.join( paths.main, 'aux', 'PUDistMC_2018_25ns_WinterMC_PUScenarioV1_PoissonOOTPU.root'))
            ,'FilePUDistData'            : string( os.path.join( paths.main, 'aux', 'RunII2018BCDEF_17Nov2018_25ns_PUXsec69200nb.root'))
            ,'produceDeepDoubleXCommissioning' : True
            }

