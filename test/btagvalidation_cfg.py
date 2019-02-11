import sys
import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
from pdb import set_trace

options = VarParsing('python')

options.register('outFilename', 'bTagValPlots.root',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Output file name"
    )
options.register('reportEvery', 1000,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "Report every N events (default is N=1000)"
    )
options.register('triggerSelection', '',
    VarParsing.multiplicity.list,
    VarParsing.varType.string,
    "Trigger selection"
    )
options.register('useJetProbaTree', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Use jet probability tree"
    )
options.register('applyFatJetMuonTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply muon tagging to fat jets (require 1 muon in fatjet)"
    )
options.register('applyFatJetBTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply b tagging to fat jets"
    )
options.register('fatJetDoubleTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Require fat jets to be double-tagged"
    )
options.register('fatJetDoubleBTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Require fat jets to be double-b-tagged"
    )
options.register('fatJetDoubleSVBTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Require fat jets to be double-SV-b-tagged"
    )
options.register('usePrunedSubjets', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Process pruned subjets"
    )
options.register('useSoftDropSubjets', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Process soft drop subjets"
    )
options.register('applySubJetMuonTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply muon tagging to subjets"
    )
options.register('applySubJetBTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply b tagging to subjets"
    )
options.register('dynamicMuonSubJetDR', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Use dynamic muon-subjet dR requirement"
    )
options.register('useFlavorCategories', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Use flavor categories for MC distributions"
    )
options.register('useRelaxedMuonID', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Use relaxed muon ID"
    )
options.register('fatJetAbsEtaMax', 2.5,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Maximum abs(eta)"
    )
options.register('fatJetPtMin', 350.,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Minimum fat jet Pt"
    )
options.register('fatJetPtMax', 1.E6,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Maximum fat jet Pt"
    )
options.register('fatJetSoftDropMassMin', 0.,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Minimum fat jet softdrop mass"
    )
options.register('fatJetSoftDropMassMax', 1.E6,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Maximum fat jet softdrop mass"
    )
options.register('fatJetTau21Min', 0.0,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "tau2/tau1 jet substructure min cut for fat jets"
    )
options.register('fatJetTau21Max', 0.6,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "tau2/tau1 jet substructure max cut for fat jets"
    )
options.register('fatJetBDiscrCut', 0.5426,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "B discriminator cut for fat jets"
    )
options.register('fatJetDoubleSVBDiscrMin', -0.20,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Double SV b discriminator cut for fat jets"
    )
options.register('fatJetDoubleSVBDiscrMax', 1.0,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Double SV b discriminator cut for fat jets"
    )
options.register('subJetBDiscrMin', 0.5426,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "B discriminator min for subjets"
    )
options.register('subJetBDiscrMax', 1.000,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "B discriminator max for subjets"
    )
options.register('SFbShift', 0.,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Shift in SFb in units of sigmas"
    )
options.register('SFlShift', 0.,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Shift in SFl in units of sigmas"
    )
options.register('MuonJetPtRatio', 0.5,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "pT(muon)/pT(Jet) for Muon Tagging"
    )
options.register('doPUReweightingOfficial', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Do pileup reweighting"
    )
options.register('doPUReweightingNPV', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Do pileup reweighting"
    )
options.register('doFatJetPtReweighting', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Do fat jet pt reweighting'
    )
options.register('doGSPUp', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Do GSP rate uncertainty (+25%)'
    )
options.register('doGSPDown', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Do GSP rate uncertainty (-25%)'
    )
options.register('doBFrag', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Do b fragmentation reweighting'
    )
options.register('doBFragUp', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Do b fragmentation reweighting up'
    )
options.register('doBFragDown', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Do b fragmentation reweighting down'
    )
options.register('doCDFrag', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Do c->D fragmentation reweighting'
    )
options.register('doCFrag', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Do c fragmentation reweighting'
    )
options.register('doK0L', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Do K0 and lambda reweighting'
    )
options.register('removeJP0', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Remove jets with JP = -0'
    )
options.register('doSubJetPtReweighting', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'Do subjet pt reweighting'
    )
options.register('applySFs', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply b-tagging SFs"
    )
options.register('btagCSVFile', 'CSVv2.csv',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "CSV file containing b-tagging SFs"
    )
options.register('btagOperatingPoint', 0,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "B-tagging opertaint point for CSVv2. Set to 'loose' by default"
    )
options.register('btagMeasurementType', "comb",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Measurement type: combined or ttbar"
    )
options.register('btagSFType', "central",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply b-tagging SFs"
    )
options.register('FileFatJetPtWt', "/afs/cern.ch/user/d/devdatta/afswork/CMSREL/BTagging/CMSSW_8_0_23/src/RecoBTag/BTagValidation/test/jetpt_data_weight_fatJetPtMin_450_tau-0p5/fatjetpt_DataRun2016Rereco_MC_RunIISummer16_25ns_MINIAOD.root",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "File with data/MC weights for fat jet pT reweighting"
    )
