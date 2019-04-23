import os
import paths
# NOTE: all configs will be saved in btagvalidation_cfg.py template for every job. Idea is to have only ONE file for definitions.
# If you want some special config for each sample you can do it from the config file samples.py.

def string(txt):
  ''' str wrapper since cmsRun/python requires string to have "". '''
  return txt.join(["'", "'"])

parameters = {
  'outFilename'               : string(''),             # Output file name
  'maxEvents'                 : -1,
  'reportEvery'               : 1000,                   # Report every N events (default is N=1000)
  'DEBUG'                     : False,                   # Display debugging statements
  'DEBUGlevel'                : 0,                      # Debugging statements level
  'triggerSelection'          : string( ','.join(['HLT_BTagMu_AK8Jet300_Mu5' + "'", "'" + 'HLT_BTagMu_Jet300_Mu5'])), # Trigger selection
  'useJetProbaTree'           : False,                   # Use jet probability tree
  'applyFatJetMuonTagging'    : False,                  # Apply muon tagging to fat jets (require 1 muon in fatjet)
  'applyFatJetMuonTaggingV2'  : True,                   # Apply muon tagging to fat jets (require at least 1 muon in a subjet)
  'applyFatJetBTagging'       : False,                  # Apply b tagging to fat jets
  'fatJetDoubleTagging'       : True,                  # Require fat jets to be double-tagged
  'fatJetDoubleBTagging'      : False,                  # Require fat jets to be double-b-tagged
  'fatJetDoubleSVBTagging'    : False,                  # Require fat jets to be double-SV-b-tagged
  'usePrunedSubjets'          : False,                  # Process pruned subjets
  'useSoftDropSubjets'        : True,                   # Process soft drop subjets
  'applySubJetMuonTagging'    : False,                  # Apply muon tagging to subjets
  'applySubJetBTagging'       : False,                  # Apply b tagging to subjets
  'dynamicMuonSubJetDR'       : False,                  # Use dynamic muon-subjet dR requirement
  'useFlavorCategories'       : True,                   # Use flavor categories for MC distributions
  'useRelaxedMuonID'          : True,                   # Use relaxed muon ID
  'muonPtMin'                 : 7.,                     # Minimum Muon Pt
  'fatJetAbsEtaMax'           : 2.4,                    # Maximum abs(eta)
  'fatJetPtMin'               : 350.,                   # Minimum fat jet Pt
  'fatJetPtMax'               : 1.E6,                   # Maximum fat jet Pt
  'removeProblemJet'          : True,                   # Remove problematic jets from low pT MC bins
  'usePrunedMass'             : False,                   # Use pruned mass cut
  'useSoftDropMass'           : True,                  # Use softdrop mass cut
  'fatJetGroomedMassMin'      : 50.,                     # Minimum fat jet softdrop/pruned mass
  'fatJetGroomedMassMax'      : 1.E6,                   # Maximum fat jet softdrop/pruned mass
  'fatJetTau21Min'            : 0.0,                    # added by rizki"tau2/tau1 jet substructure min cut for fat jets
  'fatJetTau21Max'            : 1.0,                    # added by rizki"tau2/tau1 jet substructure max cut for fat jets
  'fatJetBDiscrCut'           : 0.244,                  # B discriminator cut for fat jets
  'fatJetDoubleSVBDiscrMin'   : -0.20,                  # Double SV b discriminator cut for fat jets
  'fatJetDoubleSVBDiscrMax'   : 1.0,                    # Double SV b discriminator cut for fat jets
  'subJetBDiscrMin'           : 0.605,                  # B discriminator min for subjets
  'subJetBDiscrMax'           : 1.000,                  # B discriminator max for subjets
  'SFbShift'                  : 0.,                     # Shift in SFb in units of sigmas
  'SFlShift'                  : 0.,                     # Shift in SFl in units of sigmas
  'MuonJetPtRatio'            : 0.5,                    # pT(muon)/pT(Jet) for Muon Tagging
  'DiMuonJetPtRatio'          : 0.6,                    # pT(muon1+muon2)/pT(Jet) for Double Muon Tagging
  'doPUReweightingOfficial'   : True,                   # Do pileup reweighting
  'FilePUDistData'            : string( os.path.join( paths.main, 'aux', 'RunII2017Rereco_RunBCDEF_v1v2topUp_25ns_PUXsec69200nb_Feb8-2018.root')),   # File for data/MC weights for PU reweight (official)
  'Hist_PUDistData'           : string('pileup'),
  'DoPUReweightingNPV'        : False,                  # Do pileup reweighting
  'doFatJetPtReweighting'     : False,                   # Do fat jet pt reweighting
  'doNtracksReweighting'      : False,                  # Do ntracks reweightin
  'doBFrag'                   : False,                  # Do b fragmentation reweighting
  'doBFragUp'                 : False,                  # Do b fragmentation reweighting up
  'doBFragDown'               : False,                  # Do b fragmentation reweighting down
  'doCDFrag'                  : False,                  # Do c->D fragmentation reweighting)
  'doCFrag'                   : False,                  # Do c fragmentation reweighting
  'doK0L'                     : False,                  # Do K0 and lambda reweighting
  'doSubJetPtReweighting'     : False,                  # Do subjet pt reweighting
  'doSubJetPtBalanceReweighting':False,                 # Do subjet pt balance reweighting
  'doMassSoftDropReweighting' : False,                  # Do soft drop reweighting
  'doJetNTracksReweighting'   : False,                  # Do jetNTracks reweighting
  'doSV1EnergyRatioReweighting':False,                  # Do SV1 energy ratio reweighting
  'doIPSig1stAboveBReweighting':False,                  # Do IP sig 1st Track above bottom reweighting
  'doZratioReweighting'       : False,                  # Do Zatio reweighting
  'applySFs'                  : False,                  # Apply b-tagging SFs
  'btagCSVFile'               : string( os.path.join( paths.main, 'aux', 'CSVv2_4invfb.csv')),     # CSV file containing b-tagging SFs
  'btagOperatingPoint'        : 0,                      # B-tagging opertaint point for CSVv2. Set to loose by default
  'btagMeasurementType'       : string('comb'),         # Measurement type:  combined or ttbar
  'btagSFType'                : string('central'),              # Apply b-tagging SFs
  'FileFatJetPtWt'            : string( os.path.join( paths.main, 'aux', 'dataMC_weight_calc/FatJet_pt_all_data_mc_weight_Mu_350.root')),                # File with data/MC weights for fat jet pT reweighting
  'HistFatJetPtWt'            : string('FatJet_pt_all_weight_data_mc'),
  'FileNtracksWt'             : string( os.path.join( paths.main, 'aux', 'dataMC_weight_calc/FatJet_track_multi_data_mc_weight_Mu_350.root')),           # File with data/MC weights for fat jet ntracks reweighting
  'HistNtracksWt'             : string('FatJet_track_multi_weight_data_mc'),
  # This file doesn't exist
  'FileSubJetPtWt'            : string( os.path.join( paths.main, 'aux', 'SoftDropSubJetPt_data_mc_DoubleMuonTagged_QCDMuEnriched_76XMiniAODv2.root')),  # File with data/MC weights for subjet pT reweighting
  'HistSubJetPtWt'            : string('jetptweight_mc_data'),
  # This file doesn't exist
  'FileSV1EnergyRatioWt'      : string( os.path.join( paths.main, 'aux', 'SV1EnergyRatio_Hbb_QCDbb_pt425_Double_JetTight_weight.root')),                 # File with data/MC weights for SV1EnergyRatio reweighting
  'HistSV1EnergyRatioWt'      : string('SV1EnergyRatioweight_mc_data'),
  # This file doesn't exist
  'FileIPSig1stAboveBWt'      : string( os.path.join( paths.main, 'aux', 'IPSig1stAboveB_Hbb_QCDbb_pt425_weight.root')),                                 # File with data/MC weights for IPSig1stAboveB reweighting
  'HistIPSig1stAboveBWt'      : string('IPSig1stAboveBweight_mc_data'),
  # This file doesn't exist
  'FileZratioWt'              : string( os.path.join( paths.main, 'aux', 'ZratioRatio_Hbb_QCDbb_pt425_Double_JetTight_weight.root')),                    # File with data/MC weights for Zratio reweighting
  'HistZratioWt'              : string('Zratioweight_mc_data'),
  'newJECPayloadNames'        : string(','.join([                                                                                                                                # New JEC payload names
                                os.path.join( paths.main, 'aux', 'JECfiles/Summer16_23Sep2016V3_MC_L1FastJet_AK8PFchs.txt') + "'",
                                "'" + os.path.join( paths.main, 'aux', 'JECfiles/Summer16_23Sep2016V3_MC_L2Relative_AK8PFchs.txt') + "'",
                                "'" + os.path.join( paths.main, 'aux', 'JECfiles/Summer16_23Sep2016V3_MC_L3Absolute_AK8PFchs.txt')]
                                )),
  'jecUncPayloadName'         : string(os.path.join( paths.main, 'aux', 'JECfiles/Summer16_23Sep2016V3_MC_Uncertainty_AK8PFchs.txt')),                  # JEC uncertainty payload name
  'doNewJEC'                  : False,                   # Apply new JECs
  'doJECUncert'               : True,                  # Do JEC uncertainty, produced in parallel, safe to keep True
  # This file/path doesn't exist
  'FileBFrag'                 : string( os.path.join( paths.main, 'aux', 'PtRelFall12')), # File path for doBFrag systematics
  'FilePVWt'                  : string(''),
  'HistPVWt'                  : string(''),
  'FilePUDistMC'              : string( os.path.join( paths.main, 'aux', 'PUDistMC_2017_25ns_WinterMC_PUScenarioV1_PoissonOOTPU.root')),
  'HistPUDistMC'              : string('pileup'),
  'FileSubJetPtBalanceWt'     : string('subjetptbalance_Hbb_QCDbb_pt425_weight.root'),
  'HistSubJetPtBalanceWt'     : string('subjetptbalanceweight_mc_data'),
  'FileMassSoftDropWt'        : string('massSoftDrop_Hbb_QCDbb_pt425_weight.root'),
  'HistMassSoftDropWt'        : string('massSoftDropweight_mc_data'),
  'FileJetNTracksWt'          : string('jetNTracks_Hbb_QCDbb_pt425_weight.root'),
  'jetNTracksweight_mc_data'  : string('jetNTracksweight_mc_data'),
  'triggerLogicIsOR'          : True,
  'produceDoubleBCommissioning' : False,
  'produceDeepDoubleXCommissioning' : False,
  'produceDDXSFtemplates'     : False,
  'DDXWPFile'                   : string( os.path.join( paths.main, 'aux', 'DDX.json')),
  'chooseDDXtagger'           : string('DDBvL'),
  'produceDoubleBSFtemplates' : False,  #use JP
  'produceDoubleBSFtemplatesV2': False, #use SVmass
  'produceDoubleBSFtemplates_JPhasSV': False, #use JPhasSV
  'produceDoubleBSFtemplates_JPnoSV': False, #use JPnoSV
  'useRunRange'               : False,
  'runRangeMin'               : 0,
  'runRangeMax'               : 999999,
  'runOnData'                 : False,
}
