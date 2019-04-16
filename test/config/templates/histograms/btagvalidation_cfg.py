import FWCore.ParameterSet.Config as cms
from RecoBTag.PerformanceMeasurements.bTagAnalyzerCommon_cff import *
from RecoBTag.PerformanceMeasurements.variables_cfi import *
from RecoBTag.PerformanceMeasurements.varGroups_cfi import *

process  = cms.Process("BTagVal")

# uncomment below to turn on MSG
# process.load("FWCore.MessageService.MessageLogger_cfi")
# process.MessageLogger.cout = cms.untracked.PSet(
#     threshold = cms.untracked.string('INFO'),
#     )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1) # Keep as such
    )

process.source = cms.Source("EmptySource")

# # Output file
# ext=""
# if options.usePrunedSubjets:
#  ext="_WithPrunedSubjets"
# elif options.useSoftDropSubjets:
#  ext="_withSoftDropSubjets"
# outFilename = options.outFilename+ext+".root"

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(<output_file>)
    )

varList = []
for groupToRead in <btagAna_groups>:
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
    DEBUG                           = cms.bool(<DEBUG>),
    DEBUGlevel                      = cms.int32(<DEBUGlevel>),
    MaxEvents                       = cms.int32(<maxEvents>),
    ReportEvery                     = cms.int32(<reportEvery>),
    UseJetProbaTree                 = cms.bool(<useJetProbaTree>),
    InputTTreeEvtInfo               = cms.string('btaganaFatJets/ttree'),
    InputTTree                      = cms.string('btaganaFatJets/ttree'),
    InputFiles                      = cms.vstring(<input_files>),
    UseFlavorCategories             = cms.bool(<useFlavorCategories>),
    UseRelaxedMuonID                = cms.bool(<useRelaxedMuonID>),
    ApplyFatJetMuonTagging          = cms.bool(<applyFatJetMuonTagging>),
    ApplyFatJetMuonTaggingV2        = cms.bool(<applyFatJetMuonTaggingV2>),
    ApplyFatJetBTagging             = cms.bool(<applyFatJetBTagging>),
    FatJetDoubleTagging             = cms.bool(<fatJetDoubleTagging>),
    FatJetDoubleBTagging            = cms.bool(<fatJetDoubleBTagging>),
    FatJetDoubleSVBTagging          = cms.bool(<fatJetDoubleSVBTagging>),
    UsePrunedSubjets                = cms.bool(<usePrunedSubjets>),
    UseSoftDropSubjets              = cms.bool(<useSoftDropSubjets>),
    ApplySubJetMuonTagging          = cms.bool(<applySubJetMuonTagging>),
    ApplySubJetBTagging             = cms.bool(<applySubJetBTagging>),
    DynamicMuonSubJetDR             = cms.bool(<dynamicMuonSubJetDR>),
    FatJetBDiscrCut                 = cms.double(<fatJetBDiscrCut>),
    FatJetDoubleSVBDiscrMin         = cms.double(<fatJetDoubleSVBDiscrMin>),
    FatJetDoubleSVBDiscrMax         = cms.double(<fatJetDoubleSVBDiscrMax>),
    SubJetBDiscrMin                 = cms.double(<subJetBDiscrMin>),
    SubJetBDiscrMax                 = cms.double(<subJetBDiscrMax>),
    MuonPtMin                       = cms.double(<muonPtMin>),
    FatJetPtMin                     = cms.double(<fatJetPtMin>),
    FatJetPtMax                     = cms.double(<fatJetPtMax>),
    RemoveProblemJet                = cms.bool(<removeProblemJet>),
    UsePrunedMass                   = cms.bool(<usePrunedMass>),
    UseSoftDropMass                 = cms.bool(<useSoftDropMass>),
    FatJetGroomedMassMin            = cms.double(<fatJetGroomedMassMin>),
    FatJetGroomedMassMax            = cms.double(<fatJetGroomedMassMax>),
    File_PVWt                       = cms.string(<FilePVWt>),
    Hist_PVWt                       = cms.string(<HistPVWt>),
    File_PUDistMC                   = cms.string(<FilePUDistMC>),
    Hist_PUDistMC                   = cms.string(<HistPUDistMC>),
    File_PUDistData                 = cms.string(<FilePUDistData>),
    Hist_PUDistData                 = cms.string(<Hist_PUDistData>),
    File_FatJetPtWt                 = cms.string(<FileFatJetPtWt>),
    Hist_FatJetPtWt                 = cms.string(<HistFatJetPtWt>),
    File_NtracksWt                  = cms.string(<FileNtracksWt>),
    Hist_NtracksWt                  = cms.string(<HistNtracksWt>),
    File_SubJetPtWt                 = cms.string(<FileSubJetPtWt>),
    Hist_SubJetPtWt                 = cms.string(<HistSubJetPtWt>),
    File_SubJetPtBalanceWt          = cms.string(<FileSubJetPtBalanceWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for subjet pt balance reweighting.
    Hist_SubJetPtBalanceWt          = cms.string(<HistSubJetPtBalanceWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for subjet pt balance reweighting.
    File_MassSoftDropWt             = cms.string(<FileMassSoftDropWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for mass reweighting.
    Hist_MassSoftDropWt             = cms.string(<HistMassSoftDropWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for mass reweighting.
    File_JetNTracksWt               = cms.string(<FileJetNTracksWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for jetNTracks reweighting.
    Hist_JetNTracksWt               = cms.string(<jetNTracksweight_mc_data>), #added by rizki for Hbb tagger signal vs proxy studies. File for jetNTracks reweighting.
    File_SV1EnergyRatioWt           = cms.string(<FileSV1EnergyRatioWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for SV1 energy ratio reweighting.
    Hist_SV1EnergyRatioWt           = cms.string(<HistSV1EnergyRatioWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for SV1 energy ratio reweighting.
    File_IPSig1stAboveBWt           = cms.string(<FileIPSig1stAboveBWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for IPSig1stAboveB reweighting.
    Hist_IPSig1stAboveBWt           = cms.string(<HistIPSig1stAboveBWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for IPSig1stAboveB reweighting.
    File_ZratioWt                   = cms.string(<FileZratioWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for Zratio reweighting.
    Hist_ZratioWt                   = cms.string(<HistZratioWt>), #added by rizki for Hbb tagger signal vs proxy studies. File for Zratio reweighting.
    FatJetTau21Min                  = cms.double(<fatJetTau21Min>), #added by rizki
    FatJetTau21Max                  = cms.double(<fatJetTau21Max>), #added by rizki
    FatJetAbsEtaMax                 = cms.double(<fatJetAbsEtaMax>), #added by rizki
    SFbShift                        = cms.double(<SFbShift>),
    SFlShift                        = cms.double(<SFlShift>),
    MuonJetPtRatio                  = cms.double(<MuonJetPtRatio>),
    DiMuonJetPtRatio                = cms.double(<DiMuonJetPtRatio>),
    DoPUReweightingOfficial         = cms.bool(<doPUReweightingOfficial>),
    DoPUReweightingNPV              = cms.bool(<DoPUReweightingNPV>),
    DoFatJetPtReweighting           = cms.bool(<doFatJetPtReweighting>),
    DoNtracksReweighting            = cms.bool(<doNtracksReweighting>),
    DoBFrag                         = cms.bool(<doBFrag>),
    DoBFragUp                       = cms.bool(<doBFragUp>),
    DoBFragDown                     = cms.bool(<doBFragDown>),
    DoCDFrag                        = cms.bool(<doCDFrag>),
    DoCFrag                         = cms.bool(<doCFrag>),
    DoK0L                           = cms.bool(<doK0L>),
    DoSubJetPtReweighting           = cms.bool(<doSubJetPtReweighting>),
    DoSubJetPtBalanceReweighting    = cms.bool(<doSubJetPtBalanceReweighting>),
    DoMassSoftDropReweighting       = cms.bool(<doMassSoftDropReweighting>),
    DoJetNTracksReweighting         = cms.bool(<doJetNTracksReweighting>),
    DoSV1EnergyRatioReweighting     = cms.bool(<doSV1EnergyRatioReweighting>),
    DoIPSig1stAboveBReweighting     = cms.bool(<doIPSig1stAboveBReweighting>),
    DoZratioReweighting             = cms.bool(<doZratioReweighting>),
    TriggerSelection                = cms.vstring(<triggerSelection>), # OR of all listed triggers applied, empty list --> no trigger selection applied
    TriggerPathNames                = bTagAnalyzerCommon.TriggerPathNames,
    triggerLogicIsOR                = cms.bool(<triggerLogicIsOR>),
    ApplySFs                        = cms.bool(<applySFs>),
    btagCSVFile                     = cms.string(<btagCSVFile>),
    btagOperatingPoint              = cms.int32(<btagOperatingPoint>),
    btagMeasurementType             = cms.string(<btagMeasurementType>),
    btagSFType                      = cms.string(<btagSFType>),
    newJECPayloadNames              = cms.vstring(<newJECPayloadNames>),
    jecUncPayloadName               = cms.string(<jecUncPayloadName>),
    doNewJEC                        = cms.bool(<doNewJEC>),
    doJECUncert                     = cms.bool(<doJECUncert>),
    File_BFrag                      = cms.string(<FileBFrag>),
    produceDoubleBCommissioning     = cms.bool(<produceDoubleBCommissioning>),
    produceDeepDoubleXCommissioning = cms.bool(<produceDeepDoubleXCommissioning>),
    produceDDXSFtemplates           = cms.bool(<produceDDXSFtemplates>),
    DDXWPFile                         = cms.string(<DDXWPFile>),
    chooseDDXtagger                 = cms.string(<chooseDDXtagger>),
    produceDoubleBSFtemplates       = cms.bool(<produceDoubleBSFtemplates>),
    produceDoubleBSFtemplatesV2     = cms.bool(<produceDoubleBSFtemplatesV2>),
    produceDoubleBSFtemplates_JPhasSV     = cms.bool(<produceDoubleBSFtemplates_JPhasSV>),
    produceDoubleBSFtemplates_JPnoSV     = cms.bool(<produceDoubleBSFtemplates_JPnoSV>),
    useRunRange                     = cms.bool(<useRunRange>),
    runRangeMin                     = cms.int32(<runRangeMin>),
    runRangeMax                     = cms.int32(<runRangeMax>),
    runOnData                       = cms.bool(<runOnData>),
    variables                       = variableToRead,
    groups                          = groupSet.groups
)

# process.btagvalsubjetmu = process.btagval.clone(
#     ApplySubJetMuonTagging = cms.bool(not options.applySubJetMuonTagging),
# )

# process.btagvalsubjetbtag = process.btagval.clone(
    # ApplySubJetMuonTagging = cms.bool(not options.applySubJetMuonTagging),
#     ApplySubJetBTagging = cms.bool(True),
# )

process.p = cms.Path(process.btagval)
# process.p = cms.Path(process.btagval + process.btagvalsubjetmu + process.btagvalsubjetbtag)