options.register('FileSubJetPtWt', "/afs/cern.ch/user/d/devdatta/afswork/CMSREL/BTagging/CMSSW_9_4_1/src/RecoBTag/BTagValidation/test/SoftDropSubJetPt_data_mc_SubJetMuTagged_QCDMuEnriched_94XMiniAOD.root",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "File with data/MC weights for subjet pT reweighting"
    )
options.register('newJECPayloadNames',
    "",
    VarParsing.multiplicity.list,
    VarParsing.varType.string,
    "New JEC payload names"
    ),
options.register('jecUncPayloadName',
    "/afs/cern.ch/user/d/devdatta/afswork/CMSREL/BTagging/CMSSW_8_0_23/src/RecoBTag/BTagValidation/test/Summer16_23Sep2016V3_MC/Summer16_23Sep2016V3_MC_Uncertainty_AK4PFchs.txt",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "JEC uncertainty payload name"
    ),
options.register('doNewJEC', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply new JECs"
    )
options.register('doJECUncert', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Do JEC uncertainty"
    )
options.register('jecshift', 0,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "Shift in JEC"
    )
options.register('useRunRange', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Restrict by run ranges (only for data)"
    )
options.register('runRangeMin', 200000,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "Min Run number"
    )
options.register('runRangeMax', 999999,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "Max Run number"
    )
options.register('groups', [],
    VarParsing.multiplicity.list,
    VarParsing.varType.string,
    'variable groups to be stored')
options.register('runJetVariables', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    'True if you want to run Jet Variables')
options.register('runEventInfo', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Run Event Info"
    )


options.parseArguments()

if options.doJECUncert and options.jecshift==0:
  sys.exit("!!!!ERROR: JEC uncertainty selected byt jecshift not set. Set jecshift either to -1 or 1\n")

if options.usePrunedSubjets and options.useSoftDropSubjets:
  print("Warning: both pruned and soft drop subjets selected. Only pruned subjets will be processed.")
  print("!!!Select either pruned subjets with 'usePrunedSubjets' or soft drop subjets with 'useSoftDropSubjets'.")
elif not options.usePrunedSubjets and not options.useSoftDropSubjets:
  print("!!!Warning: no subjets will be processed.!!!")
  print("!!!Select either pruned subjets with 'usePrunedSubjets' or soft drop subjets with 'useSoftDropSubjets'.")

# print options

process = cms.Process("BTagVal")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string('INFO'),
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) ) # Keep as such

process.source = cms.Source("EmptySource")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outFilename)
    )

from inputFiles_cfi import *
from RecoBTag.PerformanceMeasurements.bTagAnalyzerCommon_cff import *

#print(bTagAnalyzerCommon.TriggerPathNames)
from RecoBTag.PerformanceMeasurements.variables_cfi import *
from RecoBTag.PerformanceMeasurements.varGroups_cfi import *

print("!!!! Opening files {}".format(FileNames))

varList = []
for groupToRead in options.groups:
  for groupExisting in groupSet.groups:
    if groupToRead == groupExisting.group:
      for varname in groupExisting.variables:
        if "FatJetInfo." in varname:
          shortvarname = varname.split(".")[1]
          var = variableDict[shortvarname].clone()
          var.variable = varname
        elif "SubJetInfo." in varname:
          shortvarname = varname.split(".")[1]
          var = variableDict[shortvarname].clone()
          var.variable = varname
        else:
          var = variableDict[varname].clone()
        var.store = cms.bool(True)
        varList.append(var)
variableToRead = cms.VPSet(varList)

process.btagval = cms.EDAnalyzer('BTagValidation',
    MaxEvents              = cms.int32(options.maxEvents),
    ReportEvery            = cms.int32(options.reportEvery),
    UseJetProbaTree        = cms.bool(options.useJetProbaTree),
    InputTTreeEvtInfo      = cms.string('btaganaFatJets/ttree'),
    InputTTree             = cms.string('btaganaFatJets/ttree'),
    InputFiles             = cms.vstring(FileNames),
    #InputFiles             = cms.vstring(FileNames_QCD_MuEnriched_800to1000),
    #InputFiles             = cms.vstring(FileNames_QCD_Pt_1000toInf),
    UseFlavorCategories    = cms.bool(options.useFlavorCategories),
    UseRelaxedMuonID       = cms.bool(options.useRelaxedMuonID),
    runEventInfo           = cms.bool(True),
    runJetVariables        = cms.bool(True),
    ApplyFatJetMuonTagging = cms.bool(options.applyFatJetMuonTagging),
    ApplyFatJetBTagging    = cms.bool(options.applyFatJetBTagging),
    FatJetDoubleTagging    = cms.bool(options.fatJetDoubleTagging),
    FatJetDoubleBTagging   = cms.bool(options.fatJetDoubleBTagging),
    FatJetDoubleSVBTagging = cms.bool(options.fatJetDoubleSVBTagging),
    UsePrunedSubjets       = cms.bool(options.usePrunedSubjets),
    UseSoftDropSubjets     = cms.bool(options.useSoftDropSubjets),
    ApplySubJetMuonTagging = cms.bool(options.applySubJetMuonTagging),
    ApplySubJetBTagging    = cms.bool(options.applySubJetBTagging),
    DynamicMuonSubJetDR    = cms.bool(options.dynamicMuonSubJetDR),
    FatJetBDiscrCut        = cms.double(options.fatJetBDiscrCut),
    FatJetDoubleSVBDiscrMin= cms.double(options.fatJetDoubleSVBDiscrMin),
    FatJetDoubleSVBDiscrMax= cms.double(options.fatJetDoubleSVBDiscrMax),
    SubJetBDiscrMin        = cms.double(options.subJetBDiscrMin),
    SubJetBDiscrMax        = cms.double(options.subJetBDiscrMax),
    FatJetPtMin            = cms.double(options.fatJetPtMin),
    FatJetPtMax            = cms.double(options.fatJetPtMax),
    FatJetSoftDropMassMin  = cms.double(options.fatJetSoftDropMassMin),
    File_PVWt              = cms.string(''),
    Hist_PVWt              = cms.string('hpvwt_data_mc'),
    File_PUDistMC          = cms.string('/afs/cern.ch/user/d/devdatta/afswork/CMSREL/BTagging/CMSSW_9_4_1/src/RecoBTag/BTagValidation/test/PUDist/PUDistMC_2017_25ns_WinterMC_PUScenarioV1_PoissonOOTPU.root'),
    Hist_PUDistMC          = cms.string('pileup'),
    File_PUDistData        = cms.string('/afs/cern.ch/user/d/devdatta/afswork/CMSREL/BTagging/CMSSW_9_4_1/src/RecoBTag/BTagValidation/test/PUDist/RunII2017BCDEF_17NovRereco_25ns_PUXsec69200nb.root'),
    Hist_PUDistData        = cms.string('pileup'),
    File_FatJetPtWt        = cms.string(options.FileFatJetPtWt),
    Hist_FatJetPtWt        = cms.string('fatjetptweight_mc_data'),
    File_SubJetPtWt        = cms.string(options.FileSubJetPtWt),
    Hist_SubJetPtWt        = cms.string('jetptweight_mc_data'),
    FatJetSoftDropMassMax  = cms.double(options.fatJetSoftDropMassMax),
    FatJetTau21Min         = cms.double(options.fatJetTau21Min), #added by rizki
    FatJetTau21Max         = cms.double(options.fatJetTau21Max), #added by rizki
    FatJetAbsEtaMax        = cms.double(options.fatJetAbsEtaMax), #added by rizki
    SFbShift               = cms.double(options.SFbShift),
    SFlShift               = cms.double(options.SFlShift),
    MuonJetPtRatio         = cms.double(options.MuonJetPtRatio),
    DoPUReweightingOfficial= cms.bool(options.doPUReweightingOfficial),
    DoPUReweightingNPV     = cms.bool(options.doPUReweightingNPV),
    DoFatJetPtReweighting  = cms.bool(options.doFatJetPtReweighting),
    DoGSPUp                = cms.bool(options.doGSPUp),
    DoGSPDown              = cms.bool(options.doGSPDown),
    DoBFrag                = cms.bool(options.doBFrag),
    DoBFragUp              = cms.bool(options.doBFragUp),
    DoBFragDown            = cms.bool(options.doBFragDown),
    DoCDFrag               = cms.bool(options.doCDFrag),
    DoCFrag                = cms.bool(options.doCFrag),
    DoK0L                  = cms.bool(options.doK0L),
    RemoveJP0              = cms.bool(options.removeJP0),
    DoSubJetPtReweighting  = cms.bool(options.doSubJetPtReweighting),
    TriggerSelection       = cms.vstring( # OR of all listed triggers applied, empty list --> no trigger selection applied
      options.triggerSelection
      ),
    TriggerPathNames       = bTagAnalyzerCommon.TriggerPathNames,
    #TriggerPathNames       = cms.vstring(
    #  "HLT_BTagMu_Jet300_Mu5_v",
    #  "HLT_BTagMu_AK8Jet300_Mu5_v",
    #  "HLT_PFJet200_v",
    #  "HLT_PFJet260_v",
    #  "HLT_PFJet320_v",
    #  "HLT_PFJet400_v",
    #  "HLT_PFJet450_v",
    #  "HLT_PFJet500_v",
    #  ),
    ApplySFs               = cms.bool(options.applySFs),
    btagCSVFile            = cms.string(options.btagCSVFile),
    btagOperatingPoint     = cms.int32(options.btagOperatingPoint),
    btagMeasurementType    = cms.string(options.btagMeasurementType),
    btagSFType             = cms.string(options.btagSFType),
    newJECPayloadNames     = cms.vstring(options.newJECPayloadNames),
    jecUncPayloadName      = cms.string(options.jecUncPayloadName),
    doNewJEC               = cms.bool(options.doNewJEC),
    doJECUncert            = cms.bool(options.doJECUncert),
    jecshift               = cms.int32(options.jecshift),
    useRunRange            = cms.bool(options.useRunRange),
    runRangeMin            = cms.int32(options.runRangeMin),
    runRangeMax            = cms.int32(options.runRangeMax),
    variables              = variableToRead,
    groups                 = groupSet.groups
)

process.p = cms.Path(process.btagval)

open('dump.py', 'w').write(process.dumpPython())
