// -*- C++ -*-
//
// Package:    BTagValidation
// Class:      BTagValidation
//
/**\class BTagValidation BTagValidation.cc RecoBTag/BTagValidation/src/BTagValidation.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Devdatta Majumder,13 2-054,+41227671675, Modified by Rizki Syarif,
//         Created:  Fri May 17 13:56:04 CEST 2013
// $Id: BTagValidation.cc,v 1.21 2013/06/13 12:18:56 devdatta Exp $
//
//


// system include files
#include <iostream>
#include <memory>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "RecoBTag/PerformanceMeasurements/interface/JetInfoBranches.h"
#include "RecoBTag/PerformanceMeasurements/interface/EventInfoBranches.h"
#include "RecoBTag/PerformanceMeasurements/interface/VariableParser.h"

#include <TString.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TF1.h>

#include <string>
#include <map>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

//
// class declaration
//

class BTagValidation : public edm::EDAnalyzer {
  public:
    explicit BTagValidation(const edm::ParameterSet&);
    ~BTagValidation();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void endRun(edm::Run const&, edm::EventSetup const&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

    bool passTrigger() ;
    bool passMuonSelection(const int muIdx, const JetInfoBranches& JetInfo, const int iJet, const double deltaR=0.4);

    //// Manage histograms
    void createJetHistos(const TString& histoTag);
    void createJetHistos_DoubleB();
    void createJetHistos_DeepDoubleX();
    void createJetHistos_SF(const TString& histoTag, const TString& LTSVvar, const TString& tagger, const std::map<TString,double>& WPmap, const std::map<TString,std::vector<int>>& ptbinmap);
    void AddHisto(const TString& name, const TString& title, const int nbins, const double min, const double max);
    void AddHisto2D(const TString& name, const TString& title, const int nbins, const double min, const double max, const int nbins2, const double min2, const double max2);

    template <class Type>
      void FillHisto(const TString& name, const int flavour, const bool isGSPbb, const bool isGSPcc ,const Type value, const double weight);

    template <class Type1, class Type2>
      void FillHisto2D(const TString& name, const int flavour, const bool isGSPbb, const bool isGSPcc, const Type1 value, const Type2 value2, const double weight);

    //// Fill jet histograms
    void fillJetHistos(const JetInfoBranches& JetInfo, const int iJet, const bool isGSPbb, const bool isGSPcc, const TString& histoTag, const int nmu, const int nselmuon, const int idxFirstMuon, const double wt);
    void fillJetHistos_DoubleB(const JetInfoBranches& JetInfo, const int iJet, const bool isGSPbb, const bool isGSPcc, const TString& histoTag, const int nmu, const int nselmuon, const int idxFirstMuon, TLorentzVector muon1_v, TLorentzVector muon2_v, TLorentzVector dimuon_v, const int idxMuon_inFirstSubjet, const int idxMuon_inSecondSubjet, TLorentzVector subjet1_p4 , TLorentzVector subjet2_p4, const double wt);
    void fillJetHistos_DeepDoubleX(const JetInfoBranches& JetInfo, const int iJet, const bool isGSPbb, const bool isGSPcc, const TString& histoTag, const double wt);
    void fillJetHistos_SF(const JetInfoBranches& JetInfo, const int iJet, const bool isGSPbb, const bool isGSPcc, const TString& histoTag, const TString& LTSVvar, const bool useSVmass, const TString& tagger, const std::map<TString,double>& WPmap, const std::map<TString,std::vector<int>>& ptbinmap, const double wt);
    
    // b-tagging scale factors
    double scaleFactor(const int partonFlavor, const double jetPt, const double jetEta, const bool isCSVM);

    double scaleFactorB_CSVL(const double jetPt, const double jetEta);
    double scaleFactorC_CSVL(const double jetPt, const double jetEta);
    double scaleFactorUDSG_CSVL(const double jetPt, const double jetEta);

    double scaleFactorB_CSVM(const double jetPt, const double jetEta);
    double scaleFactorC_CSVM(const double jetPt, const double jetEta);
    double scaleFactorUDSG_CSVM(const double jetPt, const double jetEta);

    double GetLumiWeightsPVBased (const std::string file, const std::string hist, const int npv) ; 
    double GetWeightsJetPtBased (const std::string file, const std::string hist, const double jetpt) ;
    double GetWeightsSubJetPtBalanceBased (const std::string file, const std::string hist, const double jetptbalance) ;
    double GetWeightsMassSoftDropBased (const std::string file, const std::string hist, const double jetmass) ;
    double GetWeightsJetNTracksBased (const std::string file, const std::string hist, const double jetNTracks) ;
    double GetWeightsIPSig1stAboveBBased (const std::string file, const std::string hist, const double IPSig) ;
    double GetWeightsSV1EnergyRatioBased (const std::string file, const std::string hist, const double ratio) ;
    double GetWeightsZratioBased (const std::string file, const std::string hist, const double ratio) ;

    double GetWeightsNtracksBased (const std::string file, const std::string hist, const double jetpt) ;

    void ApplyJES(TLorentzVector jetp4, double jetarea, double jetrho, double jes, int npv, double& newjec) ; 
    double GetJESUncert( int jecShift, double jetpt, double jeteta, double& jesunc ) ; 

    // ----------member data ---------------------------
    EventInfoBranches EvtInfo;
    JetInfoBranches FatJetInfo;
    SubJetInfoBranches SubJetInfo;

    JetInfoBranches SubJets ;

    TChain* JetTreeEvtInfo;
    TChain* JetTree;

    std::map<TString, TH1D*> HistoBtag_map;
    std::map<TString, TH2D*> HistoBtag2D_map;


    edm::Service<TFileService> fs;

    TH1D *h1_CutFlow;
    TH1D *h1_CutFlow_unw;
    TH1D *h1_nPUtrue_mc;
    TH1D *h1_nPUtrue_mc_unw;
    TH1D *h1_nPV_data;
    TH1D *h1_nPV_mc;
    TH1D *h1_nPV_mc_unw;
    TH1D *h1_pt_hat;
    TH1D *h1_pt_hat_sel;

    /*    TH1D *hcheck_run;
          TH1D *hcheck_id;
          TH1D *hcheck_jetpt; 
          TH1D *hcheck_jeteta;
          TH1D *hcheck_mu1pt;
          TH1D *hcheck_mu2pt;
          TH1D *hcheck_mu1eta;
          TH1D *hcheck_mu2eta;
          TH1D *hcheck_ptratioComb;*/

    TH1D *h1_nFatJet;
    TH1D *h1_fatjet_pt;
    TH1D *h1_leading_fatjet_pt;

    TH1D *h1_nSubJet;
    TH1D *h1_subjet_pt;

    TProfile *p1_SubJetPt_TotalTracks;
    TProfile *p1_SubJetPt_SharedTracks;
    TProfile *p1_SubJetPt_SharedTracksRatio;

    TProfile *p1_FatJetPt_TotalTracks;
    TProfile *p1_FatJetPt_SharedTracks;
    TProfile *p1_FatJetPt_SharedTracksRatio;

    // CSVL scale factors
    TF1  *CSVL_SFb_0to2p4;
    TH1D *CSVL_SFb_errors;

    TF1 *CSVL_SFl_0to0p5;
    TF1 *CSVL_SFl_0p5to1p0;
    TF1 *CSVL_SFl_1p0to1p5;
    TF1 *CSVL_SFl_1p5to2p4;

    TF1 *CSVL_SFl_0to0p5_min;
    TF1 *CSVL_SFl_0p5to1p0_min;
    TF1 *CSVL_SFl_1p0to1p5_min;
    TF1 *CSVL_SFl_1p5to2p4_min;

    TF1 *CSVL_SFl_0to0p5_max;
    TF1 *CSVL_SFl_0p5to1p0_max;
    TF1 *CSVL_SFl_1p0to1p5_max;
    TF1 *CSVL_SFl_1p5to2p4_max;

    // CSVM scale factors
    TF1  *CSVM_SFb_0to2p4;
    TH1D *CSVM_SFb_errors;

    TF1 *CSVM_SFl_0to0p8;
    TF1 *CSVM_SFl_0p8to1p6;
    TF1 *CSVM_SFl_1p6to2p4;

    TF1 *CSVM_SFl_0to0p8_min;
    TF1 *CSVM_SFl_0p8to1p6_min;
    TF1 *CSVM_SFl_1p6to2p4_min;

    TF1 *CSVM_SFl_0to0p8_max;
    TF1 *CSVM_SFl_0p8to1p6_max;
    TF1 *CSVM_SFl_1p6to2p4_max;

    //// Lumi reweighting object
    edm::LumiReWeighting LumiWeights_;

    //// Configurables
    const bool                      DEBUG_;
    const int                       DEBUGlevel_;
    const int                       maxEvents_;
    const int                       reportEvery_;
    const bool                      useJetProbaTree_;
    const std::string               inputTTreeEvtInfo_;
    const std::string               inputTTree_;
    const std::vector<std::string>  inputFiles_;
    const bool                      useFlavorCategories_;
    const bool                      useRelaxedMuonID_;
    const bool                      applyFatJetMuonTagging_;
    const bool                      applyFatJetMuonTaggingV2_;
    const bool                      fatJetDoubleTagging_;
    const bool                      applyFatJetBTagging_;
    const bool                      fatJetDoubleBTagging_;
    const bool                      fatJetDoubleSVBTagging_;
    const bool                      applySubJetMuonTagging_;
    const bool                      applySubJetBTagging_;
    const bool                      dynamicMuonSubJetDR_;
    const double                    fatJetBDiscrCut_;
    const double                    fatJetDoubleSVBDiscrMin_;
    const double                    fatJetDoubleSVBDiscrMax_;
    const double                    subJetBDiscrMin_;
    const double                    subJetBDiscrMax_;
    const double                    muonPtMin_;
    const double                    fatJetPtMin_;
    const double                    fatJetPtMax_;
    const bool                      removeProblemJet_; 
    const double                    fatJetAbsEtaMax_;
    const bool                      usePrunedMass_ ;
    const bool                      useSoftDropMass_ ;
    const double                    fatJetGroomedMassMin_;
    const double                    fatJetGroomedMassMax_;
    const double                        fatJetTau21Min_;
    const double                        fatJetTau21Max_;
    const double                    SFbShift_;
    const double                    SFlShift_;
    const double                    MuonJetPtRatio_;
    const double                    DiMuonJetPtRatio_;
    const std::vector<std::string>  triggerSelection_;
    const std::vector<std::string>  triggerPathNames_;
    const bool                      triggerLogicIsOR_;
    const std::string               file_PVWt_ ; 
    const std::string               file_PUDistMC_ ;
    const std::string               file_PUDistData_ ;
    const std::string               file_FatJetPtWt_ ;
    const std::string               file_NtracksWt_;
    const std::string               file_SubJetPtWt_ ;
    const std::string               file_SubJetPtBalanceWt_ ;
    const std::string               file_MassSoftDropWt_ ;
    const std::string               file_JetNTracksWt_ ;
    const std::string               file_SV1EnergyRatioWt_ ;
    const std::string               file_IPSig1stAboveBWt_ ;
    const std::string               file_ZratioWt_ ;
    const std::string               hist_PVWt_ ; 
    const std::string               hist_PUDistMC_ ;
    const std::string               hist_PUDistData_ ;
    const std::string               hist_FatJetPtWt_ ;
    const std::string               hist_NtracksWt_;
    const std::string               hist_SubJetPtWt_ ;
    const std::string               hist_SubJetPtBalanceWt_ ;
    const std::string               hist_MassSoftDropWt_ ;
    const std::string               hist_JetNTracksWt_ ;
    const std::string               hist_IPSig1stAboveBWt_ ;
    const std::string               hist_SV1EnergyRatioWt_ ;
    const std::string               hist_ZratioWt_ ;
    const bool                      doPUReweightingOfficial_ ;
    const bool                      doPUReweightingNPV_ ;
    const bool                      doFatJetPtReweighting_ ;
    const bool                      doNtracksReweighting_;
    const bool                      doBFrag_;
    const bool                      doBFragUp_; 
    const bool                      doBFragDown_;
    const std::string               file_BFrag_ ; 
    const bool                      doCDFrag_;
    const bool                      doCFrag_;
    const bool                      doK0L_;
    const bool                      doSubJetPtReweighting_ ;
    const bool                      doSubJetPtBalanceReweighting_ ;
    const bool                      doMassSoftDropReweighting_ ;
    const bool                      doJetNTracksReweighting_ ;
    const bool                      doIPSig1stAboveBReweighting_ ;
    const bool                      doSV1EnergyRatioReweighting_ ;
    const bool                      doZratioReweighting_ ;
    const bool                      usePrunedSubjets_ ;
    const bool                      useSoftDropSubjets_ ;
    const bool                      applySFs_;
    const std::string               btagCSVFile_ ;  
    const int                       btagOperatingPoint_ ; 
    const std::string               btagMeasurementType_ ; 
    const std::string               btagSFType_  ; 
    const std::vector<std::string>  newJECPayloadNames_;
    const std::string               jecUncPayloadName_;
    const bool                      doNewJEC_;
    const bool                      doJECUncert_;
    const bool                      produceDoubleBCommissioning_;
    const bool                      produceDeepDoubleXCommissioning_;
    const bool                      produceDDXSFtemplates_;
    const std::string               DDXWPFile_;
    const std::string               chooseDDXtagger_;
    const bool                      produceDoubleBSFtemplates_;
    const bool                      produceDoubleBSFtemplatesV2_;
    const bool                      produceDoubleBSFtemplates_JPhasSV_;
    const bool                      produceDoubleBSFtemplates_JPnoSV_;
    const bool                      useRunRange_;
    const int                       runRangeMin_;
    const int                       runRangeMax_;
    const bool                      runOnData;

    boost::shared_ptr<JetCorrectionUncertainty> ptr_jecUnc_ ; 
    boost::shared_ptr<FactorizedJetCorrector> ptr_newJEC_  ;

    const BTagCalibration calib; 
    const BTagCalibrationReader reader; 

    std::vector<edm::ParameterSet> groupSet, variableSet;
    std::unordered_set<std::string> variables;
    VariableParser variableParser;

    //// Event variables
    bool isData;
    int nEventsAll;
    int nEventsStored;


    ///// DDX SF setup
    std::map<TString,double>  WPmap;
    std::map<TString,std::vector<int>> ptbinmap;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
BTagValidation::BTagValidation(const edm::ParameterSet& iConfig) :
  DEBUG_(iConfig.getParameter<bool>("DEBUG")),
  DEBUGlevel_(iConfig.getParameter<int>("DEBUGlevel")),
  maxEvents_(iConfig.getParameter<int>("MaxEvents")),
  reportEvery_(iConfig.getParameter<int>("ReportEvery")),
  useJetProbaTree_(iConfig.getParameter<bool>("UseJetProbaTree")),
  inputTTreeEvtInfo_(iConfig.getParameter<std::string>("InputTTreeEvtInfo")),
  inputTTree_(iConfig.getParameter<std::string>("InputTTree")),
  inputFiles_(iConfig.getParameter<std::vector<std::string> >("InputFiles")),
  useFlavorCategories_(iConfig.getParameter<bool>("UseFlavorCategories")),
  useRelaxedMuonID_(iConfig.getParameter<bool>("UseRelaxedMuonID")),
  applyFatJetMuonTagging_(iConfig.getParameter<bool>("ApplyFatJetMuonTagging")),
  applyFatJetMuonTaggingV2_(iConfig.getParameter<bool>("ApplyFatJetMuonTaggingV2")),
  fatJetDoubleTagging_(iConfig.getParameter<bool>("FatJetDoubleTagging")),
  applyFatJetBTagging_(iConfig.getParameter<bool>("ApplyFatJetBTagging")),
  fatJetDoubleBTagging_(iConfig.exists("FatJetDoubleBTagging") ? iConfig.getParameter<bool>("FatJetDoubleBTagging") : fatJetDoubleTagging_ ),
  fatJetDoubleSVBTagging_(iConfig.getParameter<bool>("FatJetDoubleSVBTagging")),
  applySubJetMuonTagging_(iConfig.getParameter<bool>("ApplySubJetMuonTagging")),
  applySubJetBTagging_(iConfig.getParameter<bool>("ApplySubJetBTagging")),
  dynamicMuonSubJetDR_(iConfig.getParameter<bool>("DynamicMuonSubJetDR")),
  fatJetBDiscrCut_(iConfig.getParameter<double>("FatJetBDiscrCut")),
  fatJetDoubleSVBDiscrMin_(iConfig.getParameter<double>("FatJetDoubleSVBDiscrMin")),
  fatJetDoubleSVBDiscrMax_(iConfig.getParameter<double>("FatJetDoubleSVBDiscrMax")),
  subJetBDiscrMin_(iConfig.getParameter<double>("SubJetBDiscrMin")),
  subJetBDiscrMax_(iConfig.getParameter<double>("SubJetBDiscrMax")),
  muonPtMin_(iConfig.getParameter<double>("MuonPtMin")),
  fatJetPtMin_(iConfig.getParameter<double>("FatJetPtMin")),
  fatJetPtMax_(iConfig.getParameter<double>("FatJetPtMax")),
  removeProblemJet_(iConfig.getParameter<bool>("RemoveProblemJet")),
  fatJetAbsEtaMax_(iConfig.getParameter<double>("FatJetAbsEtaMax")),
  usePrunedMass_(iConfig.getParameter<bool>("UsePrunedMass")),
  useSoftDropMass_(iConfig.getParameter<bool>("UseSoftDropMass")), 
  fatJetGroomedMassMin_(iConfig.getParameter<double>("FatJetGroomedMassMin")),
  fatJetGroomedMassMax_(iConfig.getParameter<double>("FatJetGroomedMassMax")),
  fatJetTau21Min_(iConfig.getParameter<double>("FatJetTau21Min")),
  fatJetTau21Max_(iConfig.getParameter<double>("FatJetTau21Max")),
  SFbShift_(iConfig.getParameter<double>("SFbShift")),
  SFlShift_(iConfig.getParameter<double>("SFlShift")),
  MuonJetPtRatio_(iConfig.getParameter<double>("MuonJetPtRatio")),
  DiMuonJetPtRatio_(iConfig.getParameter<double>("DiMuonJetPtRatio")),
  triggerSelection_(iConfig.getParameter<std::vector<std::string> >("TriggerSelection")),
  triggerPathNames_(iConfig.getParameter<std::vector<std::string> >("TriggerPathNames")),
  triggerLogicIsOR_(iConfig.getParameter<bool>("triggerLogicIsOR")),
  file_PVWt_(iConfig.getParameter<std::string>("File_PVWt")),
  file_PUDistMC_(iConfig.getParameter<std::string>("File_PUDistMC")),
  file_PUDistData_(iConfig.getParameter<std::string>("File_PUDistData")),
  file_FatJetPtWt_(iConfig.getParameter<std::string>("File_FatJetPtWt")),
  file_NtracksWt_(iConfig.getParameter<std::string>("File_NtracksWt")),
  file_SubJetPtWt_(iConfig.getParameter<std::string>("File_SubJetPtWt")),
  file_SubJetPtBalanceWt_(iConfig.getParameter<std::string>("File_SubJetPtBalanceWt")),
  file_MassSoftDropWt_(iConfig.getParameter<std::string>("File_MassSoftDropWt")),
  file_JetNTracksWt_(iConfig.getParameter<std::string>("File_JetNTracksWt")),
  file_SV1EnergyRatioWt_(iConfig.getParameter<std::string>("File_SV1EnergyRatioWt")),
  file_IPSig1stAboveBWt_(iConfig.getParameter<std::string>("File_IPSig1stAboveBWt")),
  file_ZratioWt_(iConfig.getParameter<std::string>("File_ZratioWt")),
  hist_PVWt_(iConfig.getParameter<std::string>("Hist_PVWt")),
  hist_PUDistMC_(iConfig.getParameter<std::string>("Hist_PUDistMC")),
  hist_PUDistData_(iConfig.getParameter<std::string>("Hist_PUDistData")),
  hist_FatJetPtWt_(iConfig.getParameter<std::string>("Hist_FatJetPtWt")),
  hist_NtracksWt_(iConfig.getParameter<std::string>("Hist_NtracksWt")),
  hist_SubJetPtWt_(iConfig.getParameter<std::string>("Hist_SubJetPtWt")),
  hist_SubJetPtBalanceWt_(iConfig.getParameter<std::string>("Hist_SubJetPtBalanceWt")),
  hist_MassSoftDropWt_(iConfig.getParameter<std::string>("Hist_MassSoftDropWt")),
  hist_JetNTracksWt_(iConfig.getParameter<std::string>("Hist_JetNTracksWt")),
  hist_IPSig1stAboveBWt_(iConfig.getParameter<std::string>("Hist_IPSig1stAboveBWt")),
  hist_SV1EnergyRatioWt_(iConfig.getParameter<std::string>("Hist_SV1EnergyRatioWt")),
  hist_ZratioWt_(iConfig.getParameter<std::string>("Hist_ZratioWt")),
  doPUReweightingOfficial_(iConfig.getParameter<bool>("DoPUReweightingOfficial")),
  doPUReweightingNPV_(iConfig.getParameter<bool>("DoPUReweightingNPV")),
  doFatJetPtReweighting_(iConfig.getParameter<bool>("DoFatJetPtReweighting")),
  doNtracksReweighting_(iConfig.getParameter<bool>("DoNtracksReweighting")),
  doBFrag_(iConfig.getParameter<bool>("DoBFrag")),
  doBFragUp_(iConfig.getParameter<bool>("DoBFragUp")),
  doBFragDown_(iConfig.getParameter<bool>("DoBFragDown")),
  file_BFrag_(iConfig.getParameter<std::string>("File_BFrag")),
  doCDFrag_(iConfig.getParameter<bool>("DoCDFrag")),
  doCFrag_(iConfig.getParameter<bool>("DoCFrag")),
  doK0L_(iConfig.getParameter<bool>("DoK0L")),
  doSubJetPtReweighting_(iConfig.getParameter<bool>("DoSubJetPtReweighting")),
  doSubJetPtBalanceReweighting_(iConfig.getParameter<bool>("DoSubJetPtBalanceReweighting")),
  doMassSoftDropReweighting_(iConfig.getParameter<bool>("DoMassSoftDropReweighting")),
  doJetNTracksReweighting_(iConfig.getParameter<bool>("DoJetNTracksReweighting")),
  doIPSig1stAboveBReweighting_(iConfig.getParameter<bool>("DoIPSig1stAboveBReweighting")),
  doSV1EnergyRatioReweighting_(iConfig.getParameter<bool>("DoSV1EnergyRatioReweighting")),
  doZratioReweighting_(iConfig.getParameter<bool>("DoZratioReweighting")),
  usePrunedSubjets_(iConfig.getParameter<bool>("UsePrunedSubjets")),
  useSoftDropSubjets_(iConfig.getParameter<bool>("UseSoftDropSubjets")), 
  applySFs_(iConfig.getParameter<bool>("ApplySFs")),
  btagCSVFile_(iConfig.getParameter<std::string>("btagCSVFile")), 
  btagOperatingPoint_(iConfig.getParameter<int>("btagOperatingPoint")), 
  btagMeasurementType_(iConfig.getParameter<std::string>("btagMeasurementType")), 
  btagSFType_(iConfig.getParameter<std::string>("btagSFType")),
  newJECPayloadNames_(iConfig.getParameter<std::vector<std::string>>("newJECPayloadNames")),
  jecUncPayloadName_(iConfig.getParameter<std::string>("jecUncPayloadName")),
  doNewJEC_(iConfig.getParameter<bool>("doNewJEC")), 
  doJECUncert_(iConfig.getParameter<bool>("doJECUncert")), 
  produceDoubleBCommissioning_(iConfig.getParameter<bool>("produceDoubleBCommissioning")), 
  produceDeepDoubleXCommissioning_(iConfig.getParameter<bool>("produceDeepDoubleXCommissioning")), 
  produceDDXSFtemplates_(iConfig.getParameter<bool>("produceDDXSFtemplates")), 
  DDXWPFile_(iConfig.getParameter<std::string>("DDXWPFile")),
  chooseDDXtagger_(iConfig.getParameter<std::string>("chooseDDXtagger")),
  produceDoubleBSFtemplates_(iConfig.getParameter<bool>("produceDoubleBSFtemplates")), 
  produceDoubleBSFtemplatesV2_(iConfig.getParameter<bool>("produceDoubleBSFtemplatesV2")), 
  produceDoubleBSFtemplates_JPhasSV_(iConfig.getParameter<bool>("produceDoubleBSFtemplates_JPhasSV")), 
  produceDoubleBSFtemplates_JPnoSV_(iConfig.getParameter<bool>("produceDoubleBSFtemplates_JPnoSV")), 
  useRunRange_(iConfig.getParameter<bool>("useRunRange")), 
  runRangeMin_(iConfig.getParameter<int>("runRangeMin")), 
  runRangeMax_(iConfig.getParameter<int>("runRangeMax")),
  runOnData(iConfig.getParameter<bool>("runOnData")), 
  calib("csvv2", btagCSVFile_),  
  reader(BTagEntry::OperatingPoint(btagOperatingPoint_), btagMeasurementType_),
  groupSet(iConfig.getParameter<std::vector<edm::ParameterSet>>("groups")),
  variableSet(iConfig.getParameter<std::vector<edm::ParameterSet>>("variables")),
  variables(0),
  variableParser(false)
{
  //now do what ever initialization is needed

  isData = true;
  nEventsAll = 0;
  nEventsStored = 0;

/*  std::vector<edm::ParameterSet> groupSet = iConfig.getParameter<std::vector<edm::ParameterSet>>("groups");
  std::vector<edm::ParameterSet> variableSet = iConfig.getParameter<std::vector<edm::ParameterSet>>("variables");
  std::unordered_set<std::string> variables; // This unordered_set is going to contain the name of each single variable to be stored in the output tree
  varParser = VariableParser(!runOnData);
  variables = varParser.parseGroupsAndVariables(groupSet, variableSet);
  varParser.saveStoredVariablesToFile();*/

  if ( doNewJEC_ && newJECPayloadNames_.size() > 0 ) {
    std::vector<JetCorrectorParameters> vPar;  
    for ( std::vector<std::string>::const_iterator it = newJECPayloadNames_.begin(); it != newJECPayloadNames_.end(); ++it) {
      if(DEBUG_)std::cout << "correcting Jets using: " << *it << std::endl;
      JetCorrectorParameters pars(*it);
      vPar.push_back(pars) ; 
    }
    ptr_newJEC_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) ); 

  }

  if ( doJECUncert_ ) {  
    std::cout << "deriving JEC unc using: " << jecUncPayloadName_ << std::endl;     
    ptr_jecUnc_ = boost::shared_ptr<JetCorrectionUncertainty>(new JetCorrectionUncertainty(jecUncPayloadName_) ) ; 
  }

  if (doPUReweightingOfficial_) LumiWeights_ = edm::LumiReWeighting(file_PUDistMC_, file_PUDistData_, hist_PUDistMC_, hist_PUDistData_) ;
  
  //checks: for weights
  if (doFatJetPtReweighting_) std::cout << "ATTENTION   : pT reweighting enabled (for MC). File to be used: "<< file_FatJetPtWt_ << std::endl;  
  if (doNtracksReweighting_)  std::cout << "ATTENTION   : nTracks reweighting enabled (for MC). File to be used: "<< file_NtracksWt_ << std::endl;
  if (doBFrag_ && doBFragUp_)  std::cout << "ATTENTION  : BFrag (Up) weighting enabled. File path to be used: "<< file_BFrag_ << std::endl;
  if (doBFrag_ && doBFragDown_)  std::cout << "ATTENTION    : BFrag (Down) weighting enabled. File path to be used: "<< file_BFrag_ << std::endl;
  if (doBFrag_ && doBFragUp_ && doBFragDown_)  std::cout << "ATTENTION  : BFrag Up and Down weighting enabled!!!!! File path to be used: "<< file_BFrag_ << std::endl;
  if (doCDFrag_)  std::cout << "ATTENTION   : CDFrag weighting enabled. " << std::endl;
  if (doCFrag_)  std::cout << "ATTENTION    : CFrag weighting enabled. " << std::endl;
  if (doK0L_)  std::cout << "ATTENTION  : K0L weighting enabled. " << std::endl;

  if(!produceDoubleBSFtemplates_ && 
    !produceDoubleBSFtemplatesV2_ &&
    !produceDoubleBSFtemplates_JPhasSV_ &&
    !produceDoubleBSFtemplates_JPnoSV_) 
    std::cout << "ATTENTION : NOT creating DoubleB SF templates." << std::endl;

  if(produceDoubleBCommissioning_) std::cout << "ATTENTION  : Creating DoubleB Commissioning plots." << std::endl;
  if(produceDeepDoubleXCommissioning_) std::cout << "ATTENTION  : Creating DeepDoubleX Commissioning plots." << std::endl;
  if(produceDoubleBSFtemplates_) std::cout << "ATTENTION    : Creating DoubleB SF templates (JP plots)." << std::endl;
  if(produceDoubleBSFtemplatesV2_) std::cout << "ATTENTION  : Creating DoubleB SF templates (sv mass plots)." << std::endl;
  if(produceDoubleBSFtemplates_JPhasSV_) std::cout << "ATTENTION    : Creating DoubleB SF templates (JPhasSV plots)." << std::endl;
  if(produceDoubleBSFtemplates_JPnoSV_) std::cout << "ATTENTION : Creating DoubleB SF templates (JPnoSV plots)." << std::endl;
  if(produceDDXSFtemplates_) std::cout << "ATTENTION : Creating DDBvL templats (all)." << std::endl;

  if(DEBUG_ && triggerSelection_.size()>1){
    std::cout << "ATTENTION : Multiple triggers entered. Will perform the following logic: ";
    if(!triggerLogicIsOR_) std::cout << " AND " << std::endl; 
    else std::cout << " OR " << std::endl;
  }

// Pt bins for SFb
  double PtBins[] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600, 800};

  // CSVL scale factors
  CSVL_SFb_0to2p4 = new TF1("CSVL_SFb_0to2p4","0.997942*((1.+(0.00923753*x))/(1.+(0.0096119*x)))", 20.,800.);

  CSVL_SFb_errors = new TH1D("CSVL_SFb_errors", "CSVL_SFb_errors", 16, PtBins);
  CSVL_SFb_errors->SetBinContent( 0,(2*0.033299));
  CSVL_SFb_errors->SetBinContent( 1,0.033299);
  CSVL_SFb_errors->SetBinContent( 2,0.0146768);
  CSVL_SFb_errors->SetBinContent( 3,0.013803);
  CSVL_SFb_errors->SetBinContent( 4,0.0170145);
  CSVL_SFb_errors->SetBinContent( 5,0.0166976);
  CSVL_SFb_errors->SetBinContent( 6,0.0137879);
  CSVL_SFb_errors->SetBinContent( 7,0.0149072);
  CSVL_SFb_errors->SetBinContent( 8,0.0153068);
  CSVL_SFb_errors->SetBinContent( 9,0.0133077);
  CSVL_SFb_errors->SetBinContent(10,0.0123737);
  CSVL_SFb_errors->SetBinContent(11,0.0157152);
  CSVL_SFb_errors->SetBinContent(12,0.0175161);
  CSVL_SFb_errors->SetBinContent(13,0.0209241);
  CSVL_SFb_errors->SetBinContent(14,0.0278605);
  CSVL_SFb_errors->SetBinContent(15,0.0346928);
  CSVL_SFb_errors->SetBinContent(16,0.0350099);
  CSVL_SFb_errors->SetBinContent(17,(2*0.0350099));

  CSVL_SFl_0to0p5 =   new TF1("CSVL_SFl_0to0p5","((1.01177+(0.0023066*x))+(-4.56052e-06*(x*x)))+(2.57917e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_0p5to1p0 = new TF1("CSVL_SFl_0p5to1p0","((0.975966+(0.00196354*x))+(-3.83768e-06*(x*x)))+(2.17466e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p0to1p5 = new TF1("CSVL_SFl_1p0to1p5","((0.93821+(0.00180935*x))+(-3.86937e-06*(x*x)))+(2.43222e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p5to2p4 = new TF1("CSVL_SFl_1p5to2p4","((1.00022+(0.0010998*x))+(-3.10672e-06*(x*x)))+(2.35006e-09*(x*(x*x)))", 20.,850.);

  CSVL_SFl_0to0p5_min =   new TF1("CSVL_SFl_0to0p5_min","((0.977761+(0.00170704*x))+(-3.2197e-06*(x*x)))+(1.78139e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_0p5to1p0_min = new TF1("CSVL_SFl_0p5to1p0_min","((0.945135+(0.00146006*x))+(-2.70048e-06*(x*x)))+(1.4883e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p0to1p5_min = new TF1("CSVL_SFl_1p0to1p5_min","((0.911657+(0.00142008*x))+(-2.87569e-06*(x*x)))+(1.76619e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p5to2p4_min = new TF1("CSVL_SFl_1p5to2p4_min","((0.970045+(0.000862284*x))+(-2.31714e-06*(x*x)))+(1.68866e-09*(x*(x*x)))", 20.,850.);

  CSVL_SFl_0to0p5_max =   new TF1("CSVL_SFl_0to0p5_max","((1.04582+(0.00290226*x))+(-5.89124e-06*(x*x)))+(3.37128e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_0p5to1p0_max = new TF1("CSVL_SFl_0p5to1p0_max","((1.00683+(0.00246404*x))+(-4.96729e-06*(x*x)))+(2.85697e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p0to1p5_max = new TF1("CSVL_SFl_1p0to1p5_max","((0.964787+(0.00219574*x))+(-4.85552e-06*(x*x)))+(3.09457e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p5to2p4_max = new TF1("CSVL_SFl_1p5to2p4_max","((1.03039+(0.0013358*x))+(-3.89284e-06*(x*x)))+(3.01155e-09*(x*(x*x)))", 20.,850.);

  // CSVM scale factors
  CSVM_SFb_0to2p4 = new TF1("CSVM_SFb_0to2p4","(0.938887+(0.00017124*x))+(-2.76366e-07*(x*x))", 20.,800.);

  CSVM_SFb_errors = new TH1D("CSVM_SFb_errors", "CSVM_SFb_errors", 16, PtBins);
  CSVM_SFb_errors->SetBinContent( 0,(2*0.0415707));
  CSVM_SFb_errors->SetBinContent( 1,0.0415707);
  CSVM_SFb_errors->SetBinContent( 2,0.0204209);
  CSVM_SFb_errors->SetBinContent( 3,0.0223227);
  CSVM_SFb_errors->SetBinContent( 4,0.0206655);
  CSVM_SFb_errors->SetBinContent( 5,0.0199325);
  CSVM_SFb_errors->SetBinContent( 6,0.0174121);
  CSVM_SFb_errors->SetBinContent( 7,0.0202332);
  CSVM_SFb_errors->SetBinContent( 8,0.0182446);
  CSVM_SFb_errors->SetBinContent( 9,0.0159777);
  CSVM_SFb_errors->SetBinContent(10,0.0218531);
  CSVM_SFb_errors->SetBinContent(11,0.0204688);
  CSVM_SFb_errors->SetBinContent(12,0.0265191);
  CSVM_SFb_errors->SetBinContent(13,0.0313175);
  CSVM_SFb_errors->SetBinContent(14,0.0415417);
  CSVM_SFb_errors->SetBinContent(15,0.0740446);
  CSVM_SFb_errors->SetBinContent(16,0.0596716);
  CSVM_SFb_errors->SetBinContent(17,(2*0.0596716));

  CSVM_SFl_0to0p8 =   new TF1("CSVM_SFl_0to0p8","((1.07541+(0.00231827*x))+(-4.74249e-06*(x*x)))+(2.70862e-09*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_0p8to1p6 = new TF1("CSVM_SFl_0p8to1p6","((1.05613+(0.00114031*x))+(-2.56066e-06*(x*x)))+(1.67792e-09*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_1p6to2p4 = new TF1("CSVM_SFl_1p6to2p4","((1.05625+(0.000487231*x))+(-2.22792e-06*(x*x)))+(1.70262e-09*(x*(x*x)))", 20.,850.);

  CSVM_SFl_0to0p8_min =   new TF1("CSVM_SFl_0to0p8_min","((0.964527+(0.00149055*x))+(-2.78338e-06*(x*x)))+(1.51771e-09*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_0p8to1p6_min = new TF1("CSVM_SFl_0p8to1p6_min","((0.946051+(0.000759584*x))+(-1.52491e-06*(x*x)))+(9.65822e-10*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_1p6to2p4_min = new TF1("CSVM_SFl_1p6to2p4_min","((0.956736+(0.000280197*x))+(-1.42739e-06*(x*x)))+(1.0085e-09*(x*(x*x)))", 20.,850.);

  CSVM_SFl_0to0p8_max =   new TF1("CSVM_SFl_0to0p8_max","((1.18638+(0.00314148*x))+(-6.68993e-06*(x*x)))+(3.89288e-09*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_0p8to1p6_max = new TF1("CSVM_SFl_0p8to1p6_max","((1.16624+(0.00151884*x))+(-3.59041e-06*(x*x)))+(2.38681e-09*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_1p6to2p4_max = new TF1("CSVM_SFl_1p6to2p4_max","((1.15575+(0.000693344*x))+(-3.02661e-06*(x*x)))+(2.39752e-09*(x*(x*x)))", 20.,850.);
}


BTagValidation::~BTagValidation() {

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called once each job just before starting event loop  ------------
void BTagValidation::beginJob() {


  JetTreeEvtInfo = new TChain(inputTTreeEvtInfo_.c_str());
  JetTree = new TChain(inputTTree_.c_str());

  for(unsigned i=0; i<inputFiles_.size(); ++i)
  {
    std::cout << inputFiles_.at(i).c_str() << std::endl;
    JetTreeEvtInfo->Add(inputFiles_.at(i).c_str());
    JetTree->Add(inputFiles_.at(i).c_str());

    TFile *f = TFile::Open(inputFiles_.at(i).c_str(),"READ");

    TH1D *hEventCountAll    = (TH1D*)f->Get("allEvents/hEventCount");
    TH1D *hEventCountStored = (TH1D*)f->Get("selectedEvents/hEventCount");

    nEventsAll+=hEventCountAll->GetBinContent(1);
    nEventsStored+=hEventCountStored->GetBinContent(1);

    f->Close();
  }

  variables = variableParser.parseGroupsAndVariables(groupSet, variableSet);
  variableParser.saveStoredVariablesToFile();
  
  if(DEBUG_) std::cout << "Attempting to read EvtInfo trees" << std::endl;      
  //EvtInfo.ReadBranches(JetTreeEvtInfo,varParser);
  EvtInfo.ReadBranches(JetTreeEvtInfo,variableParser);
  if(DEBUG_) std::cout << " Done reading EvtInfo.ReadTree" << std::endl;        
;       

  if(DEBUG_) std::cout << "Attempting to read FatJetInfo trees" << std::endl;       
  //FatJetInfo.ReadBranches(JetTree,varParser,"FatJetInfo");
  FatJetInfo.ReadBranches(JetTree,variableParser,"FatJetInfo");
  if(DEBUG_) std::cout << " Done reading FatJetInfo trees" << std::endl;        

  if (usePrunedSubjets_) {
    if(DEBUG_) std::cout << "Attempting to read SubJetInfo trees - Pruned" << std::endl;        
    //SubJetInfo.ReadBranches(JetTree,varParser,"FatJetInfo","Pruned");
    //SubJets.ReadBranches(JetTree,varParser,"PrunedSubJetInfo") ;
    SubJetInfo.ReadBranches(JetTree,variableParser,"FatJetInfo","Pruned");
    SubJets.ReadBranches(JetTree,variableParser,"PrunedSubJetInfo") ;
    if(DEBUG_) std::cout << "   Done reading SubJetInfo trees - Pruned" << std::endl;       
  }


  else if (useSoftDropSubjets_) {
    if(DEBUG_) std::cout << "Attempting to read SubJetInfo trees - SoftDropPuppi" << std::endl;     
    //SubJetInfo.ReadBranches(JetTree,varParser,"FatJetInfo","SoftDropPuppi");
    //SubJets.ReadBranches(JetTree,varParser,"SoftDropPuppiSubJetInfo") ;
    SubJetInfo.ReadBranches(JetTree,variableParser,"FatJetInfo","SoftDropPuppi");
    SubJets.ReadBranches(JetTree,variableParser,"SoftDropPuppiSubJetInfo") ;
    if(DEBUG_) std::cout << "   Done reading SubJetInfo trees - SoftDropPuppi" << std::endl;        
  }
  else {
    SubJetInfo.ReadBranches(JetTree,variableParser,"FatJetInfo","SoftDropPuppi");
    SubJets.ReadBranches(JetTree,variableParser,"SoftDropPuppiSubJetInfo") ;
    edm::LogInfo("Error") << ">>>> No subjet type specified. Reading SoftDropSubject for DDX SFs\n" ;
  }
  
  double PtMax = 5000.;

  h1_CutFlow        = fs->make<TH1D>("h1_CutFlow",       "h1_CutFlow",        4,-0.5,3.5);
  h1_CutFlow->Sumw2();
  h1_CutFlow->SetDefaultSumw2(kTRUE); //// enables automatic calling of TH1::Sumw2 for all subsequent histograms
  h1_CutFlow_unw    = fs->make<TH1D>("h1_CutFlow_unw",   "h1_CutFlow_unw",    4,-0.5,3.5);
  h1_nPUtrue_mc     = fs->make<TH1D>("h1_nPUtrue_mc",    ";N(true PU in MC);;",     100,0.,100.);
  h1_nPUtrue_mc_unw = fs->make<TH1D>("h1_nPUtrue_mc_unw",";N(true PU in MC, unweighted);;", 100,0.,100.);
  h1_nPV_data       = fs->make<TH1D>("h1_nPV_data",      ";N(PV in data);;",       100,0.,100.);
  h1_nPV_mc         = fs->make<TH1D>("h1_nPV_mc",        ";N(PV in MC);;",         100,0.,100.);
  h1_nPV_mc_unw     = fs->make<TH1D>("h1_nPV_mc_unw",    ";N(PV in MC, unweighted)",     100,0.,100.);
  h1_pt_hat         = fs->make<TH1D>("h1_pt_hat",        ";#hat{p}_{T} before selection;;",         1400,0,7000);
  h1_pt_hat_sel     = fs->make<TH1D>("h1_pt_hat_sel",    ";#hat{p}_{T} after selection;;",         1400,0,7000);

  h1_nFatJet        = fs->make<TH1D>("h1_nFatJet",       ";N(AK8 jets);;",     100,0,100);
  h1_fatjet_pt      = fs->make<TH1D>("h1_fatjet_pt",     ";p_{T} (AK8 jets) [GeV];;",   PtMax/10,0,PtMax);
  h1_leading_fatjet_pt      = fs->make<TH1D>("h1_leading_fatjet_pt",     ";p_{T} (leading AK8 jet) [GeV];;",   PtMax/10,0,PtMax);

  /*  hcheck_run        = fs->make<TH1D>("hcheck_run",     ";run number;;",   );
      hcheck_id         = fs->make<TH1D>("hcheck_id",     ";event number;;",   nEntries,0,nEntries);
      hcheck_jetpt      = fs->make<TH1D>("hcheck_jetpt",     ";jet pt;;",   PtMax/10,0,PtMax);
      hcheck_jeteta     = fs->make<TH1D>("hcheck_jeteta",     ";jet eta;;",   50,-2.5,2.5);
      hcheck_mu1pt      = fs->make<TH1D>("hcheck_mu1pt",     ";mu1 pt;;",   250,0,500);
      hcheck_mu1eta     = fs->make<TH1D>("hcheck_mu1eta",     ";mu1 eta;;",   50,-2.5,2.5);
      hcheck_mu2pt      = fs->make<TH1D>("hcheck_mu2pt",     ";mu2 pt;;",   250,0,500);
      hcheck_mu2eta     = fs->make<TH1D>("hcheck_mu2eta",     ";mu2 eta;;",   50,-2.5,2.5);
      hcheck_ptratioComb= fs->make<TH1D>("hcheck_ptratioComb",     ";pt ratio combined;;",   50,0,1);
      */
  if( usePrunedSubjets_ ) {
    h1_nSubJet        = fs->make<TH1D>("h1_nPrunedSubJet",       ";N(pruned subjets);;",     100,0,100);
    h1_subjet_pt      = fs->make<TH1D>("h1_PrunedSubjet_pt",     ";p_{T} (pruned subjets) [GeV]",   PtMax/10,0,PtMax);
    p1_SubJetPt_TotalTracks = fs->make<TProfile>("p1_PrunedSubJetPt_TotalTracks",";p_{T} (pruned subjets) [GeV];N(total tracks);",20,0,1000);
    p1_SubJetPt_SharedTracks = fs->make<TProfile>("p1_PrunedSubJetPt_SharedTracks",";p_{T} (pruned subjets) [GeV];N(shared tracks);",20,0,1000);
    p1_SubJetPt_SharedTracksRatio = fs->make<TProfile>("p1_PrunedSubJetPt_SharedTracksRatio",";p_{T} (pruned subjets) [GeV];N(shared track ratio);",20,0,1000);
  }
  else if( useSoftDropSubjets_ ) {
    h1_nSubJet        = fs->make<TH1D>("h1_nSoftDropSubJet",       ";N(soft drop subjets);;",     100,0,100);
    h1_subjet_pt      = fs->make<TH1D>("h1_SoftDropSubjet_pt",     "p_{T} (soft drop subjets) [GeV]",   PtMax/10,0,PtMax);
    p1_SubJetPt_TotalTracks = fs->make<TProfile>("p1_SoftDropSubJetPt_TotalTracks",";p_{T} (soft drop subjets) [GeV];N(total tracks);",20,0,1000);
    p1_SubJetPt_SharedTracks = fs->make<TProfile>("p1_SoftDropSubJetPt_SharedTracks",";p_{T} (soft drop subjets) [GeV];N(shared tracks);",20,0,1000);
    p1_SubJetPt_SharedTracksRatio = fs->make<TProfile>("p1_SoftDropSubJetPt_SharedTracksRatio",";p_{T} (soft drop subjets) [GeV];N(shared track ratio);",20,0,1000);
  }

  p1_FatJetPt_TotalTracks = fs->make<TProfile>("p1_FatJetPt_TotalTracks",";p_{T} (AK8 jets) [GeV];N(total tracks);",20,0,1000);
  p1_FatJetPt_SharedTracks = fs->make<TProfile>("p1_FatJetPt_SharedTracks",";p_{T} (AK8 jets) [GeV];N(shared tracks);",20,0,1000);
  p1_FatJetPt_SharedTracksRatio = fs->make<TProfile>("p1_FatJetPt_SharedTracksRatio",";p_{T} (AK8 jets) [GeV];N(shared track ratio);",20,0,1000);

  //// Create jet histograms
  if (usePrunedSubjets_) {
    AddHisto("FatJet_prunedsubjet_dR"           ,";#DeltaR_{#eta-#phi}(pruned subjets);;",100,0,1);
    AddHisto("FatJet_prunedsubjet_dyphi"        ,";#DeltaR_{y-#phi}(pruned subjets);;",100,0,1);
    AddHisto("FatJet_massDrop_pruned"           ,";Mass drop (pruned AK8 jets) [GeV];;",100,0,1);
    AddHisto("FatJet_prunedsubjet_ptBalance"    ,";Subjet pT_1/(pT_1+pT_2) (pruned subjets) ;;",100,0,1); //added by rizki - for signal proxy study   
    AddHisto("FatJet_prunedsubjet_ptBalance_unw"    ,";Subjet pT_1/(pT_1+pT_2) (pruned subjets) unweighted;;",100,0,1); //added by rizki - for signal proxy study   
  }
  else if (useSoftDropSubjets_) {
    AddHisto("FatJet_softdropsubjet_dR"           ,";#DeltaR_{#eta-#phi}(soft drop subjets);;",100,0,1);
    AddHisto("FatJet_softdropsubjet_dyphi"        ,";#DeltaR_{y-#phi}(soft drop subjets);;",100,0,1);
    AddHisto("FatJet_massDrop_softdrop"           ,";Mass drop (soft drop AK8 jets) [GeV];;",100,0,1);
    AddHisto("FatJet_softdropsubjet_ptBalance"    ,";Subjet pT_1/(pT_1+pT_2) (soft drop subjets) ;;",100,0,1); //added by rizki - for signal proxy study    
    AddHisto("FatJet_softdropsubjet_ptBalance_unw"    ,";Subjet pT_1/(pT_1+pT_2) (soft drop subjets) unweighted ;;",100,0,1); //added by rizki - for signal proxy study    
  }
  AddHisto("FatJet_Mass"                        ,";M (AK8 jets) [GeV];;",200,0,400);
  AddHisto("FatJet_nsubjettiness"               ,";#tau_{2}/#tau_{1};;",50,0,1);
  AddHisto("FatJet_prunedMass"                  ,";M_{pruned} (AK8 jets) [GeV];;",200,0,400);
  AddHisto("FatJet_softdropMass"                ,";M_{soft drop} (AK8 jets) [GeV];;",200,0,400);
  AddHisto("FatJet_softdropMass_unw"                ,";unweighted M_{soft drop} (AK8 jets) [GeV];;",200,0,400);
  AddHisto2D("FatJet_prunedMass_nsubjettiness"  ,";M_{pruned} (AK8 jets) [GeV];#tau_{2}/#tau_{1};" ,200 ,0 ,400 ,50 ,0 ,1);
  AddHisto2D("FatJet_pt_prunedMass"             ,";p_{T} (AK8 jets) [GeV];M_{pruned} (AK8 jets) [GeV];",PtMax/10,0,PtMax,200,0,400);
  AddHisto2D("FatJet_softdropMass_nsubjettiness",";M_{soft drop} (AK8 jets) [GeV];#tau_{2}/#tau_{1};",200,0,400,50,0,1);
  AddHisto2D("FatJet_pt_softdropMass"           ,";p_{T} (AK8 jets) [GeV];M_{soft drop} (AK8 jets) [GeV];",PtMax/10,0,PtMax,200,0,400);


  //// Common histograms for both fat jets and subjets
  createJetHistos("FatJet");
  if( usePrunedSubjets_ ) createJetHistos("PrunedSubJet");
  else if( useSoftDropSubjets_ ) createJetHistos("SoftDropSubJet");
  
  ///Create DoubleB input vars
  if(produceDoubleBCommissioning_) createJetHistos_DoubleB();
  if(produceDeepDoubleXCommissioning_) createJetHistos_DeepDoubleX();

  ///Create histos for DoubleB SF calculation/templates

  if(produceDDXSFtemplates_) {
    // Read chooseDDXtagger_ WPs from DDX.
    std:: cout << "WP file " << DDXWPFile_ << std::endl;
    boost::property_tree::ptree root;
    try {
    //boost::property_tree::read_json("aux/DDX.json", root);
    boost::property_tree::read_json(DDXWPFile_, root);
    }
    catch(std::exception & e) {
    }
    std::cout << "Reading WPs for DDX tagger : " << chooseDDXtagger_ << std::endl;
    //for (auto& wp_pair : root.get_child(xtag)) {
    for (auto& wp_pair : root.get_child(chooseDDXtagger_)) {
        TString wp = wp_pair.first;
        double wp_value = std::stod(wp_pair.second.data());
        std::cout << "        WP: " << wp << " = " << wp_value << std::endl;
        WPmap.insert (std::pair<TString,double>(wp, wp_value)); 
    } 
    // Define ptbins in one place
    ptbinmap = {//{"pt250to300",{250,300}},
                // {"pt300to350",{300,350}},
                // {"pt350to400",{350,400}},
                // {"pt400to450",{400,450}},
                // {"pt450to500",{450,500}},
                // {"pt400to470",{400,470}},
                // {"pt470to2000",{470,2000}},
                {"pt350to430",{350,430}},
                {"pt430to2000",{430,2000}},
                // {"pt250to350",{250,350}},
                // {"pt350to450",{350,450}},
                // {"pt300to400",{300,400}},
                // {"pt400to500",{400,500}},
                // {"pt500to600",{500,600}},
                // {"pt600to2000",{600,2000}},
                // {"pt500to2000",{500,2000}},
                //{"pt450to2000",{450,2000}},
                };
    
    TString taggerTString = chooseDDXtagger_;
    createJetHistos_SF("FatJet", "JP",                      taggerTString, WPmap, ptbinmap);
    createJetHistos_SF("FatJet", "tau1VertexMassCorr",      taggerTString, WPmap, ptbinmap);
    createJetHistos_SF("FatJet", "JPhasSV",                 taggerTString, WPmap, ptbinmap);
    createJetHistos_SF("FatJet", "JPnoSV",                  taggerTString, WPmap, ptbinmap);
  }  
  
}

void BTagValidation::createJetHistos(const TString& histoTag) {

  double PtMax = 5000.;
  double pi=TMath::Pi();

  AddHisto(histoTag+"_pt_all_unw"           ,";p_{T}(all jets) unweighted [GeV];;"             ,PtMax/10  ,0      ,PtMax ); //debug for signal vs proxy reweighting - rizki

  AddHisto(histoTag+"_pt_all"           ,";p_{T}(all jets) [GeV];;"             ,PtMax/10  ,0      ,PtMax );
  AddHisto(histoTag+"_eta"              ,";#eta(all jets);;"              ,50        ,-2.5   ,2.5);
  AddHisto(histoTag+"_phi"              ,";#phi(all jets);;"              ,40        ,-1.*pi ,pi);
  AddHisto(histoTag+"_mass"             ,";M(all jets) [GeV];;"              ,200       ,0      ,400);
  AddHisto(histoTag+"_pt_sv"            ,";p_{T}(jets containing a SV) [GeV];;" ,PtMax/10  ,0      ,PtMax);

  AddHisto(histoTag+"_muon_multi",      ";N(#mu);;",7,-0.5,6.5);
  AddHisto(histoTag+"_muon_multi_sel",  ";N(selected #mu);;",7,-0.5,6.5);
  AddHisto(histoTag+"_mu_ptrel",        ";p_{T,rel}(#mu) [GeV];;",50,0,5);
  AddHisto(histoTag+"_mu_chi2",         ";#chi^{2}/NDF(#mu);;",50,0,10);
  AddHisto(histoTag+"_muon_Pt",         ";p_{T} (#mu) [GeV];;",250,0,500);
  AddHisto(histoTag+"_muon_eta",        ";#eta(#mu);;",50,-2.5, 2.5);
  AddHisto(histoTag+"_muon_phi",        ";#phi(#mu);;",40,-1.*pi,pi);
  AddHisto(histoTag+"_muon_Ip3d",       ";Muon 3D IP;;",500,-1.0,1.0);
  AddHisto(histoTag+"_muon_Ip2d",       ";Muon 2D IP;;",500,-1.0,1.0);
  AddHisto(histoTag+"_muon_Sip3d",      ";Muon 3D IP significance;;",100,-50,50);
  AddHisto(histoTag+"_muon_Sip2d",      ";Muon 2D IP significance;;",100,-50,50);
  AddHisto(histoTag+"_muon_DeltaR",     ";Muon1 #DeltaR;;",100,0,1.0); 
  AddHisto(histoTag+"_dimuon_ptRatio",  ";(p_{T}(#mu_{1})+p_{T}(#mu_{2}))/p_{T}(jet);;",50,0,1);
  AddHisto(histoTag+"_muon_ptRatio",  ";(p_{T}(#mu_{1}))/p_{T}(jet);;",50,0,1);   

  AddHisto(histoTag+"_sv_deltaR_jet",      ";#DeltaR(SV, jet);;",50,0.,0.5);
  AddHisto(histoTag+"_sv_deltaR_sumJet",   ";#DeltaR(SV,sumJet);;",50,0.,0.5);
  AddHisto(histoTag+"_sv_deltaR_sumDir",   ";#DeltaR(SV,sumDir);;",50,0.,0.5);
  AddHisto(histoTag+"_sv_en_ratio",        ";SV energy ratio;;",60,0.,1.2);
  AddHisto(histoTag+"_sv_pt",              ";p_{T}(SV) [GeV];;",250,0.,500.);
  AddHisto(histoTag+"_sv_eta",             ";#eta(SV);;",50,-2.5,2.5);
  AddHisto(histoTag+"_sv_phi",             ";#phi(SV);;",40,-1.*pi,pi);
  AddHisto(histoTag+"_sv_flightSig2D",     ";Flight distance significance 2D;;",150,0.,150.);
  AddHisto(histoTag+"_sv_flight2D",        ";Flight distance 2D (cm);;",100,0.,5.);
  AddHisto(histoTag+"_sv_flightSig2D_3trk",";Flight distance 2D with #geq3 tracks (cm);;",100,0.,5.);
  AddHisto(histoTag+"_sv_flight3D",        ";Flight distance 3D (cm);;",50,0.,15.);
  AddHisto(histoTag+"_sv_flight3DSig",     ";Flight distance significance 3D;;",150,0.,150.);
  AddHisto(histoTag+"_sv_multi_0",         ";N(SV);;",6,-0.5,5.5);
  AddHisto(histoTag+"_sv_multi",           ";N(SV);;",6,-0.5,5.5);
  AddHisto(histoTag+"_TagVarCSV_sv_mass",  ";M(SV from TagVarCSV) [GeV];;",750,0.,15.);
  AddHisto(histoTag+"_sv_mass",            ";M(SV) [GeV];;",750,0.,15.);
  AddHisto(histoTag+"_sv_chi2norm",        ";#chi^{2}/NDF(SV);;",100,0.,20.);
  AddHisto(histoTag+"_sv_tot_charge",      ";Total charge (SV);;",21,-10.5,10.5);
  AddHisto(histoTag+"_svnTrk",             ";N(tracks) : SVnVertexTracks (centered);;",   15,-0.5,14.5 );
  AddHisto(histoTag+"_svnTrk_firstVxt",    ";N(tracks): SVnFirstVertexTracks (centered);;",15,-0.5,14.5 );
  AddHisto(histoTag+"_sv_flight3Derr",     ";Flight distance error 3D;;",100,0.,1.0);
  AddHisto(histoTag+"_sv_flight2Derr",     ";Flight distance error 2D;;",100,0.,1.0);
  AddHisto(histoTag+"_sv_mass_3trk",       ";M(SV, at least 3 tracks) [GeV];;",750,0.,15.);
  //DMAddHisto(histoTag+"_sv_aboveC",        ";IP significance 2D charm",                           100,-50.,50.  );

  AddHisto(histoTag+"_track_multi"  ,";number of tracks in the jets;;",         80,-0.5,79.5);
  AddHisto(histoTag+"_trk_multi_sel",";number of selected tracks in the jets;;",80,-0.5,79.5);
  AddHisto(histoTag+"_track_chi2"   ,";normalized chi2 of the tracks;;",        200,0.,10.);
  AddHisto(histoTag+"_track_nHit"   ,";number of hits ;;",                      35,-0.5,34.5);
  AddHisto(histoTag+"_track_HPix"   ,";number of hits in the Pixel;;",          10,-0.5,9.5);

  AddHisto(histoTag+"_track_IPs"        ,";3D IP significance of all tracks;;",      200,-50.,50.);
  AddHisto(histoTag+"_track_IPs1tr"     ,";3D IP significance of the first track;;", 200,-50.,50.);
  AddHisto(histoTag+"_track_IPs2tr"     ,";3D IP significance of the second track;;",200,-50.,50.);
  AddHisto(histoTag+"_track_IP"         ,";3D IP of all tracks;;",                   200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP1tr"      ,";3D IP of the first track;;",              200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2tr"      ,";3D IP of the second track;;",             200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2Ds"      ,";2D IP significance of all tracks;;",      200,-50.,50.);
  AddHisto(histoTag+"_track_IP2Ds1tr"   ,";2D IP significance of the first track;;", 200,-50.,50.);
  AddHisto(histoTag+"_track_IP2Ds2tr"   ,";2D IP significance of the second track;;",200,-50.,50.);
  AddHisto(histoTag+"_track_IP2D"       ,";2D IP of all tracks;;",                   200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2D1tr"    ,";2D IP of the first track;;",              200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2D2tr"    ,";2D IP of the second track;;",             200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2Derr1tr" ,";2D IP error of the first track;;",        100,0,0.1);
  AddHisto(histoTag+"_track_IPerr1tr"   ,";3D IP error of the first track;;",        100,0,0.1);
  AddHisto(histoTag+"_track_IP2Derr2tr" ,";2D IP error of the second track;;",       100,0,0.1);
  AddHisto(histoTag+"_track_IPerr2tr"   ,";3D IP error of the second track;;",       100,0,0.1);
  AddHisto(histoTag+"_track_IP2Derr"    ,";2D IP error;;",                           100,0,0.1);
  AddHisto(histoTag+"_track_IPerr"      ,";3D IP error;;",                           100,0,0.1);
  AddHisto(histoTag+"_track_IPs3tr"     ,";3D IP significance of the third track;;", 200,-50.,50.);
  AddHisto(histoTag+"_track_IP3tr"      ,";3D IP of the third track;;",              200,-0.2,0.2);
  AddHisto(histoTag+"_track_IPerr3tr"   ,";3D IP error of the third track;;",        100,0,0.1);
  AddHisto(histoTag+"_track_IP2Ds3tr"   ,";2D IP significance of the second track;;",100,-35.,35.);
  AddHisto(histoTag+"_track_IP2D3tr"    ,";2D IP of the third track;;",              200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2Derr3tr" ,";2D IP error of the third track;;",        100,0,0.1);

  AddHisto(histoTag+"_track_len"     ,";track decay length;;",                       100,0,5.     );
  AddHisto(histoTag+"_track_dist"    ,";distance to the jet axis;;",                 100,0.,0.08  );
  AddHisto(histoTag+"_track_dz"      ,";transverse IP;;",                            100,-3,3     );
  AddHisto(histoTag+"_track_isfromSV",";Track is from SV;;",                         2,-0.5, 1.5  );
  AddHisto(histoTag+"_track_pt"      ,";pT of all the tracks;;",                     200,0.,1000. );
  AddHisto(histoTag+"_track_pt15"    ,";pT of all the tracks;;",                     500,0.,50.   );
  AddHisto(histoTag+"_track_chi2_cut",";normalized chi2 ;;",                         100,0.,30.   );
  AddHisto(histoTag+"_track_nHit_cut",";number of hits  ;;",                         35,-0.5, 34.5);
  AddHisto(histoTag+"_track_HPix_cut",";number of hits in the Pixel ;;",             10,-0.5, 9.5 );
  AddHisto(histoTag+"_track_len_cut" ,";track decay length passing selections;;",    100,0,25.    );
  AddHisto(histoTag+"_track_dist_cut",";distance to the jet axis ;;",                100,0.,0.3   );
  AddHisto(histoTag+"_track_dz_cut"  ,";transverse IP ;;",                           100,-20., 20.);
  AddHisto(histoTag+"_track_pt_cut"  ,";pT ;;",                                      200,0.,1000. );
  AddHisto(histoTag+"_track_pt15_cut",";pT of all the tracks;;",                     300,0.,30.   );
  AddHisto(histoTag+"_track_IP2D_cut",";IP2D ;;",                                    100,-1.,1.   );

  AddHisto(histoTag+"_TCHE"    ,";TCHE;;",50,0.,30.); 
  AddHisto(histoTag+"_TCHP"    ,";TCHP;;",50,0.,30.); 
  AddHisto(histoTag+"_JP"      ,";JP;;",50,0.,2.5);
  AddHisto(histoTag+"_JBP"     ,";JBP;;",50,0.,8.);
  AddHisto(histoTag+"_SSV"     ,";SSVHE;;",70,0.,7.);
  AddHisto(histoTag+"_SSVHP"   ,";SSVHP;;",70,0.,7.);
  AddHisto(histoTag+"_CSVIVFv2",";CSVIVFv2;;",50,0.,1.);
  AddHisto(histoTag+"_cMVAv2"  ,";cMVAv2;;",50,0.,1.);
  AddHisto(histoTag+"_DoubleB" ,";DoubleB;;",100,-1,1.);
  AddHisto2D(histoTag+"_JP_DoubleB",";JP vs DoubleB;;", 50, 0., 2.5, 100,-1,1.);  
  AddHisto2D(histoTag+"_TagVarCSV_sv_mass_DoubleB",";M(sv from TagVarCSV) vs DoubleB;;", 100,0,30, 100,-1,1.);  
  AddHisto2D(histoTag+"_JP_v_DDBvL",";JP vs DDBvL;;", 50, 0., 2.5, 100, 0 ,1.);  
  AddHisto2D(histoTag+"_TagVarCSV_sv_mass_DDBvL",";M(sv from TagVarCSV) vs DDBvL;;", 100,0,30, 100,0,1.);  
  AddHisto2D(histoTag+"_JP_v_DDCvL",";JP vs DDCvL;;", 50, 0., 2.5, 100, 0 ,1.);  
  AddHisto2D(histoTag+"_TagVarCSV_sv_mass_DDCvL",";M(sv from TagVarCSV) vs DDCvL;;", 100,0,30, 100,0,1.);  
  AddHisto2D(histoTag+"_JP_v_DDCvB",";JP vs DDCvB;;", 50, 0., 2.5, 100, 0 ,1.);  
  AddHisto2D(histoTag+"_TagVarCSV_sv_mass_DDCvB",";M(sv from TagVarCSV) vs DDCvB;;", 100,0,30, 100,0,1.);  
  AddHisto(histoTag+"_deepDoubleBvLQCD", "Jet_deepDoubleBvLQCD", 100, 0, 1 );
  AddHisto(histoTag+"_deepDoubleBvLHbb", "Jet_deepDoubleBvLHbb", 100, 0, 1 );
  AddHisto(histoTag+"_deepDoubleCvLQCD", "Jet_deepDoubleCvLQCD", 100, 0, 1 );
  AddHisto(histoTag+"_deepDoubleCvLHcc", "Jet_deepDoubleCvLHcc", 100, 0, 1 );
  AddHisto(histoTag+"_deepDoubleCvBHcc", "Jet_deepDoubleCvBHcc", 100, 0, 1 );
  AddHisto(histoTag+"_deepDoubleCvBHbb", "Jet_deepDoubleCvBHbb", 100, 0, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleBvLQCD", "Jet_massIndDeepDoubleBvLQCD", 100, 0, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleBvLHbb", "Jet_massIndDeepDoubleBvLHbb", 100, 0, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleCvLQCD", "Jet_massIndDeepDoubleCvLQCD", 100, 0, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleCvLHcc", "Jet_massIndDeepDoubleCvLHcc", 100, 0, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleCvBHcc", "Jet_massIndDeepDoubleCvBHcc", 100, 0, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleCvBHbb", "Jet_massIndDeepDoubleCvBHbb", 100, 0, 1 );
  AddHisto(histoTag+"_deepBoostedJetbbvsLight", "Jet_deepBoostedJetbbvsLight", 100, 0, 1 );
  AddHisto(histoTag+"_deepBoostedJetccvsLight", "Jet_deepBoostedJetccvsLight", 100, 0, 1 );
  AddHisto(histoTag+"_deepBoostedJetTvsQCD", "Jet_deepBoostedJetTvsQCD", 100, 0, 1 );
  AddHisto(histoTag+"_deepBoostedJetZHccvsQCD", "Jet_deepBoostedJetZHccvsQCD", 100, 0, 1 );
  AddHisto(histoTag+"_deepBoostedJetWvsQCD", "Jet_deepBoostedJetWvsQCD", 100, 0, 1 );
  AddHisto(histoTag+"_deepBoostedJetZHbbvsQCD", "Jet_deepBoostedJetZHbbvsQCD", 100, 0, 1 );
  
  AddHisto(histoTag+"_TCHE_extended1",";TCHE_extended1;;",70,-30.,30.); 
  AddHisto(histoTag+"_TCHP_extended1",";TCHP_extended1;;",70,-30.,30.); 
  AddHisto(histoTag+"_TCHE_extended2",";TCHE_extended2;;",100,-30.,30.); 
  AddHisto(histoTag+"_TCHP_extended2",";TCHP_extended2;;",100,-30.,30.); 
  AddHisto(histoTag+"_discri_ssche0" ,";SSVHE Discriminator;;",80,-1.,7.);
  AddHisto(histoTag+"_discri_sschp0" ,";SSVHP Discriminator;;",80,-1.,7.);

  AddHisto(histoTag+"_SoftMu"    ,";SoftMu;;",50,0.,1.);
  AddHisto(histoTag+"_SoftEl"    ,";SoftEl;;",50,0.,1.);
  AddHisto(histoTag+"_CombCSVSL" ,";CombCSVSL;;",50,0.,1.);
  AddHisto(histoTag+"_RetCombSvx",";RetCombSvx;;",50,0.,1.);

  AddHisto2D(histoTag+"_seltrack_vs_jetpt"         ,";sel track multiplicity vs jet pt;;",PtMax/20,0,PtMax,100,-0.5,99.5);
  AddHisto2D(histoTag+"_sv_mass_vs_flightDist3D"   ,";SVMass vs SV 3D flight distance;;",50,0, 10,60,0,6);
  AddHisto2D(histoTag+"_TagVarCSV_sv_mass_vs_jetpt",";p_{T}(jet) [GeV];M(sv from TagVarCSV);;",PtMax/20,0,PtMax,100,0,6);
  AddHisto2D(histoTag+"_avg_sv_mass_vs_jetpt"      ,";Avg SVMass vs jet pt;;",PtMax/20,0,PtMax,100,0,6);
  AddHisto2D(histoTag+"_sv_deltar_jet_vs_jetpt"    ,";SVJetDeltaR vs jet pt;;",PtMax/20,0,PtMax,50,0.,0.5);
  AddHisto2D(histoTag+"_sv_deltar_sum_jet_vs_jetpt",";SVvtxSumJetDeltaR vs jet pt;;",PtMax/20,0,PtMax,50,0.,0.5);
  AddHisto2D(histoTag+"_sv_deltar_sum_dir_vs_jetpt",";SVvtxSumVtxDirDeltaR vs jet pt;;",PtMax/20,0,PtMax,100,0.,1.0);
  AddHisto2D(histoTag+"_muon_ptrel_vs_jetpt"       ,";PFMuon_p{T}^{rel} vs jet pt;;",PtMax/20,0,PtMax,50,0,5);
  AddHisto2D(histoTag+"_muon_DeltaR_vs_jetpt"      ,";Muon1 DeltaR vs jet pt;;",PtMax/20,0,PtMax,50,0,0.5);
  AddHisto2D(histoTag+"_muon_ptrel_vs_nseltrack"   ,";PFMuon_p{T}^{rel} vs N(sel. track);;",100,0.,1.0,100,-0.5,99.5);
}

void BTagValidation::createJetHistos_DoubleB() {

  // Hbb tag vars - added by rizki - start

  AddHisto("FatJet_z_ratio"          ,";z ratio;;",100,0.,60.);
  AddHisto("FatJet_z_ratio_unw"          ,";z ratio;;",100,0.,60.);

  AddHisto("FatJet_trackSip3dSig_3",";trackSip3dSig_3;;",100,-20,20);
  AddHisto("FatJet_trackSip3dSig_2",";trackSip3dSig_2;;",100,-20,20);
  AddHisto("FatJet_trackSip3dSig_1",";trackSip3dSig_1;;",100,-20,20);
  AddHisto("FatJet_trackSip3dSig_0",";trackSip3dSig_0;;",100,-20,20);

  AddHisto("FatJet_tau1_trackSip3dSig_1",";tau1_trackSip3dSig_1;;",100,-20,20);
  AddHisto("FatJet_tau1_trackSip3dSig_0",";tau1_trackSip3dSig_0;;",100,-20,20);
  AddHisto("FatJet_tau2_trackSip3dSig_1",";tau2_trackSip3dSig_1;;",100,-20,20);
  AddHisto("FatJet_tau2_trackSip3dSig_0",";tau2_trackSip3dSig_0;;",100,-20,20);

  AddHisto("FatJet_trackSip2dSigAboveCharm_0",";trackSip2dSigAboveCharm_0;;",100,-20,20);
  AddHisto("FatJet_trackSip2dSigAboveBottom_0",";trackSip2dSigAboveBottom_0;;",100,-20,20);
  AddHisto("FatJet_trackSip2dSigAboveBottom_0_unw",";trackSip2dSigAboveBottom_0;;",100,-20,20);
  AddHisto("FatJet_trackSip2dSigAboveBottom_1",";trackSip2dSigAboveBottom_1;;",100,-20,20);

  AddHisto("FatJet_tau2_trackEtaRel_0",";tau2_trackEtaRel_0;;",400,-20,20);
  AddHisto("FatJet_tau2_trackEtaRel_1",";tau2_trackEtaRel_1;;",400,-20,20);
  AddHisto("FatJet_tau2_trackEtaRel_2",";tau2_trackEtaRel_2;;",400,-20,20);

  AddHisto("FatJet_tau1_trackEtaRel_0",";tau1_trackEtaRel_0;;",400,-20,20);
  AddHisto("FatJet_tau1_trackEtaRel_1",";tau1_trackEtaRel_1;;",400,-20,20);
  AddHisto("FatJet_tau1_trackEtaRel_2",";tau1_trackEtaRel_2;;",400,-20,20);

  AddHisto("FatJet_tau1_vertexMass"            ,";tau1_vertexMass;;",500,0.,500);
  AddHisto("FatJet_tau1_vertexEnergyRatio"      ,";tau1_vertexEnergyRatio;;",100,-1,4);
  AddHisto("FatJet_tau1_vertexEnergyRatio_unw"      ,";tau1_vertexEnergyRatio (unweighted);;",100,-1,4);
  AddHisto("FatJet_tau1_vertexDeltaR"          ,";tau1_vertexDeltaR;;",100,-1,1);
  AddHisto("FatJet_tau1_flightDistance2dSig"    ,";tau1_flightDistance2dSig;;",100,-20,20);

  AddHisto("FatJet_tau2_vertexMass"            ,";tau2_vertexMass;;",500,0.,500);
  AddHisto("FatJet_tau2_vertexEnergyRatio"      ,";tau2_vertexEnergyRatio;;",100,-1,4);
  AddHisto("FatJet_tau2_flightDistance2dSig"    ,";tau2_flightDistance2dSig;;",100,-20,20);

  AddHisto("FatJet_jetNTracks"             ,";jetNTracks;;",40,0,40);
  AddHisto("FatJet_jetNTracks_unw"             ,";jetNTracks (unweighted);;",40,0,40);
  AddHisto("FatJet_nSV"  ,";nSV;;",10,0,10);

  AddHisto("FatJet_DoubleB"    ,";DoubleB;;",100,-1.,1.);
  AddHisto("FatJet_DeepDoubleBvLQCD"     ,";DeepDoubleBvLQCD;;",100,-1.,1.);

  AddHisto2D("FatJet_DoubleB_trackSip3dSig_0"           ,";DoubleB ;IP Sig 1st Track;",100,-1.,1.,100,-20,20);
  AddHisto2D("FatJet_DoubleB_trackSip3dSig_1"           ,";DoubleB ;IP Sig 2nd Track;",100,-1.,1.,100,-20,20);
  AddHisto2D("FatJet_DoubleB_trackSip3dSig_2"           ,";DoubleB ;IP Sig 3rd Track;",100,-1.,1.,100,-20,20);
  AddHisto2D("FatJet_DoubleB_trackSip3dSig_3"           ,";DoubleB ;IP Sig 4th Track;",100,-1.,1.,100,-20,20);

  AddHisto2D("FatJet_DoubleB_jetNTracks"           ,";DoubleB ;Number of tracks;",100,-1.,1.,40,0,40);

  AddHisto2D("FatJet_jetNTracks_trackSip3dSig_0"           ,";Number of tracks ;IP Sig 1st Track ;",40,0,40,100,-20,20);

  double pi=TMath::Pi();
  
  //All region
    
  TString Tag_ = "FatJet"   ;

  AddHisto(Tag_+"_mu1_ptRatio",        "p_{T}(#mu) / p_{T}(AK8 jet) (#mu_{0}) ;;",50,0,1);  
  AddHisto(Tag_+"_mu1_ptrel",        ";p_{T,rel}(#mu) [GeV] (#mu_{0});;",50,0,5);
  AddHisto(Tag_+"_mu1_chi2",         ";#chi^{2}/NDF(#mu) (#mu_{0});;",50,0,10);
  AddHisto(Tag_+"_muon1_Pt",         ";p_{T} (#mu) [GeV] (#mu_{0});;",250,0,500);
  AddHisto(Tag_+"_muon1_eta",        ";#eta(#mu) (#mu_{0});;",50,-2.5, 2.5);
  AddHisto(Tag_+"_muon1_phi",        ";#phi(#mu)(#mu_{0}) ;;",40,-1.*pi,pi);
  AddHisto(Tag_+"_muon1_dR",        ";#Delta R(#mu,jet) (#mu_{0});;",250,0,5);
  AddHisto(Tag_+"_muon1_sj1_dR",        ";#Delta R(#mu,sj1) (#mu_{0});;",250,0,5);

  AddHisto(Tag_+"_mu2_ptRatio",        "p_{T}(#mu) / p_{T}(AK8 jet) (#mu_{1}) ;;",50,0,1);  
  AddHisto(Tag_+"_mu2_ptrel",        ";p_{T,rel}(#mu) [GeV] (#mu_{1});;",50,0,5);
  AddHisto(Tag_+"_mu2_chi2",         ";#chi^{2}/NDF(#mu) (#mu_{1}) ;;",50,0,10);
  AddHisto(Tag_+"_muon2_Pt",         ";p_{T} (#mu) [GeV] (#mu_{1}) ;;",250,0,500);
  AddHisto(Tag_+"_muon2_eta",        ";#eta(#mu)(#mu_{1}) ;;",50,-2.5, 2.5);
  AddHisto(Tag_+"_muon2_phi",        ";#phi(#mu)(#mu_{1});;",40,-1.*pi,pi);
  AddHisto(Tag_+"_muon2_dR",        ";#Delta R(#mu,jet) (#mu_{1});;",250,0,5);
  AddHisto(Tag_+"_muon2_sj2_dR",        ";#Delta R(#mu,sj2) (#mu_{0});;",250,0,5);

  AddHisto(Tag_+"_dimuon_ptRatio",        "p_{T}(#mu) / p_{T}(AK8 jet) (#mu_{0,1}) ;;",50,0,1);  
  AddHisto(Tag_+"_dimuon_Pt",         ";p_{T} (#mu) [GeV] (#mu_{0,1};;",250,0,500);
  AddHisto(Tag_+"_dimuon_eta",        ";#eta(#mu) (#mu_{0,1};;",50,-2.5, 2.5);
  AddHisto(Tag_+"_dimuon_phi",        ";#phi(#mu) (#mu_{0,1};;",40,-1.*pi,pi);
  AddHisto(Tag_+"_dimuon_dR",        ";#Delta R(#mu,jet) (#mu_{0,1});;",250,0,5);

  AddHisto("Subjet1_dR",        ";#Delta R(Subjet1,AK8jet) ;;",250,0,5);
  AddHisto("Subjet2_dR",        ";#Delta R(Subjet2,AK8jet) ;;",250,0,5);

  // added by rizki - end
}

void BTagValidation::createJetHistos_DeepDoubleX() {

  // DeepDoubleX and DeepBoostedJet tag variables
  //
  AddHisto("FatJet_DeepDoubleX_jetNTracks"             ,";jetNTracks;;",40,0,40);
  AddHisto("FatJet_DeepDoubleX_nSV"  ,";nSV;;",10,0,10);
  AddHisto("FatJet_DeepDoubleX_trackSip3dSig_0",";trackSip3dSig_0;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_trackSip3dSig_1",";trackSip3dSig_1;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_trackSip3dSig_2",";trackSip3dSig_2;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_trackSip3dSig_3",";trackSip3dSig_3;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau1_trackSip3dSig_0",";tau1_trackSip3dSig_0;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau1_trackSip3dSig_1",";tau1_trackSip3dSig_1;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau2_trackSip3dSig_0",";tau2_trackSip3dSig_0;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau2_trackSip3dSig_1",";tau2_trackSip3dSig_1;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_trackSip2dSigAboveBottom_0",";trackSip2dSigAboveBottom_0;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_trackSip2dSigAboveBottom_1",";trackSip2dSigAboveBottom_1;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_trackSip2dSigAboveCharm",";trackSip2dSigAboveCharm;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau1_trackEtaRel_0",";tau1_trackEtaRel_0;;",400,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau1_trackEtaRel_1",";tau1_trackEtaRel_1;;",400,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau1_trackEtaRel_2",";tau1_trackEtaRel_2;;",400,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau2_trackEtaRel_0",";tau2_trackEtaRel_0;;",400,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau2_trackEtaRel_1",";tau2_trackEtaRel_1;;",400,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau2_trackEtaRel_2",";tau2_trackEtaRel_2;;",400,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau1_vertexMass"            ,";tau1_vertexMass;;",500,0.,500);
  AddHisto("FatJet_DeepDoubleX_tau1_vertexEnergyRatio"      ,";tau1_vertexEnergyRatio;;",100,-1,4);
  AddHisto("FatJet_DeepDoubleX_tau1_flightDistance2dSig"    ,";tau1_flightDistance2dSig;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau1_vertexDeltaR"          ,";tau1_vertexDeltaR;;",100,-1,1);
  AddHisto("FatJet_DeepDoubleX_tau2_vertexMass"            ,";tau2_vertexMass;;",500,0.,500);
  AddHisto("FatJet_DeepDoubleX_tau2_vertexEnergyRatio"      ,";tau2_vertexEnergyRatio;;",100,-1,4);
  AddHisto("FatJet_DeepDoubleX_tau2_flightDistance2dSig"    ,";tau2_flightDistance2dSig;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_tau2_vertexDeltaR"          ,";tau2_vertexDeltaR;;",100,-1,1);
  AddHisto("FatJet_DeepDoubleX_z_ratio"          ,";z ratio;;",100,0.,60.);
  AddHisto("FatJet_DeepDoubleX_charged_EtaRel",";charged_EtaRel;;",400,-20,20);
  AddHisto("FatJet_DeepDoubleX_charged_PtRatio",";charged_PtRatio;;",100,0,1);
  AddHisto("FatJet_DeepDoubleX_charged_PParRatio",";charged_PParRatio;;",100,-1,1);
  AddHisto("FatJet_DeepDoubleX_charged_PtRatio",";charged_PtRatio;;",100,0,1);
  AddHisto("FatJet_DeepDoubleX_charged_Sip2dVal",";chargwed_Sip2dVal;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_charged_Sip2dSig",";charged_Sip2dSig;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_charged_Sip3dVal",";chargwed_Sip3dVal;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_charged_Sip3dSig",";charged_Sip3dSig;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_charged_JetDistVal",";chargwed_JetDistVal;;",100,-20,20);
  AddHisto("FatJet_DeepDoubleX_sv_d3d"           ,";sv d3d;;",100,0.,60.);
  AddHisto("FatJet_DeepDoubleX_sv_d3dsig", ";sv_d3dsig;;",100,-20,20);


  AddHisto("FatJet_DeepBoostedJet_pf_puppiw", ";pf_puppiw;;",100,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_hcalFrac", ";pf_hcalFrac;;",100,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_VTX_ass", ";pf_VTX_ass;;",10,0,10);
  AddHisto("FatJet_DeepBoostedJet_pf_lostInnerHits", ";pf_lostInnerHits;;",40,-2,2);
  AddHisto("FatJet_DeepBoostedJet_pf_quality", ";pf_quality;;",10,0,10);
  AddHisto("FatJet_DeepBoostedJet_pf_charge", ";pf_charge;;",10,-2,2);
  AddHisto("FatJet_DeepBoostedJet_pf_isEl", ";pf_isEl;;",10,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_isMu", ";pf_isMu;;",10,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_isChargedHad", ";pf_isChargedHad;;",10,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_isGamma", ";pf_isGamma;;",10,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_isNeutral", ";pf_isNeutral;;",10,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_phirel", ";pf_phirel;;",100,-1,1);
  AddHisto("FatJet_DeepBoostedJet_pf_etarel", ";pf_etarel;;",100,-1,1);
  AddHisto("FatJet_DeepBoostedJet_pf_deltaR", ";pf_deltaR;;",100,0,5);
  AddHisto("FatJet_DeepBoostedJet_pf_abseta", ";pf_abseta;;",100,0,5);
  AddHisto("FatJet_DeepBoostedJet_pf_ptrel_log", ";pf_ptrel_log;;",100,-20,20);
  AddHisto("FatJet_DeepBoostedJet_pf_erel_log", ";pf_erel_log;;",100,-20,20);
  AddHisto("FatJet_DeepBoostedJet_pf_pt_log", ";pf_pt_log;;",100,-20,20);
  AddHisto("FatJet_DeepBoostedJet_pf_drminsv", ";pf_drminsv;;",100,-1,5);
  AddHisto("FatJet_DeepBoostedJet_pf_drsubjet1", ";pf_drsubjet1;;",100,-1,5);
  AddHisto("FatJet_DeepBoostedJet_pf_drsubjet2", ";pf_drsubjet2;;",100,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_normchi2", ";pf_normchi2;;",1000,0,1000);
  AddHisto("FatJet_DeepBoostedJet_pf_dz", ";pf_dz;;",100,0,2);
  AddHisto("FatJet_DeepBoostedJet_pf_dzsig", ";pf_dzsig;;",100,-20,20);
  AddHisto("FatJet_DeepBoostedJet_pf_dxy", ";pf_dxy;;",100,-20,20);
  AddHisto("FatJet_DeepBoostedJet_pf_dxysig", ";pf_dxysig;;",100,-20,20);
  AddHisto("FatJet_DeepBoostedJet_pf_dptdpt", ";pf_dptdpt;;",1000,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_detadeta", ";pf_detadeta;;",1000,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_dphidphi", ";pf_dphidphi;;",1000,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_dxydxy", ";pf_dxydxy;;",1000,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_dzdz", ";pf_dzdz;;",1000,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_dxydz", ";pf_dxydz;;",1000,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_dphidxy", ";pf_dphidxy;;",1000,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_dlambdadz", ";pf_dlambdadz;;",1000,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_btagEtaRel", ";pf_btagEtaRel;;",100,0,10);
  AddHisto("FatJet_DeepBoostedJet_pf_btagPtRatio", ";pf_btagPtRatio;;",100,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_btagPParRatio", ";pf_btagPParRatio;;",100,0,1);
  AddHisto("FatJet_DeepBoostedJet_pf_btagSip2dVal", ";pf_btagSip2dVal;;",100,-20,20);
  AddHisto("FatJet_DeepBoostedJet_pf_btagSip2dSig", ";pf_btagSip2dSig;;",100,-20,20);
  AddHisto("FatJet_DeepBoostedJet_pf_btagSip3dVal", ";pf_btagSip3dVal;;",100,-20,20);
  AddHisto("FatJet_DeepBoostedJet_pf_btagSip3dSig", ";pf_btagSip3dSig;;",100,-20,20);
  AddHisto("FatJet_DeepBoostedJet_pf_btagJetDistVal", ";pf_btagJetDistVal;;",100,0,1);
  AddHisto("FatJet_DeepBoostedJet_sv_phirel", ";sv_phirel;;",100,-1,1);
  AddHisto("FatJet_DeepBoostedJet_sv_etarel", ";sv_etarel;;",100,-1,1);
  AddHisto("FatJet_DeepBoostedJet_sv_deltaR", ";sv_deltaR;;",100,0,5);
  AddHisto("FatJet_DeepBoostedJet_sv_abseta", ";sv_abseta;;",100,0,5);
  AddHisto("FatJet_DeepBoostedJet_sv_mass", ";sv_mass;;",100,0,100);
  AddHisto("FatJet_DeepBoostedJet_sv_ptrel_log", ";sv_ptrel_log;;",100,-10,1);
  AddHisto("FatJet_DeepBoostedJet_sv_erel_log", ";sv_erel_log;;",100,-10,1);
  AddHisto("FatJet_DeepBoostedJet_sv_pt_log", ";sv_pt_log;;",100,0,10);
  AddHisto("FatJet_DeepBoostedJet_sv_ntracks", ";sv_ntracks;;",30,0,30);
  AddHisto("FatJet_DeepBoostedJet_sv_normchi2", ";sv_normchi2;;",100,0,100);
  AddHisto("FatJet_DeepBoostedJet_sv_dxy", ";sv_dxy;;",100,0,100);
  AddHisto("FatJet_DeepBoostedJet_sv_dxysig", ";sv_dxysig;;",1000,0,1000);
  AddHisto("FatJet_DeepBoostedJet_sv_d3d", ";sv_d3d;;",100,0,100);
  AddHisto("FatJet_DeepBoostedJet_sv_d3dsig", ";sv_d3dsig;;",100,0,1);
  AddHisto("FatJet_DeepBoostedJet_sv_costhetasvpv", ";sv_costhetasvpv;;",100,-1,1);
}

void BTagValidation::createJetHistos_SF(const TString& histoTag, const TString& LTSVvar,  
                                        const TString& tagger, const std::map<TString,double>& WPmap,
                                        const std::map<TString,std::vector<int>>& ptbinmap) {

    std::vector<TString> ptStr;
    boost::copy(ptbinmap | boost::adaptors::map_keys, std::back_inserter(ptStr));
    std::vector<TString> WPStr;
    boost::copy(WPmap | boost::adaptors::map_keys, std::back_inserter(WPStr));
    float low_tag_bound = 0.;
    if (tagger == TString("DoubleB")) low_tag_bound  = -1.;
    float high_LTSV_bound = 2.5;
    if ( LTSVvar == TString("tau1VertexMassCorr")) high_LTSV_bound = 30.;
  
    for(unsigned int ipt=0; ipt < ptStr.size(); ipt++ ){
        
        if(DEBUG_ && DEBUGlevel_>1 )std::cout << "  JP notag "  << std::endl;
        AddHisto(histoTag+"_"+LTSVvar+"_all_"+ptStr.at(ipt), ";"+LTSVvar+"_all_"+ptStr.at(ipt)+";;", 50, 0., high_LTSV_bound);
        for(unsigned int iWP=0; iWP < WPStr.size(); iWP++ ){
            if(DEBUG_ && DEBUGlevel_>1 )std::cout << "  " << "WP = " << WPStr.at(iWP) << ", no JEC"<<std::endl;

            AddHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_pass_"+ptStr.at(ipt), LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_pass_"+ptStr.at(ipt), 50, 0., high_LTSV_bound);
            AddHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_fail_"+ptStr.at(ipt), LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_fail_"+ptStr.at(ipt), 50, 0., high_LTSV_bound);
        }

        AddHisto(histoTag+"_"+LTSVvar+"_all_"+ptStr.at(ipt)+"_JESup", ";"+LTSVvar+"_all_"+ptStr.at(ipt)+"_JESup"+";;", 50, 0., high_LTSV_bound);
        for(unsigned int iWP=0; iWP < WPStr.size(); iWP++ ){
            if(DEBUG_ && DEBUGlevel_>1 )std::cout << "  " << "WP = " << WPStr.at(iWP) << " JESup"<<std::endl;

            AddHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_pass_"+ptStr.at(ipt)+"_JESup", LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_pass_"+ptStr.at(ipt)+"_JESup", 50, 0., high_LTSV_bound);
            AddHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_fail_"+ptStr.at(ipt)+"_JESup", LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_fail_"+ptStr.at(ipt)+"_JESup", 50, 0., high_LTSV_bound);

        }

        AddHisto(histoTag+"_"+LTSVvar+"_all_"+ptStr.at(ipt)+"_JESdown", ";"+LTSVvar+"_all_"+ptStr.at(ipt)+"_JESdown"+";;", 50, 0., high_LTSV_bound);
        for(unsigned int iWP=0; iWP < WPStr.size(); iWP++ ){
            if(DEBUG_ && DEBUGlevel_>1 )std::cout << "  " << "WP = " << WPStr.at(iWP) << " JESdown"<<std::endl;

            AddHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_pass_"+ptStr.at(ipt)+"_JESdown", LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_pass_"+ptStr.at(ipt)+"_JESdown", 50, 0., high_LTSV_bound);
            AddHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_fail_"+ptStr.at(ipt)+"_JESdown", LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_fail_"+ptStr.at(ipt)+"_JESdown", 50, 0., high_LTSV_bound);

        }

    }

  // for SF LT method calculation - end
}


// ------------ method called for each event  ------------
void BTagValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;

  if(JetTree == 0 && JetTreeEvtInfo == 0) return;

  Long64_t nEntries = JetTree->GetEntries();
  if(maxEvents_>=0) nEntries = maxEvents_;

  float BTemplateCorrections[100][20][2];


  if (doBFrag_)
  {
    if(DEBUG_)std::cout << "Running BFrag systematics. Initializing weights for BFrag systematics, loading files from: "+file_BFrag_ << std::endl; 
    int nPtRelPtBins = 15;

    TString PtRelPtBin[] = {
      "Pt2030", "Pt3040", "Pt4050", "Pt5060", "Pt6070"
        , "Pt7080", "Pt80100", "Pt100120", "Pt120160", "Pt160210"
        , "Pt210260", "Pt260320", "Pt320400", "Pt400500", "Pt500" 
    };

    for( int ptb=0;ptb<nPtRelPtBins;ptb++ )
    {      
      for(int ib=0;ib<100;ib++ )
      {
        BTemplateCorrections[ib][ptb][0] = 1.;
        BTemplateCorrections[ib][ptb][1] = 1.;
      }
      std::ifstream MnusCorrectionsFile;      
//       MnusCorrectionsFile.open("/afs/cern.ch/work/a/asady/rizki_test/CMSSW_7_6_3/src/RecoBTag/BTagValidation/test/PtRelFall12/EnergyFraction_" + PtRelPtBin[ptb] + "_m5.txt"); //Make sure files are in place and up-to-date!
      MnusCorrectionsFile.open(file_BFrag_ + "/" + "EnergyFraction_" + PtRelPtBin[ptb] + "_m5.txt"); //Make sure files are in place and up-to-date!
      while( MnusCorrectionsFile )
      {
        float xBin, efcorr;
        MnusCorrectionsFile >> xBin >> efcorr;
        if ( efcorr > 4. ) efcorr = 1.;
        int ib = int(xBin/0.02);
        BTemplateCorrections[ib][ptb][0] = efcorr;
        if(DEBUG_) std::cout << "Calculating BTemplateCorrections["<<ib<<"]["<<ptb<<"][0] (BFragDown)"<< BTemplateCorrections[ib][ptb][0] << std::endl;
;
      }

      std::ifstream PlusCorrectionsFile; 
//       PlusCorrectionsFile.open("/afs/cern.ch/work/a/asady/rizki_test/CMSSW_7_6_3/src/RecoBTag/BTagValidation/test/PtRelFall12/EnergyFraction_" + PtRelPtBin[ptb] + "_p5.txt"); //Make sure files are in place and up-to-date!
      PlusCorrectionsFile.open(file_BFrag_ + "/" + "EnergyFraction_" + PtRelPtBin[ptb] + "_p5.txt"); //Make sure files are in place and up-to-date!
      while( PlusCorrectionsFile )
      {
        float xBin, efcorr;
        PlusCorrectionsFile >> xBin >> efcorr;
        if ( efcorr > 4. ) efcorr = 1.;
        int ib = int(xBin/0.02);
        BTemplateCorrections[ib][ptb][1] = efcorr;
        if(DEBUG_) std::cout << "Calculating BTemplateCorrections["<<ib<<"]["<<ptb<<"][1] (BFragUp)"<< BTemplateCorrections[ib][ptb][1] << std::endl;
      }
    }
  }

  //---------------------------- Start event loop ---------------------------------------//
  for(Long64_t iEntry = 0; iEntry < nEntries; ++iEntry) {
    JetTreeEvtInfo->GetEntry(iEntry);

    JetTree->GetEntry(iEntry);

    int run = EvtInfo.Run ; 
    int lumi = EvtInfo.LumiBlock ; 
    if((iEntry%reportEvery_) == 0) edm::LogInfo("EventNumber") << ">>>>> Processing event with run no. " << run << " and lumisection " << lumi << " Evt number: " <<  iEntry << " of " << nEntries;

    if(run < 0) {
      isData = false;
      if ( iEntry == 0) edm::LogInfo("IsMC") << ">>>>> Running on simulation\n" ;
    }
    else if( iEntry == 0 ) edm::LogInfo("IsData") << ">>>>> Running on data\n" ;

    if(isData && useRunRange_){
        if(DEBUG_) std::cout << "Restricting by run range. Run:" << run <<std::endl;
        if( run < runRangeMin_ || run > runRangeMax_ ){
            if (DEBUG_) std::cout <<    "Run is outside range (" << runRangeMin_ <<" - "<< runRangeMax_ <<" ). Skipping run." << std::endl;
            continue;
        }
    }

    double wtPU = 1.;
    if ( doPUReweightingOfficial_ && !isData )
      wtPU *= LumiWeights_.weight(EvtInfo.nPUtrue);
    else if ( doPUReweightingNPV_ && !isData ) 
      wtPU *= GetLumiWeightsPVBased(file_PVWt_, hist_PVWt_, EvtInfo.nPV) ;  
    h1_CutFlow->Fill(2.,wtPU); //// count all events
    h1_CutFlow_unw->Fill(2.);

    //edm::LogInfo("PUWeight") << " EvtInfo.nPUtrue: " << EvtInfo.nPUtrue << " wtPU: " << wtPU ;

    if( !isData ) h1_pt_hat->Fill(EvtInfo.pthat,wtPU);

    //if(DEBUG_)std::cout << "Event loop:" << iEntry << std::endl;
    if( !passTrigger() ) continue; //// apply trigger selection
    if(DEBUG_)std::cout << "Event selection: pass Trigger "<< std::endl;

    h1_CutFlow->Fill(3.,wtPU); //// count events passing trigger selection
    h1_CutFlow_unw->Fill(3.);

    /*    hcheck_run->Fill(run);
          hcheck_id->Fill(iEntry);
          */
    //// pileup distributions
    if( isData )
      h1_nPV_data->Fill(EvtInfo.nPV);
    else {
      h1_nPUtrue_mc    ->Fill(EvtInfo.nPUtrue,wtPU);
      h1_nPUtrue_mc_unw->Fill(EvtInfo.nPUtrue);
      h1_nPV_mc        ->Fill(EvtInfo.nPV,wtPU);
      h1_nPV_mc_unw    ->Fill(EvtInfo.nPV);
    }

    if( !isData && EvtInfo.nPUtrue < 2) continue;  //// in 2017 some MC events contain nPUTrue=0
    if(FatJetInfo.nJet <= 0) continue; //// require at least 1 fat jet in the event
    if(DEBUG_)std::cout << "Event selection: pass Fatjet>0 "<< std::endl;
    int nFatJet = 0;
    int nSubJet = 0;


    //---------------------------- Start fat jet loop ---------------------------------------//
    bool firstJet=true;
    bool total_removedProblemJet=0;
    for(int iJet = 0; iJet < FatJetInfo.nJet; ++iJet) {
      if(DEBUG_)std::cout << "Fatjet selection: in fatjet loop, FatJetInfo.Jet_pt["<<iJet<<"] = "<< FatJetInfo.Jet_pt[iJet] << std::endl;

      //double jetpt(FatJetInfo.Jet_pt[iJet]) ;
      //double jeteta(FatJetInfo.Jet_eta[iJet]) ;

      /*      hcheck_jetpt->Fill(jetpt);
              hcheck_jeteta->Fill(jeteta);
              */

      if ( FatJetInfo.Jet_pt[iJet] < fatJetPtMin_ ||
          FatJetInfo.Jet_pt[iJet] >= fatJetPtMax_ )                  continue; //// apply jet pT cut
      if(DEBUG_)std::cout << "Fatjet selection: pass pT cut "<< std::endl;

      bool problemJet = false;      
      if (removeProblemJet_ && !isData && EvtInfo.pthat < 300. && FatJetInfo.Jet_pt[iJet] > EvtInfo.pthat * 7.) problemJet = true;
      if (removeProblemJet_ && !isData && FatJetInfo.nJet > 15) problemJet = true;
      if (removeProblemJet_ && !isData && FatJetInfo.Jet_pt[iJet] > 5000. ) problemJet = true;
      if(problemJet) total_removedProblemJet++;
      if(DEBUG_ && problemJet && removeProblemJet_ && !isData)std::cout << "Fatjet selection: ---------------------------------> removed (problematic) Jet! "<< std::endl;
      if(DEBUG_ && removeProblemJet_ && !isData)std::cout << "Fatjet selection: done with problemJet "<< std::endl;

      if ( fabs(FatJetInfo.Jet_eta[iJet]) > fatJetAbsEtaMax_ )       continue; //// apply jet eta cut
      if(DEBUG_)std::cout << "Fatjet selection: pass eta cut "<< std::endl;
      if ( FatJetInfo.Jet_tightID[iJet]==0 )                         continue; //// apply tight/loose jet ID
      if(DEBUG_)std::cout << "Fatjet selection: pass jet tight ID "<< std::endl;

      if (usePrunedMass_) {
        if ( FatJetInfo.Jet_massPruned[iJet] < fatJetGroomedMassMin_ ||
            FatJetInfo.Jet_massPruned[iJet] > fatJetGroomedMassMax_ )  continue; //// apply pruned jet mass cut
      }
      else if (useSoftDropMass_) {
        if ( FatJetInfo.Jet_massSoftDrop[iJet] < fatJetGroomedMassMin_ ||
            FatJetInfo.Jet_massSoftDrop[iJet] > fatJetGroomedMassMax_ )  continue; //// apply softdrop jet mass cut
      }
      if(DEBUG_)std::cout << "Fatjet selection: pass groomed mass cut "<< std::endl;

      //added by rizki - start
      float tau1 = FatJetInfo.Jet_tau1[iJet];
      float tau2 = FatJetInfo.Jet_tau2[iJet];
      float tau21 = (tau1!=0 ? tau2/tau1 : 0.);
      if ( tau21 > fatJetTau21Max_ ||tau21 < fatJetTau21Min_ ) continue ; ////apply jet substructure tau21 cut.
      if(DEBUG_)std::cout << "Fatjet selection: pass tau2/tau1 cut "<< std::endl;
      //added by rizki - end

      int idxFirstMuon = -1;
      int nselmuon = 0;
      int nmu = 0;

      if (FatJetInfo.nPFMuon>0) {
        for (int iMu=0; iMu<FatJetInfo.nPFMuon; ++iMu) {
          if (FatJetInfo.PFMuon_IdxJet[iMu]==iJet ) {
            ++nmu;
            if (passMuonSelection(iMu, FatJetInfo, iJet, 0.8)) {
              if(nselmuon == 0) idxFirstMuon = iMu;
              ++nselmuon;
            }
          }
        }
      }

      TLorentzVector jet_p4;
      jet_p4.SetPtEtaPhiM(FatJetInfo.Jet_pt[iJet], FatJetInfo.Jet_eta[iJet], FatJetInfo.Jet_phi[iJet], FatJetInfo.Jet_mass[iJet]);

      if ( SubJetInfo.Jet_nSubJets[iJet] != 2 ) continue ;
      if(DEBUG_)std::cout << "Fatjet selection: pass nsubjets ==2"<< std::endl;

      int iSubJet1 = SubJetInfo.SubJetIdx[SubJetInfo.Jet_nFirstSJ[iJet]] ; //added by rizki
      int iSubJet2 = SubJetInfo.SubJetIdx[SubJetInfo.Jet_nFirstSJ[iJet]+1] ; //added by rizki


      //// If  processing subjets, discard fat jet with any one subjet having pt = 0
      if( (usePrunedSubjets_ || useSoftDropSubjets_)
          && (SubJets.Jet_pt[iSubJet1]==0. || SubJets.Jet_pt[iSubJet2]==0.) ) continue;
      if(DEBUG_)std::cout << "Fatjet selection: subjets pt > 0"<< std::endl;

      TLorentzVector subjet1_p4, subjet2_p4;
      subjet1_p4.SetPtEtaPhiM(SubJets.Jet_pt[iSubJet1], SubJets.Jet_eta[iSubJet1], SubJets.Jet_phi[iSubJet1], SubJets.Jet_mass[iSubJet1]);
      subjet2_p4.SetPtEtaPhiM(SubJets.Jet_pt[iSubJet2], SubJets.Jet_eta[iSubJet2], SubJets.Jet_phi[iSubJet2], SubJets.Jet_mass[iSubJet2]);

      //double fatjet_subjet1_dR = jet_p4.DeltaR(subjet1_p4);
      //double fatjet_subjet2_dR = jet_p4.DeltaR(subjet2_p4);
      double subjet_dR = subjet1_p4.DeltaR(subjet2_p4);
      //double subjet_deta = subjet1_p4.Eta() - subjet2_p4.Eta() ;
      double subjet_dy = subjet1_p4.Rapidity() - subjet2_p4.Rapidity() ;
      double subjet_dphi = subjet1_p4.DeltaPhi(subjet2_p4); ;
      double subjet_dyphi = sqrt( subjet_dy*subjet_dy + subjet_dphi*subjet_dphi ) ;

      ////// If processing subjets, skip fat jets for which the subjets are separated by dR>0.8
      //////if( (usePrunedSubjets_ || useSoftDropSubjets_)  && subjet_dR>fatJetCone ) continue;

      //// If processing subjets, skip infrared unsafe configurations
      if( (usePrunedSubjets_ || useSoftDropSubjets_)
          && subjet_dR < (FatJetInfo.Jet_mass[iJet]/FatJetInfo.Jet_pt[iJet]) ) continue;
      if(DEBUG_)std::cout << "Fatjet selection: pass infrared unsafe config "<< std::endl;

      bool isDoubleMuonTagged = false;

      int idxMuon_inFirstSubjet = -1; //1st Muon tagged (in double muon tagged fatjets) - //added by rizki
      int idxMuon_inSecondSubjet = -1; //2nd Muon tagged (in double muon tagged fatjets) - //added by rizki

      int nselmuon_sj1 = 0, nselmuon_sj2 =0;

      if ( (fatJetDoubleTagging_ || applyFatJetMuonTaggingV2_) && SubJets.nPFMuon>0) {
        //// Collect all muons matched to the two subjets
        std::vector<int> selectedMuonIdx1, selectedMuonIdx2;

        for (int iMu=0; iMu<SubJets.nPFMuon; ++iMu) {
          if ( SubJets.PFMuon_IdxJet[iMu]==iSubJet1 ) {
            if (passMuonSelection(iMu, SubJets, iSubJet1, (dynamicMuonSubJetDR_ ? subjet_dR/2 : 0.4 ))){
              selectedMuonIdx1.push_back(iMu);
              nselmuon_sj1++;
            }
          }
          if ( SubJets.PFMuon_IdxJet[iMu]==iSubJet2 ) {
            if (passMuonSelection(iMu, SubJets, iSubJet2, (dynamicMuonSubJetDR_ ? subjet_dR/2 : 0.4 ))){
              selectedMuonIdx2.push_back(iMu);
              nselmuon_sj2++;
            }
          }

        }

        //// Check that there are at least two distinct muons matched to the two subjets
        for(unsigned int iMu1=0; iMu1<selectedMuonIdx1.size(); ++iMu1) {
          for(unsigned int iMu2=0; iMu2<selectedMuonIdx2.size(); ++iMu2) {
            if( fabs( SubJets.PFMuon_eta[selectedMuonIdx1.at(iMu1)] - SubJets.PFMuon_eta[selectedMuonIdx2.at(iMu2)] ) > 1.E-03 &&
                fabs( SubJets.PFMuon_phi[selectedMuonIdx1.at(iMu1)] - SubJets.PFMuon_phi[selectedMuonIdx2.at(iMu2)] ) > 1.E-03 &&
                fabs( SubJets.PFMuon_pt[selectedMuonIdx1.at(iMu1)] - SubJets.PFMuon_pt[selectedMuonIdx2.at(iMu2)] ) > 1.E-03 ) {
              isDoubleMuonTagged = true;
              break;
            }
          }
          if( isDoubleMuonTagged ) break;
        }

        if(selectedMuonIdx1.size()>0)idxMuon_inFirstSubjet =  selectedMuonIdx1.at(0); //warning this might not be correct if mu not ordered in pT! // added by rizki
        if(selectedMuonIdx2.size()>0)idxMuon_inSecondSubjet = selectedMuonIdx2.at(0); //warning this might not be correct if mu not ordered in pT! // added by rizki

        //         if(selectedMuonIdx1.size()>0)edm::LogInfo("Check idxMuon_in1stSubjets") << "in1stSubjet: " << selectedMuonIdx1.at(0);
        //         if(selectedMuonIdx2.size()>0)edm::LogInfo("Check idxMuon_in2ndSubjets") << "in2stSubjet: " << selectedMuonIdx2.at(0);

        //degug - start - rizki
        if(isDoubleMuonTagged){
                  if(DEBUG_)std::cout << " idxMuon_inFirstSubjet = " << idxMuon_inFirstSubjet << std::endl;
                  if(DEBUG_)std::cout << " idxMuon_inSecondSubjet = " << idxMuon_inSecondSubjet << std::endl;
                  if(DEBUG_)std::cout << " selectedMuonIdx1.size() = " << selectedMuonIdx1.size() << std::endl;
                  if(DEBUG_)std::cout << " selectedMuonIdx2.size() = " << selectedMuonIdx2.size() << std::endl;
        }
        //debug - end - rizki

      }

      if( applyFatJetMuonTagging_ || applyFatJetMuonTaggingV2_ ) { //// if enabled, select muon-tagged fat jets
        if( fatJetDoubleTagging_ && !isDoubleMuonTagged ) continue;
        else if( !fatJetDoubleTagging_ && nselmuon==0)    continue;
        else if( applyFatJetMuonTaggingV2_ && !fatJetDoubleTagging_ && nselmuon_sj1==0 && nselmuon_sj2==0 )    continue; //require one of the subjets has a muon

      }
      if(DEBUG_)std::cout << "Fatjet selection: pass muon tagging "<< std::endl;

      if( applyFatJetBTagging_ ) //// if enabled, select b-tagged fat jets
      {
        if( fatJetDoubleBTagging_ && 
            !( SubJets.Jet_CombIVF[iSubJet1]>subJetBDiscrMin_ && SubJets.Jet_CombIVF[iSubJet1]<subJetBDiscrMax_ 
              && SubJets.Jet_CombIVF[iSubJet2]>subJetBDiscrMin_ && SubJets.Jet_CombIVF[iSubJet2]<subJetBDiscrMax_ ) 
          ) continue;
        else if( !fatJetDoubleBTagging_ && FatJetInfo.Jet_CombIVF[iJet]<=fatJetBDiscrCut_ ) continue;
      }

      if (fatJetDoubleSVBTagging_ && 
          (FatJetInfo.Jet_DoubleSV[iJet] < fatJetDoubleSVBDiscrMin_ || FatJetInfo.Jet_DoubleSV[iJet] > fatJetDoubleSVBDiscrMax_) 
         ) continue ;  


      //---------------------------------------------------------------------------------------------------      
      //Extra measures to remove isolated muons (for double muon tagged jets. Calculate pt(mu1+mu2)/pt(ak8)       
      //---------------------------------------------------------------------------------------------------     

      float muon1_eta;
      float muon1_phi;
      float muon1_pt;      
      TLorentzVector muon1_v;

      float muon2_eta;
      float muon2_phi;
      float muon2_pt;
      TLorentzVector muon2_v;

      float dimuon_eta;
      float dimuon_phi;
      float dimuon_pt;
      float dimuon_ptRatio; // muon pt / jet pt 
      float dimuon_dR; 
      TLorentzVector dimuon_v;
      
      for (int iMu=0; iMu<SubJets.nPFMuon; ++iMu) {
        if ( SubJets.PFMuon_IdxJet[iMu]==iSubJet1 ) {
          muon1_eta = SubJets.PFMuon_eta[idxMuon_inFirstSubjet]; 
          muon1_phi = SubJets.PFMuon_phi[idxMuon_inFirstSubjet]; 
          muon1_pt = SubJets.PFMuon_pt[idxMuon_inFirstSubjet]; 
          muon1_v.SetPtEtaPhiM(muon1_pt,muon1_eta,muon1_phi,0);
        }
        if ( SubJets.PFMuon_IdxJet[iMu]==iSubJet2 ) {
          muon2_eta = SubJets.PFMuon_eta[idxMuon_inSecondSubjet]; 
          muon2_phi = SubJets.PFMuon_phi[idxMuon_inSecondSubjet]; 
          muon2_pt = SubJets.PFMuon_pt[idxMuon_inSecondSubjet]; 
          muon2_v.SetPtEtaPhiM(muon2_pt,muon2_eta,muon2_phi,0);
        }
      }    
      dimuon_v = muon1_v + muon2_v;
      dimuon_eta = dimuon_v.Eta();
      dimuon_phi = dimuon_v.Phi();
      dimuon_pt = dimuon_v.Pt();

      dimuon_ptRatio = dimuon_pt / FatJetInfo.Jet_pt[iJet];

      //       TLorentzVector jet_v,sj1_v,sj2_v;
      //       jet_v.SetPtEtaPhiM(FatJetInfo.Jet_pt[iJet], FatJetInfo.Jet_eta[iJet], FatJetInfo.Jet_phi[iJet], FatJetInfo.Jet_mass[iJet]);      
      //       sj1_v.SetPtEtaPhiM(SubJetInfo.Jet_pt[SubJetInfo.Jet_nFirstSJ[iJet]], SubJetInfo.Jet_eta[SubJetInfo.Jet_nFirstSJ[iJet]], SubJetInfo.Jet_phi[SubJetInfo.Jet_nFirstSJ[iJet]], SubJetInfo.Jet_mass[SubJetInfo.Jet_nFirstSJ[iJet]]);
      //       sj2_v.SetPtEtaPhiM(SubJetInfo.Jet_pt[SubJetInfo.Jet_nFirstSJ[iJet]+1], SubJetInfo.Jet_eta[SubJetInfo.Jet_nFirstSJ[iJet]+1], SubJetInfo.Jet_phi[SubJetInfo.Jet_nFirstSJ[iJet]+1], SubJetInfo.Jet_mass[SubJetInfo.Jet_nFirstSJ[iJet]+1]);

      dimuon_dR = dimuon_v.DeltaR(jet_p4);


      /*      hcheck_mu1pt->Fill(FatJetInfo.PFMuon_pt[idxMuon_inFirstSubjet]);
              hcheck_mu1eta->Fill(FatJetInfo.PFMuon_eta[idxMuon_inFirstSubjet]);
              hcheck_mu2pt->Fill(FatJetInfo.PFMuon_pt[idxMuon_inSecondSubjet]);
              hcheck_mu2eta->Fill(FatJetInfo.PFMuon_eta[idxMuon_inSecondSubjet]);
              hcheck_ptratioComb->Fill(dimuon_ptRatio);
              */
      //       std::cout << "muon1 dR =" << muon1_v.DeltaR(jet_p4) << std::endl;
      //       std::cout << "muon2 dR =" << muon2_v.DeltaR(jet_p4) << std::endl;
      //       std::cout << "mu12 dR =" << dimuon_dR << std::endl;

        if(isDoubleMuonTagged==true){
          if (dimuon_ptRatio >0.6) continue; //remove spike in final bb discriminator
          if(DEBUG_)std::cout << "Fatjet selection: dimuon_ptRatio cut "<< std::endl;
        }

      //-------------------------------------------------------------------------------------------------------------      
      //  Apply Scale factors and reweightings      
      //-------------------------------------------------------------------------------------------------------------     

      //// apply b-tagging scale factors
      double wtFatJet = 1.;
      if( applySFs_ && !isData ) {
        if( applyFatJetBTagging_ && fatJetDoubleBTagging_ ) {
          //wtFatJet *= ( scaleFactor(SubJets.Jet_flavour[iSubJet1], SubJets.Jet_pt[iSubJet1], SubJets.Jet_eta[iSubJet1], (subJetBDiscrMin_>0.25)) *
          //    scaleFactor(SubJets.Jet_flavour[iSubJet2], SubJets.Jet_pt[iSubJet2], SubJets.Jet_eta[iSubJet2], (subJetBDiscrMin_>0.25)) );
          wtFatJet *= reader.eval(BTagEntry::JetFlavor(SubJets.Jet_flavour[iSubJet1]), SubJets.Jet_eta[iSubJet1], SubJets.Jet_pt[iSubJet1]); 
          wtFatJet *= reader.eval(BTagEntry::JetFlavor(SubJets.Jet_flavour[iSubJet2]), SubJets.Jet_eta[iSubJet2], SubJets.Jet_pt[iSubJet2]); 
        }
        else if( applyFatJetBTagging_ && !fatJetDoubleBTagging_ )
          //wtFatJet *= scaleFactor(FatJetInfo.Jet_flavour[iJet], FatJetInfo.Jet_pt[iJet], FatJetInfo.Jet_eta[iJet], (fatJetBDiscrCut_>0.25));
          wtFatJet *= reader.eval(BTagEntry::JetFlavor(FatJetInfo.Jet_flavour[iJet]), FatJetInfo.Jet_eta[iJet], FatJetInfo.Jet_pt[iJet]); 
      }

      //added by Erich - jetPt reweighting factor
      double wtJetPt = 1.;
      //       if (doFatJetPtReweighting_ && !isData && FatJetInfo.Jet_nbHadrons[iJet] > 1 ) { //added by rizki only temporarily for Hbb tagger signal vs proxy studies. Want to only reweight jets of bgromgsp flavour.
      if (DEBUG_) edm::LogInfo("Check") << " doFatJetPtReweighting_ = " << doFatJetPtReweighting_ << ",!isData = " << !isData ;
      if (doFatJetPtReweighting_ && !isData) { //original UNCOMMENT!
        wtJetPt *= GetWeightsJetPtBased(file_FatJetPtWt_, hist_FatJetPtWt_, FatJetInfo.Jet_pt[iJet]) ;
        if (DEBUG_) edm::LogInfo("jetPtWt") << " jetPtWt = " << wtJetPt ;
        wtFatJet *= wtJetPt ;
      }

      if (doNtracksReweighting_ && !isData) {
        wtJetPt *= GetWeightsNtracksBased(file_NtracksWt_, hist_NtracksWt_, FatJetInfo.Jet_ntracks[iJet]) ;
        wtFatJet *= wtJetPt ;
      }

      //added by Rizki - subjet_ptBalance reweighting factor
      double wtSubJetPtBalance = 1.;
      if (doSubJetPtBalanceReweighting_ && !isData && FatJetInfo.Jet_nbHadrons[iJet] > 1 ) { //added by rizki for Hbb tagger signal vs proxy studies. Want to only reweight jets of bgromgsp flavour.
        wtSubJetPtBalance *= GetWeightsSubJetPtBalanceBased(file_SubJetPtBalanceWt_, hist_SubJetPtBalanceWt_, SubJets.Jet_pt[iSubJet1] / (SubJets.Jet_pt[iSubJet1] + SubJets.Jet_pt[iSubJet2]) ) ;
        wtFatJet *= wtSubJetPtBalance ;
      }

      //added by Rizki - mass reweighting factor
      double wtMassSoftDrop = 1.;
      double wtMassSoftDrop_unw = 1.;
      if (doMassSoftDropReweighting_ && !isData && FatJetInfo.Jet_nbHadrons[iJet] > 1 ) { //added by rizki for Hbb tagger signal vs proxy studies. Want to only reweight jets of bgromgsp flavour.
        wtMassSoftDrop *= GetWeightsMassSoftDropBased(file_MassSoftDropWt_, hist_MassSoftDropWt_, FatJetInfo.Jet_massSoftDrop[iJet] ) ;
        wtMassSoftDrop_unw = wtFatJet;
        wtFatJet *= wtMassSoftDrop ;
      }
      // edm::LogInfo("MassSoftDropReweighting") << "massSoftDrop = " << FatJetInfo.Jet_massSoftDrop[iJet] << ", wtMassSoftDrop = " << wtMassSoftDrop ;

      //added by Rizki - jetNTracks reweighting factor
      double wtJetNTracks = 1.;
      double wtJetNTracks_unw = 1.;
      if (doJetNTracksReweighting_ && !isData && FatJetInfo.Jet_nbHadrons[iJet] > 1 ) { //added by rizki for Hbb tagger signal vs proxy studies. Want to only reweight jets of bgromgsp flavour.
        wtJetNTracks *= GetWeightsJetNTracksBased(file_JetNTracksWt_, hist_JetNTracksWt_, FatJetInfo.TagVarCSV_jetNTracks[iJet] ) ;
        wtJetNTracks_unw = wtFatJet;
        wtFatJet *= wtJetNTracks ;
      }
      // edm::LogInfo("JetNTracksReweighting") << "JetNTracks = " << FatJetInfo.TagVarCSV_jetNTracks[iJet] << ", wtJetNTracks = " << wtJetNTracks ;

      //added by Rizki - SV1EnergyRatio reweighting factor
      double wtSV1EnergyRatio = 1.;
      double wtSV1EnergyRatio_unw = 1.;
      if (doSV1EnergyRatioReweighting_ && !isData && FatJetInfo.Jet_nbHadrons[iJet] > 1 ) { //added by rizki for Hbb tagger signal vs proxy studies. Want to only reweight jets of bgromgsp flavour.
        wtSV1EnergyRatio *= GetWeightsSV1EnergyRatioBased(file_SV1EnergyRatioWt_, hist_SV1EnergyRatioWt_, FatJetInfo.Jet_tau1_vertexEnergyRatio[iJet] ) ;
        wtSV1EnergyRatio_unw = wtFatJet;
        wtFatJet *= wtSV1EnergyRatio ;
      }
      // edm::LogInfo("SV1EnergyRatioReweighting") << "SV1 Energy ratio = " << FatJetInfo.Jet_tau1_vertexEnergyRatio[iJet] << ", wtSV1EnergyRatio = " << wtSV1EnergyRatio ;

      //added by Rizki - IPSig1stAboveB reweighting factor
      double wtIPSig1stAboveB = 1.;
      double wtIPSig1stAboveB_unw = 1.;
      if (doIPSig1stAboveBReweighting_ && !isData && FatJetInfo.Jet_nbHadrons[iJet] > 1 ) { //added by rizki for Hbb tagger signal vs proxy studies. Want to only reweight jets of bgromgsp flavour.
        wtIPSig1stAboveB *= GetWeightsIPSig1stAboveBBased(file_IPSig1stAboveBWt_, hist_IPSig1stAboveBWt_, FatJetInfo.Jet_trackSip2dSigAboveBottom_0[iJet] ) ;
        wtIPSig1stAboveB_unw = wtFatJet;
        wtFatJet *= wtIPSig1stAboveB ;
      }
      // edm::LogInfo("IPSig1stAboveBReweighting") << "IPSig1stAboveB = " << FatJetInfo.Jet_trackSip2dSigAboveBottom_0[iJet] << ", wtIPSig1stAboveB = " << wtIPSig1stAboveB ;

      //added by Rizki - zRatio reweighting factor
      double wtZratio = 1.;
      double wtZratio_unw = 1.;
      if (doZratioReweighting_ && !isData && FatJetInfo.Jet_nbHadrons[iJet] > 1 ) { //added by rizki for Hbb tagger signal vs proxy studies. Want to only reweight jets of bgromgsp flavour.
        wtZratio *= GetWeightsZratioBased(file_ZratioWt_, hist_ZratioWt_, FatJetInfo.Jet_z_ratio[iJet] ) ;
        wtZratio_unw = wtFatJet;
        wtFatJet *= wtZratio ;
      }
      // edm::LogInfo("ZratioReweighting") << "Zratio = " << FatJetInfo.Jet_z_ratio[iJet] << ", wtZratio = " << wtZratio ;


      // -------For calculating b-fragmentation systematic
      if (abs(FatJetInfo.Jet_flavour[iJet]) == 5){
        if (doBFrag_){

          //float sfbFrag = 1.;
          float drMin = 0.8;   
          float jPT = FatJetInfo.Jet_pt[iJet];
          float jeta = FatJetInfo.Jet_eta[iJet];
          float jphi = FatJetInfo.Jet_phi[iJet];
          float WeightBFrag = 1.;
          float EnergyFraction = 0.; 
          int iB = -1, iptBin = 0, efbin = -1;
          if( jPT > 500 ) iptBin = 14;
          else if( jPT > 400 ) iptBin = 13;
          else if( jPT > 320 ) iptBin = 12;  
          else if( jPT > 260 ) iptBin = 11;
          else if( jPT > 210 ) iptBin = 10;
          else if( jPT > 160 ) iptBin =  9;
          else if( jPT > 120 ) iptBin =  8;  
          else if( jPT > 100 ) iptBin =  7;  
          else if( jPT >  80 ) iptBin =  6;  
          else if( jPT >  70 ) iptBin =  5;  
          else if( jPT >  60 ) iptBin =  4;  
          else if( jPT >  50 ) iptBin =  3;  
          else if( jPT >  40 ) iptBin =  2;  
          else if( jPT >  30 ) iptBin =  1;  
          else                 iptBin =  0;

          float B_Mass = 0.;
          for( int ib=0;ib<EvtInfo.nBHadrons;ib++ )
          {
            float drB = reco::deltaR(jeta,jphi,EvtInfo.BHadron_eta[ib],EvtInfo.BHadron_phi[ib]);
            if( drB < drMin )
            {
              if( EvtInfo.BHadron_mass[ib] > B_Mass ) 
              {
                B_Mass = EvtInfo.BHadron_mass[ib];
                iB = ib;
              } 
            }    
          }

          if( iB >= 0 ) 
          {
            EnergyFraction = EvtInfo.BHadron_pT[iB]/FatJetInfo.Jet_genpt[iJet];
            efbin = int( EnergyFraction / 0.02 );
            if( efbin >= 0 && efbin < 100 ) 
            {
              if( doBFragDown_ ) WeightBFrag = BTemplateCorrections[efbin][iptBin][0];
              if( doBFragUp_ ) WeightBFrag = BTemplateCorrections[efbin][iptBin][1];
            }    
          }

          wtFatJet *= WeightBFrag;

          if(DEBUG_ && doBFragUp_)std::cout << "SYSTEMATICS: Applying weights for B Frag Up systematics.. WeightBFrag = " << WeightBFrag  << std::endl; 
          if(DEBUG_ && doBFragDown_)std::cout << "SYSTEMATICS: Applying weights for B Frag Up systematics.. WeightBFrag = " << WeightBFrag << std::endl; 
        }
      }  

      //cd Frag Systematic
      if (abs(FatJetInfo.Jet_flavour[iJet]) == 5 || abs(FatJetInfo.Jet_flavour[iJet]) == 4){
        if (doCDFrag_)
        {

          float sfCD = 1.;
          float drMin = 0.8;   
          float jeta = FatJetInfo.Jet_eta[iJet];
          float jphi = FatJetInfo.Jet_phi[iJet];   
          bool isDplusMu = false, isDzeroMu = false, isDsubsMu = false;

          int ndaughters = 0;
          for( int k=0;k<EvtInfo.nDHadrons;k++ )
          {
            double dR = reco::deltaR(EvtInfo.DHadron_eta[k], 
                EvtInfo.DHadron_phi[k], 
                jeta,
                jphi);
            if( dR > drMin ) continue;
            bool isSemiMu = false;
            int nd = EvtInfo.DHadron_nDaughters[k];
            for( int kk=0;kk<nd;kk++ )
            {
              if( abs(EvtInfo.DHadron_DaughtersPdgID[kk+ndaughters]) == 13 ) isSemiMu = true;
            }

            ndaughters += nd;

            if( !isSemiMu ) continue;
            if( abs(EvtInfo.DHadron_pdgID[k]) == 411 ) isDplusMu = true;
            if( abs(EvtInfo.DHadron_pdgID[k]) == 421 ) isDzeroMu = true;
            if( abs(EvtInfo.DHadron_pdgID[k]) == 431 ) isDsubsMu = true;
          }

          if( isDplusMu ) sfCD *= 0.176 / 0.172;
          if( isDzeroMu ) sfCD *= 0.067 / 0.077;
          if( isDsubsMu ) sfCD *= 0.067 / 0.080;

          wtFatJet *= sfCD;

          if(DEBUG_)std::cout << "SYSTEMATICS: Applying weights for cd Frag systematics.. sfCD = " << sfCD <<std::endl; 

        }
      }

      //c fragmentation
      if (abs(FatJetInfo.Jet_flavour[iJet]) == 4){
        if (doCFrag_){

          float sfC = 1.;
          float drMin = 0.8;   
          float jeta = FatJetInfo.Jet_eta[iJet];
          float jphi = FatJetInfo.Jet_phi[iJet];

          bool hasCquark = 0;
          for( int c=0;c<EvtInfo.ncQuarks;c++ )
          {
            double dRc = reco::deltaR(EvtInfo.cQuark_eta[c], 
                EvtInfo.cQuark_phi[c], 
                jeta,
                jphi);
            if( dRc < drMin ) hasCquark = 1;
          }

          if( hasCquark )
          { 
            bool isDplus = false, isDzero = false, isDsubs = false;// isDbary = false;
            for( int k=0;k<EvtInfo.nDHadrons;k++ )
            {
              double dR = reco::deltaR(EvtInfo.DHadron_eta[k], 
                  EvtInfo.DHadron_phi[k], 
                  jeta, 
                  jphi);
              if( dR > drMin ) continue;

              if( abs(EvtInfo.DHadron_pdgID[k]) == 411 ) isDplus = true;
              if( abs(EvtInfo.DHadron_pdgID[k]) == 421 ) isDzero = true;
              if( abs(EvtInfo.DHadron_pdgID[k]) == 431 ) isDsubs = true;
              //if((abs(EvtInfo.DHadron_pdgID[k])/1000)%10 == 4 ) isDbary = true;  //ATTENTION! what is this? ASK ALICE/Kirill!
            }       

            if( isDplus ) sfC *= 1.37; // PDG2008(0.246+-0.020)
            if( isDzero ) sfC *= 0.91; // PDG2008(0.565+-0.032)
            if( isDsubs ) sfC *= 0.67; // PDG2008(0.080+-0.017)
            // 0.185072, 0.58923, 0.115961
          }
          wtFatJet *= sfC;

          if(DEBUG_)std::cout << "SYSTEMATICS: Applying weights for C Frag systematics.. sfC = " << sfC <<std::endl; 
        }    
      }

      // K0s Lambda sys
      if (abs(FatJetInfo.Jet_flavour[iJet]) < 4 || abs(FatJetInfo.Jet_flavour[iJet])==21)
      {
        if (doK0L_)
        {

          float sfK0L = 1.;
          float jeta = FatJetInfo.Jet_eta[iJet];
          float jphi = FatJetInfo.Jet_phi[iJet];
          int nK0s = 0, nLambda = 0;
          for( int k=0;k<EvtInfo.nGenV0;k++ )
          {
            double dR = reco::deltaR(EvtInfo.GenV0_eta[k], 
                EvtInfo.GenV0_phi[k], 
                jeta,
                jphi);
            if( dR > 0.3 ) continue;
            int pdgid = abs(EvtInfo.GenV0_pdgID[k]);
            if( pdgid == 310 )  nK0s++;
            if( pdgid == 3122 ) nLambda++;
          }
          if( nK0s > 0 )    sfK0L *= 1.3;
          if( nLambda > 0 ) sfK0L *= 1.5;
          wtFatJet *= sfK0L;

          if(DEBUG_)std::cout << "SYSTEMATICS: Applying weights for K0L systematics.. sfK0L = " << sfK0L << std::endl; 
        }
      }


      //// fat jet multiplicity
      ++nFatJet;

      //// test if the jet is a gluon splitting b jet
      bool isGSPbb(false);
      if ( FatJetInfo.Jet_nbHadrons[iJet] > 1 ) isGSPbb = true ;

      //// test if the jet is a gluon splitting c jet
      bool isGSPcc(false);
      if ( FatJetInfo.Jet_ncHadrons[iJet] > 1 ) isGSPcc = true ;

      //// ----------------------------------------------
      //// Fill (fat) jet histograms
      //// ----------------------------------------------

        FillHisto2D("FatJet_pt_softdropMass" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,FatJetInfo.Jet_pt[iJet], FatJetInfo.Jet_massSoftDrop[iJet] ,wtPU*wtFatJet);
        FillHisto("FatJet_Mass" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,FatJetInfo.Jet_mass[iJet] ,wtPU*wtFatJet);
        FillHisto("FatJet_prunedMass" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,FatJetInfo.Jet_massPruned[iJet] ,wtPU*wtFatJet);
        FillHisto2D("FatJet_prunedMass_nsubjettiness" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,FatJetInfo.Jet_massPruned[iJet] ,FatJetInfo.Jet_tau2[iJet]/FatJetInfo.Jet_tau1[iJet] ,wtPU*wtFatJet);
        FillHisto2D("FatJet_pt_prunedMass" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,FatJetInfo.Jet_pt[iJet], FatJetInfo.Jet_massPruned[iJet] ,wtPU*wtFatJet);
        FillHisto("FatJet_softdropMass" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,FatJetInfo.Jet_massSoftDrop[iJet] ,wtPU*wtFatJet);
        FillHisto("FatJet_softdropMass_unw" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,FatJetInfo.Jet_massSoftDrop[iJet] ,wtPU*wtMassSoftDrop_unw);
        FillHisto2D("FatJet_softdropMass_nsubjettiness" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,FatJetInfo.Jet_massSoftDrop[iJet] ,FatJetInfo.Jet_tau2[iJet]/FatJetInfo.Jet_tau1[iJet] ,wtPU*wtFatJet);

        if (usePrunedSubjets_) {
          FillHisto("FatJet_massDrop_pruned" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,std::max( SubJets.Jet_mass[iSubJet1]/SubJets.Jet_jes[iSubJet1], SubJets.Jet_mass[iSubJet2]/SubJets.Jet_jes[iSubJet2] )/(FatJetInfo.Jet_massPruned[iJet]/FatJetInfo.Jet_jes[iJet]) ,wtPU*wtFatJet);
          FillHisto("FatJet_prunedsubjet_dR" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,subjet_dR ,wtPU*wtFatJet);
          FillHisto("FatJet_prunedsubjet_dyphi" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,subjet_dyphi ,wtPU*wtFatJet);
        }
        else if (useSoftDropSubjets_) {
          FillHisto("FatJet_massDrop_softdrop" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,std::max( SubJets.Jet_mass[iSubJet1]/SubJets.Jet_jes[iSubJet1], SubJets.Jet_mass[iSubJet2]/SubJets.Jet_jes[iSubJet2] )/(FatJetInfo.Jet_massSoftDrop[iJet]/FatJetInfo.Jet_jes[iJet]) ,wtPU*wtFatJet);
          FillHisto("FatJet_softdropsubjet_dR" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,subjet_dR ,wtPU*wtFatJet);
          FillHisto("FatJet_softdropsubjet_dyphi" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,subjet_dyphi ,wtPU*wtFatJet);
        }

        h1_fatjet_pt->Fill(FatJetInfo.Jet_pt[iJet],wtPU*wtFatJet);
        
        if(firstJet){
            h1_leading_fatjet_pt->Fill(FatJetInfo.Jet_pt[iJet],wtPU*wtFatJet);
            if(DEBUG_)std::cout<<"leading jet pT = " <<  FatJetInfo.Jet_pt[iJet] << std::endl;
            firstJet=false;
        }

        FillHisto("FatJet_nsubjettiness" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,FatJetInfo.Jet_tau2[iJet]/FatJetInfo.Jet_tau1[iJet] ,wtPU*wtFatJet);

        FillHisto("FatJet_pt_all_unw" ,FatJetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,FatJetInfo.Jet_pt[iJet] ,wtPU*wtFatJet/wtJetPt); //debug for signal vs proxy reweighting - rizki

        //fill histo for general plots: 
        fillJetHistos(FatJetInfo, iJet, isGSPbb, isGSPcc ,"FatJet", nmu, nselmuon, idxFirstMuon, wtPU*wtFatJet);

        //fill histo for DoubleB Commissioning Plots: 
        if(produceDoubleBCommissioning_) fillJetHistos_DoubleB(FatJetInfo, iJet, isGSPbb, isGSPcc ,"FatJet", nmu, nselmuon, idxFirstMuon, muon1_v, muon2_v, dimuon_v, idxMuon_inFirstSubjet, idxMuon_inSecondSubjet, subjet1_p4, subjet2_p4, wtPU*wtFatJet);

        //fill histo for DeepDoubleX Commissioning Plots: 
        if(produceDeepDoubleXCommissioning_) fillJetHistos_DeepDoubleX(FatJetInfo, iJet, isGSPbb, isGSPcc ,"FatJet", wtPU*wtFatJet);

        //fill histo for DoubleB SF calculation / templates: 
        if(produceDDXSFtemplates_) {
            TString taggerTString = chooseDDXtagger_; // should implement this typefix properly
            if (DEBUG_) std::cout << "nSV : " << FatJetInfo.Jet_nSV_fat[iJet] << std::endl;
            fillJetHistos_SF(FatJetInfo, iJet, isGSPbb, isGSPcc ,"FatJet", "JP", false, taggerTString , WPmap, ptbinmap, wtPU*wtFatJet);
            if (FatJetInfo.Jet_nSV_fat[iJet]>0)  fillJetHistos_SF(FatJetInfo, iJet, isGSPbb, isGSPcc ,"FatJet", "tau1VertexMassCorr",   true,  taggerTString,  WPmap, ptbinmap, wtPU*wtFatJet);
            if (FatJetInfo.Jet_nSV_fat[iJet]>0)  fillJetHistos_SF(FatJetInfo, iJet, isGSPbb, isGSPcc ,"FatJet", "JPhasSV",              false,  taggerTString, WPmap, ptbinmap, wtPU*wtFatJet);
            if (FatJetInfo.Jet_nSV_fat[iJet]==0) fillJetHistos_SF(FatJetInfo, iJet, isGSPbb, isGSPcc ,"FatJet", "JPnoSV",               false,  taggerTString, WPmap, ptbinmap, wtPU*wtFatJet);
        }     

        /* Uncomment below as needed.
        //// ------- start process subjets --------------
        if( usePrunedSubjets_ || useSoftDropSubjets_ ) {
          int nTotalFat = 0, nSharedFat = 0; // for track sharing

          for(int sj=0; sj<2; ++sj) {
            int iSubJet = SubJetInfo.Jet_nFirstSJ[iJet];
            if( sj==1 ) iSubJet = SubJetInfo.Jet_nLastSJ[iJet];

            int idxFirstMuonSubJet = -1;
            int nselmuonSubJet = 0;
            int nmuSubJet = 0;

            if (SubJets.nPFMuon>0) {
              for (int iMu=0; iMu<SubJets.nPFMuon; ++iMu) {
                if (SubJets.PFMuon_IdxJet[iMu]==iSubJet ) {
                  ++nmuSubJet;
                  if (passMuonSelection(iMu, SubJets, iSubJet, (dynamicMuonSubJetDR_ ? subjet_dR/2 : 0.4 ))) {
                    if(nselmuonSubJet == 0) idxFirstMuonSubJet = iMu;
                    ++nselmuonSubJet;
                  }
                }
              }
            }

            if(applySubJetMuonTagging_ && nselmuonSubJet==0) continue;  //// if enabled, select muon-tagged subjets
            if(applySubJetBTagging_ && 
                (SubJets.Jet_CombIVF[iSubJet]<=subJetBDiscrMin_ || SubJets.Jet_CombIVF[iSubJet]>subJetBDiscrMax_) 
              ) continue;  //// if enabled, select b-tagged subjets

            //// apply b-tagging scale factors
            double wtSubJet = 1.;
            if( applySFs_ && !isData ) {
              if( applyFatJetBTagging_ && fatJetDoubleBTagging_ ) wtSubJet *= wtFatJet;
              else wtSubJet *= scaleFactor(SubJets.Jet_flavour[iSubJet], SubJets.Jet_pt[iSubJet], SubJets.Jet_eta[iSubJet], (subJetBDiscrMin_>0.25));
            }

            double wtSubJetPt = 1.;
            if (doSubJetPtReweighting_ && !isData) {
              wtSubJetPt *= GetWeightsJetPtBased(file_SubJetPtWt_, hist_SubJetPtWt_, SubJets.Jet_pt[iSubJet]) ;
              wtSubJet *= wtSubJetPt ;
            }

            //// subjet multiplicity
            ++nSubJet;

            //// fill subjet histograms
            h1_subjet_pt->Fill(SubJets.Jet_pt[iSubJet],wtPU*wtSubJet);

            std::string sjlabel ;
            if (usePrunedSubjets_) sjlabel = "PrunedSubJet" ;
            else if (useSoftDropSubjets_) sjlabel = "SoftDropSubJet" ;
            //fillJetHistos(SubJets, iSubJet, false, false ,sjlabel, nmuSubJet, nselmuonSubJet, idxFirstMuonSubJet, wtPU*wtSubJet);

            //// track sharing
            int iSubJetComp = (sj==0 ? SubJetInfo.Jet_nLastSJ[iJet] : SubJetInfo.Jet_nLastSJ[iJet]); // companion subjet index
            int nTotal = 0, nShared = 0;

            for (int iTrk=SubJets.Jet_nFirstTrack[iSubJet]; iTrk<SubJets.Jet_nLastTrack[iSubJet]; ++iTrk) {
              if( reco::deltaR( SubJets.Track_eta[iTrk], SubJets.Track_phi[iTrk], SubJets.Jet_eta[iSubJet], SubJets.Jet_phi[iSubJet] ) < 0.3 ) {
                ++nTotal;
                ++nTotalFat;
                if( reco::deltaR( SubJets.Track_eta[iTrk], SubJets.Track_phi[iTrk], SubJets.Jet_eta[iSubJetComp], SubJets.Jet_phi[iSubJetComp] ) < 0.3 ) {
                  ++nShared;
                  if(sj==0) ++nSharedFat;
                }
              }
            }

            p1_SubJetPt_TotalTracks->Fill(SubJets.Jet_pt[iSubJet], nTotal, wtPU*wtSubJet);
            p1_SubJetPt_SharedTracks->Fill(SubJets.Jet_pt[iSubJet], nShared, wtPU*wtSubJet);
            if( nTotal>0 ) p1_SubJetPt_SharedTracksRatio->Fill(SubJets.Jet_pt[iSubJet], double(nShared)/double(nTotal), wtPU*wtSubJet);
          }

          p1_FatJetPt_TotalTracks->Fill(FatJetInfo.Jet_pt[iJet], nTotalFat-nSharedFat, wtPU*wtFatJet);
          p1_FatJetPt_SharedTracks->Fill(FatJetInfo.Jet_pt[iJet], nSharedFat, wtPU*wtFatJet);
          if( nTotalFat>0 ) p1_FatJetPt_SharedTracksRatio->Fill(FatJetInfo.Jet_pt[iJet], double(nSharedFat)/double(nTotalFat-nSharedFat), wtPU*wtFatJet);

        } //// ------- end process subjets --------------
        */

    }    

    //----------------------------- End fat jet loop ----------------------------------------//

    // fill jet multiplicity
    if(DEBUG_)std::cout << "==========FatJetStats========== " << std::endl;
    if(DEBUG_ && !isData)std::cout << " removeProblemJet     = " << total_removedProblemJet << std::endl;
    if(DEBUG_)std::cout << "    Passed all selection = " << nFatJet << std::endl;
    if(DEBUG_)std::cout << "    Total in event       = " <<     FatJetInfo.nJet << std::endl;
    if(DEBUG_)std::cout << "=============================== " << std::endl;
    if(DEBUG_)std::cout << " " << std::endl;
    h1_nFatJet->Fill(nFatJet, wtPU);
    if( usePrunedSubjets_ || useSoftDropSubjets_ ) h1_nSubJet->Fill(nSubJet, wtPU);

    if( !isData && nFatJet>0 ) h1_pt_hat_sel->Fill(EvtInfo.pthat,wtPU);

    }
    //----------------------------- End event loop ----------------------------------------//
  }

  // ------------------------------------------------------------------------------
  void BTagValidation::fillJetHistos(const JetInfoBranches& JetInfo, const int iJet, const bool isGSPbb, const bool isGSPcc, const TString& histoTag, const int nmu, const int nselmuon, const int idxFirstMuon, const double wt) {

    float ptjet      = JetInfo.Jet_pt[iJet];
    float etajet     = JetInfo.Jet_eta[iJet];
    float phijet     = JetInfo.Jet_phi[iJet];
    float mass       = JetInfo.Jet_mass[iJet];
    float ntrkjet    = JetInfo.Jet_ntracks[iJet];
    int   flav       = JetInfo.Jet_flavour[iJet];

    FillHisto(histoTag+"_pt_all", flav, isGSPbb , isGSPcc, ptjet , wt) ;
    if (JetInfo.Jet_SV_multi[iJet] > 0) FillHisto(histoTag+"_pt_sv", flav, isGSPbb , isGSPcc ,ptjet , wt) ;

    FillHisto(histoTag+"_eta"         ,flav ,isGSPbb , isGSPcc ,etajet     ,wt) ;
    FillHisto(histoTag+"_phi"         ,flav ,isGSPbb , isGSPcc ,phijet     ,wt) ;
    FillHisto(histoTag+"_mass"        ,flav ,isGSPbb , isGSPcc ,mass       ,wt) ;
    FillHisto(histoTag+"_track_multi" ,flav ,isGSPbb , isGSPcc ,ntrkjet    ,wt) ;

    float mass_sv        = 0.;
    int   n_sv           = 0 ;
    float chi2norm_sv    = 0.;
    float flightSig_sv   = 0.;
    float flight2DSig_sv = 0.;
    float sv_dR_jet      = 0.;
    float sv_dR_dir_sum  = 0.;
    float sv_dR_jet_sum  = 0.;
    float sv_en_rat      = 0.;
    //DMfloat sv_aboveC      = 0.;
    float sv_pt          = 0.;
    float sveta          = 0.;
    float svphi          = 0.;
    float sv_flight3D    = 0.;
    float sv_flight3Derr = 0.;
    float sv_flight2D    = 0.;
    float sv_flight2Derr = 0.;
    int   sv_totchar     = 0;
    float sv_nTrk        = 0.;
    //DMfloat sv_1st_nTrk    = 0.;

    int   n1_ip     = -1;
    int   n2_ip     = -1;
    int   n3_ip     = -1;
    float sig1_ip   = -9999;
    float sig2_ip   = -9999;
    float sig3_ip   = -9999;
    float sig12D_ip = -9999;
    float sig22D_ip = -9999;
    float sig32D_ip = -9999;

    int ntracksel   = 0;

    if ( useJetProbaTree_ ) {
      //---------------------------- Start tracks loop ---------------------------------------//
      for (int iTrk=JetInfo.Jet_nFirstTrack[iJet]; iTrk<JetInfo.Jet_nLastTrack[iJet] ; ++iTrk) {
        // -----------------------------------------
        // -------- track information --------------
        // -----------------------------------------
        bool passNhit=false;
        bool passPix= false;
        bool passIPz=false;
        bool passPt=false;
        bool passnormchi2=false;
        bool passtrkdist=false;
        bool passtrklen=false;
        bool passTrackIP2D=false;

        if (JetInfo.Track_nHitAll[iTrk]>=8)      passNhit=true;
        if (JetInfo.Track_nHitPixel[iTrk]>=1)    passPix= true;
        if (fabs(JetInfo.Track_dz[iTrk])<17)     passIPz=true;
        if (JetInfo.Track_pt[iTrk]>1)            passPt=true;
        if (JetInfo.Track_chi2[iTrk]<5)          passnormchi2=true;
        if (fabs(JetInfo.Track_dist[iTrk])<0.07) passtrkdist=true;
        if (JetInfo.Track_length[iTrk]<5)        passtrklen=true;
        if (fabs(JetInfo.Track_IP2D[iTrk])<0.2)  passTrackIP2D=true;

        if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passTrackIP2D)
          FillHisto(histoTag+"_track_len_cut",          flav, isGSPbb ,isGSPcc ,JetInfo.Track_length[iTrk] , wt);

        if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrklen && passTrackIP2D)
          FillHisto(histoTag+"_track_dist_cut",         flav, isGSPbb ,isGSPcc ,fabs(JetInfo.Track_dist[iTrk])   , wt);

        if (passNhit && passPix && passIPz && passPt && passtrkdist && passtrklen && passTrackIP2D)
          FillHisto(histoTag+"_track_chi2_cut",         flav, isGSPbb ,isGSPcc ,JetInfo.Track_chi2[iTrk]         , wt);

        if (passNhit && passPix && passIPz && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
        {
          FillHisto(histoTag+"_track_pt_cut",           flav, isGSPbb ,isGSPcc ,JetInfo.Track_pt[iTrk]     , wt);
          FillHisto(histoTag+"_track_pt15_cut",         flav, isGSPbb ,isGSPcc ,JetInfo.Track_pt[iTrk]     , wt);
        }

        if (passNhit && passPix && passPt && passnormchi2 && passtrkdist && passtrklen)
          FillHisto(histoTag+"_track_dz_cut",          flav, isGSPbb ,isGSPcc ,JetInfo.Track_dz[iTrk]      ,wt);

        if (passNhit && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
          FillHisto(histoTag+"_track_HPix_cut",         flav, isGSPbb ,isGSPcc ,JetInfo.Track_nHitPixel[iTrk],wt);

        if (passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
          FillHisto(histoTag+"_track_nHit_cut",       flav, isGSPbb ,isGSPcc ,JetInfo.Track_nHitAll[iTrk],wt);

        if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen )
          FillHisto(histoTag+"_track_IP2D_cut",         flav, isGSPbb ,isGSPcc ,JetInfo.Track_IP2D[iTrk],wt);

        // -------------------- start selected tracks -------------------------
        if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D) {
          ++ntracksel;

          FillHisto(histoTag+"_track_chi2",    flav, isGSPbb , isGSPcc ,JetInfo.Track_chi2[iTrk]      ,wt);
          FillHisto(histoTag+"_track_nHit",    flav, isGSPbb , isGSPcc ,JetInfo.Track_nHitAll[iTrk]   ,wt);
          FillHisto(histoTag+"_track_HPix",    flav, isGSPbb , isGSPcc ,JetInfo.Track_nHitPixel[iTrk] ,wt);
          FillHisto(histoTag+"_track_IPs",     flav, isGSPbb , isGSPcc ,JetInfo.Track_IPsig[iTrk]     ,wt);
          FillHisto(histoTag+"_track_IP",      flav, isGSPbb , isGSPcc ,JetInfo.Track_IP[iTrk]        ,wt);
          FillHisto(histoTag+"_track_IP2Ds",   flav, isGSPbb , isGSPcc ,JetInfo.Track_IP2Dsig[iTrk]   ,wt);
          FillHisto(histoTag+"_track_IP2D",    flav, isGSPbb , isGSPcc ,JetInfo.Track_IP2D[iTrk]      ,wt);
          FillHisto(histoTag+"_track_IP2Derr", flav, isGSPbb , isGSPcc ,JetInfo.Track_IP2Derr[iTrk]   ,wt);
          FillHisto(histoTag+"_track_IPerr",   flav, isGSPbb , isGSPcc ,JetInfo.Track_IPerr[iTrk]     ,wt);
          FillHisto(histoTag+"_track_dz",      flav, isGSPbb , isGSPcc ,JetInfo.Track_dz[iTrk]        ,wt);
          FillHisto(histoTag+"_track_isfromSV",flav, isGSPbb , isGSPcc ,JetInfo.Track_isfromSV[iTrk]  ,wt);
          FillHisto(histoTag+"_track_len",     flav, isGSPbb , isGSPcc ,JetInfo.Track_length[iTrk]    ,wt);
          FillHisto(histoTag+"_track_dist",    flav, isGSPbb , isGSPcc ,fabs(JetInfo.Track_dist[iTrk]),wt);
          FillHisto(histoTag+"_track_pt",      flav, isGSPbb , isGSPcc ,JetInfo.Track_pt[iTrk]        ,wt);
          FillHisto(histoTag+"_track_pt15",    flav, isGSPbb , isGSPcc ,JetInfo.Track_pt[iTrk]        ,wt);

          // tracks sorted by IP
          float sig   = JetInfo.Track_IP[iTrk]/JetInfo.Track_IPerr[iTrk];
          float sig2D = JetInfo.Track_IP2D[iTrk]/JetInfo.Track_IP2Derr[iTrk];
          if (sig>sig1_ip)
          {
            sig3_ip   = sig2_ip;
            sig2_ip   = sig1_ip;
            sig1_ip   = sig;
            sig32D_ip = sig22D_ip;
            sig22D_ip = sig12D_ip;
            sig12D_ip = sig2D;
            n3_ip     = n2_ip;
            n2_ip     = n1_ip;
            n1_ip     = iTrk;
          }
          else if (sig>sig2_ip)
          {
            sig3_ip   = sig2_ip;
            sig2_ip   = sig;
            sig32D_ip = sig22D_ip;
            sig22D_ip = sig2D;
            n3_ip     = n2_ip;
            n2_ip     = iTrk;
          }
          else if (sig>sig3_ip)
          {
            sig3_ip   = sig;
            sig32D_ip = sig2D;
            n3_ip     = iTrk;
          }
        }
        // -------------------- end selected tracks -------------------------
      }
      //---------------------------- End tracks loop ---------------------------------------//

      if (n1_ip>-1)  {
        FillHisto(histoTag+"_track_IPs1tr",    flav, isGSPbb , isGSPcc ,sig1_ip                          , wt) ;
        FillHisto(histoTag+"_track_IP1tr",     flav, isGSPbb , isGSPcc ,JetInfo.Track_IP[n1_ip]       , wt) ;
        FillHisto(histoTag+"_track_IPerr1tr",  flav, isGSPbb , isGSPcc ,JetInfo.Track_IPerr[n1_ip]    , wt) ;
        FillHisto(histoTag+"_track_IP2Ds1tr",  flav, isGSPbb , isGSPcc ,sig12D_ip                        , wt) ;
        FillHisto(histoTag+"_track_IP2D1tr",   flav, isGSPbb , isGSPcc ,JetInfo.Track_IP2D[n1_ip]     , wt) ;
        FillHisto(histoTag+"_track_IP2Derr1tr",flav, isGSPbb , isGSPcc ,JetInfo.Track_IP2Derr[n1_ip]  , wt) ;
      }

      if (n2_ip>-1) {
        FillHisto(histoTag+"_track_IPs2tr",    flav, isGSPbb , isGSPcc ,sig2_ip                          , wt) ;
        FillHisto(histoTag+"_track_IP2tr",     flav, isGSPbb , isGSPcc ,JetInfo.Track_IP[n2_ip]       , wt) ;
        FillHisto(histoTag+"_track_IPerr2tr",  flav, isGSPbb , isGSPcc ,JetInfo.Track_IPerr[n2_ip]    , wt) ;
        FillHisto(histoTag+"_track_IP2Ds2tr",  flav, isGSPbb , isGSPcc ,sig22D_ip                        , wt) ;
        FillHisto(histoTag+"_track_IP2D2tr",   flav, isGSPbb , isGSPcc ,JetInfo.Track_IP2D[n2_ip]     , wt) ;
        FillHisto(histoTag+"_track_IP2Derr2tr",flav, isGSPbb , isGSPcc ,JetInfo.Track_IP2Derr[n2_ip]  , wt) ;
      }

      if (n3_ip>-1) {
        FillHisto(histoTag+"_track_IPs3tr",    flav, isGSPbb , isGSPcc ,sig3_ip                          , wt) ;
        FillHisto(histoTag+"_track_IP3tr",     flav, isGSPbb , isGSPcc ,JetInfo.Track_IP[n3_ip]       , wt) ;
        FillHisto(histoTag+"_track_IPerr3tr",  flav, isGSPbb , isGSPcc ,JetInfo.Track_IPerr[n3_ip]    , wt) ;
        FillHisto(histoTag+"_track_IP2Ds3tr",  flav, isGSPbb , isGSPcc ,sig32D_ip                        , wt) ;
        FillHisto(histoTag+"_track_IP2D3tr",   flav, isGSPbb , isGSPcc ,JetInfo.Track_IP2D[n3_ip]     , wt) ;
        FillHisto(histoTag+"_track_IP2Derr3tr",flav, isGSPbb , isGSPcc ,JetInfo.Track_IP2Derr[n3_ip]  , wt) ;
      }

      FillHisto(histoTag+"_trk_multi_sel",     flav, isGSPbb , isGSPcc ,ntracksel           , wt);
      FillHisto2D(histoTag+"_seltrack_vs_jetpt", flav, isGSPbb ,isGSPcc ,ptjet ,  ntracksel , wt);
      FillHisto2D(histoTag+"_muon_ptrel_vs_nseltrack", flav, isGSPbb ,isGSPcc ,JetInfo.PFMuon_ptrel[idxFirstMuon],  ntracksel , wt);

    } // end useJetProbaTree

    // ------------------------------------------------
    // -------------- SV information ------------------
    // ------------------------------------------------
    n_sv = JetInfo.Jet_SV_multi[iJet];
    FillHisto(histoTag+"_sv_multi_0",      flav, isGSPbb , isGSPcc ,n_sv   ,         wt);

    if (n_sv>0)
    {

      chi2norm_sv    = JetInfo.SV_chi2[JetInfo.Jet_nFirstSV[iJet]]/JetInfo.SV_ndf[JetInfo.Jet_nFirstSV[iJet]];
      flightSig_sv   = JetInfo.SV_flight[JetInfo.Jet_nFirstSV[iJet]]/JetInfo.SV_flightErr[JetInfo.Jet_nFirstSV[iJet]];
      flight2DSig_sv = JetInfo.SV_flight2D[JetInfo.Jet_nFirstSV[iJet]]/JetInfo.SV_flight2DErr[JetInfo.Jet_nFirstSV[iJet]];
      mass_sv        = JetInfo.SV_mass[JetInfo.Jet_nFirstSV[iJet]];
      sv_dR_jet      = JetInfo.SV_deltaR_jet[JetInfo.Jet_nFirstSV[iJet]];
      sv_dR_dir_sum  = JetInfo.SV_deltaR_sum_dir[JetInfo.Jet_nFirstSV[iJet]];
      sv_dR_jet_sum  = JetInfo.SV_deltaR_sum_jet[JetInfo.Jet_nFirstSV[iJet]];
      sv_en_rat      = JetInfo.SV_EnergyRatio[JetInfo.Jet_nFirstSV[iJet]];
      //DMsv_aboveC       = JetInfo.SV_aboveC[JetInfo.Jet_nFirstSV[iJet]];
      sv_pt          = JetInfo.SV_vtx_pt[JetInfo.Jet_nFirstSV[iJet]];
      sveta          = JetInfo.SV_vtx_eta[JetInfo.Jet_nFirstSV[iJet]];
      svphi          = JetInfo.SV_vtx_phi[JetInfo.Jet_nFirstSV[iJet]];

      sv_flight3D     = JetInfo.SV_flight[JetInfo.Jet_nFirstSV[iJet]] ;
      sv_flight3Derr  = JetInfo.SV_flightErr[JetInfo.Jet_nFirstSV[iJet]];
      sv_flight2D     = JetInfo.SV_flight2D[JetInfo.Jet_nFirstSV[iJet]];
      sv_flight2Derr  = JetInfo.SV_flight2DErr[JetInfo.Jet_nFirstSV[iJet]];
      sv_totchar      = JetInfo.SV_totCharge[JetInfo.Jet_nFirstSV[iJet]] ;

      sv_nTrk         = JetInfo.SV_nTrk[JetInfo.Jet_nFirstSV[iJet]] ;
      //DMsv_1st_nTrk     = JetInfo.SV_nTrk_firstVxt[JetInfo.Jet_nFirstSV[iJet]];

      // --------- SV histograms --------
      FillHisto(histoTag+"_sv_multi",        flav, isGSPbb ,isGSPcc ,n_sv ,  wt);
      FillHisto(histoTag+"_sv_chi2norm",     flav, isGSPbb ,isGSPcc ,chi2norm_sv        , wt);
      FillHisto(histoTag+"_sv_mass",         flav, isGSPbb ,isGSPcc ,mass_sv,             wt);
      FillHisto(histoTag+"_sv_deltaR_jet",   flav, isGSPbb ,isGSPcc ,sv_dR_jet,           wt);
      FillHisto(histoTag+"_sv_deltaR_sumJet",flav, isGSPbb ,isGSPcc ,sv_dR_dir_sum,       wt);
      FillHisto(histoTag+"_sv_deltaR_sumDir",flav, isGSPbb ,isGSPcc ,sv_dR_jet_sum,       wt);
      FillHisto(histoTag+"_sv_en_ratio",     flav, isGSPbb ,isGSPcc ,sv_en_rat,           wt);
      //DMFillHisto(histoTag+"_sv_aboveC",       flav, isGSPbb ,isGSPcc ,sv_aboveC,            wt);
      FillHisto(histoTag+"_sv_pt",             flav, isGSPbb ,isGSPcc ,sv_pt,               wt);
      FillHisto(histoTag+"_sv_flight2D",       flav, isGSPbb ,isGSPcc ,sv_flight2D,         wt);
      FillHisto(histoTag+"_sv_flight2Derr",    flav, isGSPbb ,isGSPcc ,sv_flight2Derr,      wt);
      FillHisto(histoTag+"_sv_flightSig2D",    flav, isGSPbb ,isGSPcc ,flight2DSig_sv,      wt);
      FillHisto(histoTag+"_sv_tot_charge",     flav, isGSPbb ,isGSPcc ,sv_totchar,          wt);
      FillHisto(histoTag+"_svnTrk",            flav, isGSPbb ,isGSPcc ,sv_nTrk,             wt);
      //DMFillHisto(histoTag+"_svnTrk_firstVxt", flav, isGSPbb ,isGSPcc ,sv_1st_nTrk,         wt);
      FillHisto(histoTag+"_sv_eta",            flav, isGSPbb ,isGSPcc ,sveta,               wt);
      FillHisto(histoTag+"_sv_phi",            flav, isGSPbb ,isGSPcc ,svphi,               wt);
      FillHisto(histoTag+"_sv_flight3D",       flav, isGSPbb ,isGSPcc ,sv_flight3D,         wt);
      FillHisto(histoTag+"_sv_flight3Derr",    flav, isGSPbb ,isGSPcc ,sv_flight3Derr,      wt);
      FillHisto(histoTag+"_sv_flight3DSig",    flav, isGSPbb ,isGSPcc ,flightSig_sv,        wt);

      if (sv_nTrk >2)
      {
        FillHisto(histoTag+"_sv_mass_3trk", flav, isGSPbb ,isGSPcc ,mass_sv,wt);
        FillHisto(histoTag+"_sv_flightSig2D_3trk",  flav, isGSPbb ,isGSPcc ,flight2DSig_sv,        wt);
      }

      FillHisto2D(histoTag+"_sv_mass_vs_flightDist3D"     ,flav,isGSPbb ,isGSPcc ,sv_flight3D,mass_sv,wt);
      FillHisto2D(histoTag+"_avg_sv_mass_vs_jetpt"        ,flav,isGSPbb ,isGSPcc ,ptjet,mass_sv,wt);
      FillHisto2D(histoTag+"_sv_deltar_jet_vs_jetpt"      ,flav,isGSPbb ,isGSPcc ,ptjet,sv_dR_jet,wt);
      FillHisto2D(histoTag+"_sv_deltar_sum_jet_vs_jetpt"  ,flav,isGSPbb ,isGSPcc ,ptjet,sv_dR_dir_sum,wt);
      FillHisto2D(histoTag+"_sv_deltar_sum_dir_vs_jetpt"  ,flav,isGSPbb ,isGSPcc ,ptjet,sv_dR_jet_sum,wt);
    } // if n_sv > 0

    // ------------------------------------------------
    // ------------- tagger information ---------------
    // ------------------------------------------------
    float tche              = JetInfo.Jet_Ip2P[iJet];
    float tchp              = JetInfo.Jet_Ip3P[iJet];
    float jetproba          = JetInfo.Jet_Proba[iJet];
    float jetbproba         = JetInfo.Jet_Bprob[iJet];
    float ssvhe             = JetInfo.Jet_Svx[iJet] ;
    float ssvhp             = JetInfo.Jet_SvxHP[iJet];
    float csvivfv2          = JetInfo.Jet_CombIVF[iJet];
    float cmvav2            = JetInfo.Jet_cMVAv2[iJet];
    float doubleb           = JetInfo.Jet_DoubleSV[iJet];
    float mass_TagVarCSV_sv = JetInfo.TagVarCSV_vertexMass[iJet];
    float deepDoubleBvLQCD  = JetInfo.Jet_DeepDoubleBvLQCD[iJet];
    float deepDoubleBvLHbb  = JetInfo.Jet_DeepDoubleBvLHbb[iJet];
    float deepDoubleCvLQCD  = JetInfo.Jet_DeepDoubleCvLQCD[iJet];
    float deepDoubleCvLHcc  = JetInfo.Jet_DeepDoubleCvLHcc[iJet];
    float deepDoubleCvBHcc  = JetInfo.Jet_DeepDoubleCvBHcc[iJet];
    float deepDoubleCvBHbb  = JetInfo.Jet_DeepDoubleCvBHbb[iJet];
    float massIndDeepDoubleBvLQCD  = JetInfo.Jet_MassIndDeepDoubleBvLQCD[iJet];
    float massIndDeepDoubleBvLHbb  = JetInfo.Jet_MassIndDeepDoubleBvLHbb[iJet];
    float massIndDeepDoubleCvLQCD  = JetInfo.Jet_MassIndDeepDoubleCvLQCD[iJet];
    float massIndDeepDoubleCvLHcc  = JetInfo.Jet_MassIndDeepDoubleCvLHcc[iJet];
    float massIndDeepDoubleCvBHcc  = JetInfo.Jet_MassIndDeepDoubleCvBHcc[iJet];
    float massIndDeepDoubleCvBHbb  = JetInfo.Jet_MassIndDeepDoubleCvBHbb[iJet];
    float deepBoostedJetbbvsLight = JetInfo.Jet_DeepBoostedJetbbvsLight[iJet];
    float deepBoostedJetccvsLight = JetInfo.Jet_DeepBoostedJetccvsLight[iJet];
    float deepBoostedJetTvsQCD = JetInfo.Jet_DeepBoostedJetTvsQCD[iJet];
    float deepBoostedJetZHccvsQCD = JetInfo.Jet_DeepBoostedJetZHccvsQCD[iJet];
    float deepBoostedJetWvsQCD = JetInfo.Jet_DeepBoostedJetWvsQCD[iJet];
    float deepBoostedJetZHbbvsQCD = JetInfo.Jet_DeepBoostedJetZHbbvsQCD[iJet];

    if(DEBUG_)std::cout << "got to FillHisto_JP" << std::endl;
    FillHisto(histoTag+"_TCHE",     flav, isGSPbb, isGSPcc ,tche      ,wt);
    FillHisto(histoTag+"_TCHP",     flav, isGSPbb, isGSPcc ,tchp      ,wt);
    FillHisto(histoTag+"_JP",       flav, isGSPbb, isGSPcc ,jetproba  ,wt);
    FillHisto(histoTag+"_JBP",      flav, isGSPbb, isGSPcc ,jetbproba ,wt);
    FillHisto(histoTag+"_SSV",      flav, isGSPbb, isGSPcc ,ssvhe     ,wt);
    FillHisto(histoTag+"_SSVHP",    flav, isGSPbb, isGSPcc ,ssvhp     ,wt);
    FillHisto(histoTag+"_CSVIVFv2", flav, isGSPbb, isGSPcc ,csvivfv2  ,wt);
    FillHisto(histoTag+"_cMVAv2",   flav, isGSPbb, isGSPcc ,cmvav2    ,wt);
    FillHisto(histoTag+"_DoubleB",  flav, isGSPbb, isGSPcc ,doubleb   ,wt);
    FillHisto(histoTag+"_TagVarCSV_sv_mass", flav, isGSPbb ,isGSPcc ,mass_TagVarCSV_sv,   wt);
    FillHisto2D(histoTag+"_TagVarCSV_sv_mass_vs_jetpt"        ,flav,isGSPbb , isGSPcc, ptjet,mass_TagVarCSV_sv,wt);
    FillHisto2D(histoTag+"_JP_DoubleB", flav,isGSPbb , isGSPcc, jetproba, doubleb, wt);
    FillHisto2D(histoTag+"_TagVarCSV_sv_mass_DoubleB", flav,isGSPbb , isGSPcc, mass_TagVarCSV_sv, doubleb, wt);
    FillHisto2D(histoTag+"_JP_v_DDBvL", flav,isGSPbb , isGSPcc, jetproba, deepDoubleBvLHbb, wt);
    FillHisto2D(histoTag+"_TagVarCSV_sv_mass_DDBvL"  , flav,isGSPbb , isGSPcc, mass_TagVarCSV_sv, deepDoubleBvLHbb, wt);
    FillHisto2D(histoTag+"_JP_v_DDCvL", flav,isGSPbb , isGSPcc, jetproba, deepDoubleCvLHcc, wt);
    FillHisto2D(histoTag+"_TagVarCSV_sv_mass_DDCvL"  , flav,isGSPbb , isGSPcc, mass_TagVarCSV_sv, deepDoubleCvLHcc, wt);
    FillHisto2D(histoTag+"_JP_v_DDCvB", flav,isGSPbb , isGSPcc, jetproba, deepDoubleCvBHcc, wt);
    FillHisto2D(histoTag+"_TagVarCSV_sv_mass_DDCvB"  , flav,isGSPbb , isGSPcc, mass_TagVarCSV_sv, deepDoubleCvBHcc, wt);
    FillHisto(histoTag+"_deepDoubleBvLQCD",  flav, isGSPbb, isGSPcc ,deepDoubleBvLQCD   ,wt);
    FillHisto(histoTag+"_deepDoubleBvLHbb",  flav, isGSPbb, isGSPcc ,deepDoubleBvLHbb   ,wt);
    FillHisto(histoTag+"_deepDoubleCvLQCD",  flav, isGSPbb, isGSPcc ,deepDoubleCvLQCD   ,wt);
    FillHisto(histoTag+"_deepDoubleCvLHcc",  flav, isGSPbb, isGSPcc ,deepDoubleCvLHcc   ,wt);
    FillHisto(histoTag+"_deepDoubleCvBHcc",  flav, isGSPbb, isGSPcc ,deepDoubleCvBHcc   ,wt);
    FillHisto(histoTag+"_deepDoubleCvBHbb",  flav, isGSPbb, isGSPcc ,deepDoubleCvBHbb   ,wt);
    FillHisto(histoTag+"_massIndDeepDoubleBvLQCD",  flav, isGSPbb, isGSPcc ,massIndDeepDoubleBvLQCD   ,wt);
    FillHisto(histoTag+"_massIndDeepDoubleBvLHbb",  flav, isGSPbb, isGSPcc ,massIndDeepDoubleBvLHbb   ,wt);
    FillHisto(histoTag+"_massIndDeepDoubleCvLQCD",  flav, isGSPbb, isGSPcc ,massIndDeepDoubleCvLQCD   ,wt);
    FillHisto(histoTag+"_massIndDeepDoubleCvLHcc",  flav, isGSPbb, isGSPcc ,massIndDeepDoubleCvLHcc   ,wt);
    FillHisto(histoTag+"_massIndDeepDoubleCvBHcc",  flav, isGSPbb, isGSPcc ,massIndDeepDoubleCvBHcc   ,wt);
    FillHisto(histoTag+"_massIndDeepDoubleCvBHbb",  flav, isGSPbb, isGSPcc ,massIndDeepDoubleCvBHbb   ,wt);
    FillHisto(histoTag+"_deepBoostedJetbbvsLight",  flav, isGSPbb, isGSPcc ,deepBoostedJetbbvsLight ,wt);
    FillHisto(histoTag+"_deepBoostedJetccvsLight",  flav, isGSPcc, isGSPcc ,deepBoostedJetccvsLight ,wt);
    FillHisto(histoTag+"_deepBoostedJetTvsQCD",  flav, isGSPbb, isGSPcc ,deepBoostedJetTvsQCD ,wt);
    FillHisto(histoTag+"_deepBoostedJetZHccvsQCD",  flav, isGSPbb, isGSPcc ,deepBoostedJetZHccvsQCD ,wt);
    FillHisto(histoTag+"_deepBoostedJetWvsQCD",  flav, isGSPbb, isGSPcc ,deepBoostedJetWvsQCD ,wt);
    FillHisto(histoTag+"_deepBoostedJetZHbbvsQCD", flav, isGSPbb, isGSPcc ,deepBoostedJetZHbbvsQCD ,wt);
    
    FillHisto(histoTag+"_TCHE_extended1",  flav, isGSPbb, isGSPcc ,tche  , wt);
    FillHisto(histoTag+"_TCHP_extended1",  flav, isGSPbb, isGSPcc ,tchp  , wt);
    FillHisto(histoTag+"_TCHE_extended2",  flav, isGSPbb, isGSPcc ,tche  , wt);
    FillHisto(histoTag+"_TCHP_extended2",  flav, isGSPbb, isGSPcc ,tchp  , wt);
    FillHisto(histoTag+"_discri_ssche0",   flav, isGSPbb, isGSPcc ,ssvhe , wt);
    FillHisto(histoTag+"_discri_sschp0",   flav, isGSPbb, isGSPcc ,ssvhp , wt);

    float softmu       = JetInfo.Jet_SoftMu[iJet];
    float solfel       = JetInfo.Jet_SoftEl[iJet];
    //DM>>float combsl       = JetInfo.Jet_CombCSVSL[iJet];

    FillHisto(histoTag+"_SoftMu",      flav, isGSPbb, isGSPcc ,softmu  ,   wt);
    FillHisto(histoTag+"_SoftEl",      flav, isGSPbb, isGSPcc ,solfel  ,   wt);
    //DM>>FillHisto(histoTag+"_CombCSVSL",   flav, isGSPbb, isGSPcc ,combsl  ,   wt);

    // ------------------------------------------------
    // ------------- muon information -----------------
    // ------------------------------------------------
    FillHisto(histoTag+"_muon_multi_sel",  flav, isGSPbb ,isGSPcc ,nselmuon   ,wt);
    FillHisto(histoTag+"_muon_multi",      flav, isGSPbb ,isGSPcc ,nmu        ,wt);

    if(idxFirstMuon > -1)
    {
      FillHisto(histoTag+"_mu_ptrel",    flav, isGSPbb ,isGSPcc ,JetInfo.PFMuon_ptrel[idxFirstMuon] ,wt);
      FillHisto(histoTag+"_mu_chi2",     flav, isGSPbb ,isGSPcc ,JetInfo.PFMuon_chi2[idxFirstMuon]  ,wt);
      FillHisto(histoTag+"_muon_Pt",     flav, isGSPbb ,isGSPcc ,JetInfo.PFMuon_pt[idxFirstMuon] ,     wt);
      FillHisto(histoTag+"_muon_eta",    flav, isGSPbb ,isGSPcc ,JetInfo.PFMuon_eta[idxFirstMuon] ,    wt);
      FillHisto(histoTag+"_muon_phi",    flav, isGSPbb ,isGSPcc ,JetInfo.PFMuon_phi[idxFirstMuon] ,    wt);
      FillHisto(histoTag+"_muon_Ip3d",   flav, isGSPbb ,isGSPcc ,JetInfo.PFMuon_IP[idxFirstMuon] ,     wt);
      FillHisto(histoTag+"_muon_Ip2d",   flav, isGSPbb ,isGSPcc ,JetInfo.PFMuon_IP2D[idxFirstMuon] ,   wt);

      TLorentzVector themuon, thejet;

      thejet.SetPtEtaPhiM(JetInfo.Jet_pt[iJet], JetInfo.Jet_eta[iJet], JetInfo.Jet_phi[iJet], JetInfo.Jet_mass[iJet]);
      themuon.SetPtEtaPhiM(JetInfo.PFMuon_pt[idxFirstMuon], JetInfo.PFMuon_eta[idxFirstMuon], JetInfo.PFMuon_phi[idxFirstMuon], 0);

      FillHisto(histoTag+"_muon_ptRatio",     flav, isGSPbb ,isGSPcc ,JetInfo.PFMuon_pt[idxFirstMuon]/ptjet ,     wt);
      FillHisto(histoTag+"_muon_DeltaR",         flav, isGSPbb, isGSPcc ,themuon.DeltaR(thejet) , wt);
      FillHisto2D(histoTag+"_muon_ptrel_vs_jetpt", flav, isGSPbb, isGSPcc ,ptjet,JetInfo.PFMuon_ptrel[idxFirstMuon],wt);
      FillHisto2D(histoTag+"_muon_DeltaR_vs_jetpt",flav, isGSPbb, isGSPcc ,ptjet,themuon.DeltaR(thejet),wt);
    }

  }

  void BTagValidation::fillJetHistos_DoubleB(const JetInfoBranches& JetInfo, const int iJet, const bool isGSPbb, const bool isGSPcc, const TString& histoTag, const int nmu, const int nselmuon, const int idxFirstMuon, TLorentzVector muon1_v, TLorentzVector muon2_v, TLorentzVector dimuon_v,const int idxMuon_inFirstSubjet, const int idxMuon_inSecondSubjet, TLorentzVector subjet1_p4 , TLorentzVector subjet2_p4, const double wt) {

      TLorentzVector fatjet_p4;
      fatjet_p4.SetPtEtaPhiM(JetInfo.Jet_pt[iJet], JetInfo.Jet_eta[iJet], JetInfo.Jet_phi[iJet], JetInfo.Jet_mass[iJet]);

    
      // ------------------------------------------------
      // ---------- fill fat jet Hbb tagger information -------------- added by rizki - start
      // ------------------------------------------------

      float z_ratio = JetInfo.Jet_z_ratio[iJet];

      float trackSip3dSig_3 = JetInfo.Jet_trackSip3dSig_3[iJet];
      float trackSip3dSig_2 = JetInfo.Jet_trackSip3dSig_2[iJet];
      float trackSip3dSig_1 = JetInfo.Jet_trackSip3dSig_1[iJet];
      float trackSip3dSig_0 = JetInfo.Jet_trackSip3dSig_0[iJet];

      float tau1_trackSip3dSig_1 = JetInfo.Jet_tau1_trackSip3dSig_1[iJet];
      float tau1_trackSip3dSig_0 = JetInfo.Jet_tau1_trackSip3dSig_0[iJet];
      float tau2_trackSip3dSig_1 = JetInfo.Jet_tau2_trackSip3dSig_1[iJet];
      float tau2_trackSip3dSig_0 = JetInfo.Jet_tau2_trackSip3dSig_0[iJet];

      float trackSip2dSigAboveCharm_0  = JetInfo.Jet_trackSip2dSigAboveCharm_0[iJet];
      float trackSip2dSigAboveBottom_0  = JetInfo.Jet_trackSip2dSigAboveBottom_0[iJet];
      float trackSip2dSigAboveBottom_1  = JetInfo.Jet_trackSip2dSigAboveBottom_1[iJet];

      float tau2_trackEtaRel_2 = JetInfo.Jet_tau2_trackEtaRel_2[iJet];
      float tau2_trackEtaRel_1 = JetInfo.Jet_tau2_trackEtaRel_1[iJet];
      float tau2_trackEtaRel_0 = JetInfo.Jet_tau2_trackEtaRel_0[iJet];
      float tau1_trackEtaRel_2 = JetInfo.Jet_tau1_trackEtaRel_2[iJet];
      float tau1_trackEtaRel_1 = JetInfo.Jet_tau1_trackEtaRel_1[iJet];
      float tau1_trackEtaRel_0 = JetInfo.Jet_tau1_trackEtaRel_0[iJet];
      float tau1_vertexMass   = JetInfo.Jet_tau1_vertexMass[iJet];
      float tau1_vertexEnergyRatio   = JetInfo.Jet_tau1_vertexEnergyRatio[iJet];
      float tau1_vertexDeltaR        = JetInfo.Jet_tau1_vertexDeltaR[iJet];
      float tau1_flightDistance2dSig = JetInfo.Jet_tau1_flightDistance2dSig[iJet];
      float tau2_vertexMass          = JetInfo.Jet_tau2_vertexMass[iJet];
      float tau2_vertexEnergyRatio   = JetInfo.Jet_tau2_vertexEnergyRatio[iJet];
      float tau2_flightDistance2dSig = JetInfo.Jet_tau2_flightDistance2dSig[iJet];

      float jetNTracks  = JetInfo.TagVarCSV_jetNTracks[iJet];
      float nSV = JetInfo.Jet_nSV_fat[iJet];

      float DoubleB = JetInfo.Jet_DoubleSV[iJet];

      // debug - start - rizki


      //All Region
      TString hTag = "FatJet";

        if(idxMuon_inFirstSubjet > -1){
          //         FillHisto(hTag+"_mu1_ptrel",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,JetInfo.PFMuon_ptrel[idxMuon_inFirstSubjet] ,wt);
          //         FillHisto(hTag+"_mu1_chi2",     JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,JetInfo.PFMuon_chi2[idxMuon_inFirstSubjet]  ,wt);
          FillHisto(hTag+"_muon1_Pt",     JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,muon1_v.Pt() ,     wt);
          FillHisto(hTag+"_muon1_eta",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,muon1_v.Eta() ,    wt);
          FillHisto(hTag+"_muon1_phi",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,muon1_v.Phi() ,    wt);      
          FillHisto(hTag+"_muon1_dR",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,muon1_v.DeltaR(fatjet_p4) ,    wt);       
          FillHisto(hTag+"_muon1_sj1_dR",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,muon1_v.DeltaR(subjet1_p4) ,    wt);      
        }
        if(idxMuon_inSecondSubjet > -1){
          //         FillHisto(hTag+"_mu2_ptrel",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,JetInfo.PFMuon_ptrel[idxMuon_inSecondSubjet] ,wt);
          //         FillHisto(hTag+"_mu2_chi2",     JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,JetInfo.PFMuon_chi2[idxMuon_inSecondSubjet]  ,wt);
          FillHisto(hTag+"_muon2_Pt",     JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,muon2_v.Pt() ,     wt);
          FillHisto(hTag+"_muon2_eta",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,muon2_v.Eta() ,    wt);
          FillHisto(hTag+"_muon2_phi",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,muon2_v.Phi() ,    wt);      
          FillHisto(hTag+"_muon2_dR",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,muon2_v.DeltaR(fatjet_p4) ,    wt);       
          FillHisto(hTag+"_muon2_sj2_dR",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,muon2_v.DeltaR(subjet2_p4) ,    wt);      
        }
        if(idxMuon_inFirstSubjet > -1 && idxMuon_inSecondSubjet > -1){
          FillHisto(hTag+"_dimuon_Pt",     JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,dimuon_v.Pt() ,     wt);
          FillHisto(hTag+"_dimuon_eta",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,dimuon_v.Eta() ,    wt);
          FillHisto(hTag+"_dimuon_phi",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,dimuon_v.Phi() ,    wt);      
          FillHisto(hTag+"_dimuon_ptRatio",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc , dimuon_v.Pt() / JetInfo.Jet_pt[iJet] ,wt);
          FillHisto(hTag+"_dimuon_dR",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,dimuon_v.DeltaR(fatjet_p4), wt);

          FillHisto("Subjet1_dR",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,subjet1_p4.DeltaR(fatjet_p4) ,    wt);        
          FillHisto("Subjet2_dR",    JetInfo.Jet_flavour[iJet], isGSPbb ,isGSPcc ,subjet2_p4.DeltaR(fatjet_p4) ,    wt);        
        }

        // debug - end - rizki

        FillHisto("FatJet_z_ratio",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, z_ratio  ,   wt);

        FillHisto("FatJet_trackSip3dSig_3",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, trackSip3dSig_3  ,   wt);
        FillHisto("FatJet_trackSip3dSig_2",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, trackSip3dSig_2  ,   wt);
        FillHisto("FatJet_trackSip3dSig_1",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, trackSip3dSig_1  ,   wt);
        FillHisto("FatJet_trackSip3dSig_0",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, trackSip3dSig_0  ,   wt);

        FillHisto("FatJet_tau1_trackSip3dSig_1",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau1_trackSip3dSig_1  ,   wt);
        FillHisto("FatJet_tau1_trackSip3dSig_0",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau1_trackSip3dSig_0  ,   wt);
        FillHisto("FatJet_tau2_trackSip3dSig_1",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau2_trackSip3dSig_1  ,   wt);
        FillHisto("FatJet_tau2_trackSip3dSig_0",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau2_trackSip3dSig_0  ,   wt);

        FillHisto("FatJet_trackSip2dSigAboveCharm_0",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, trackSip2dSigAboveCharm_0 ,   wt);
        FillHisto("FatJet_trackSip2dSigAboveBottom_0",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, trackSip2dSigAboveBottom_0 ,   wt);
        FillHisto("FatJet_trackSip2dSigAboveBottom_1",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, trackSip2dSigAboveBottom_1 ,   wt);

        FillHisto("FatJet_tau2_trackEtaRel_0",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau2_trackEtaRel_0  ,   wt);
        FillHisto("FatJet_tau2_trackEtaRel_1",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau2_trackEtaRel_1  ,   wt);
        FillHisto("FatJet_tau2_trackEtaRel_2",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau2_trackEtaRel_2  ,   wt);

        FillHisto("FatJet_tau1_trackEtaRel_0",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau1_trackEtaRel_0  ,   wt);
        FillHisto("FatJet_tau1_trackEtaRel_1",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau1_trackEtaRel_1  ,   wt);
        FillHisto("FatJet_tau1_trackEtaRel_2",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau1_trackEtaRel_2  ,   wt);

        FillHisto("FatJet_tau1_vertexMass"      ,      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau1_vertexMass ,   wt);
        FillHisto("FatJet_tau1_vertexEnergyRatio"   ,      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau1_vertexEnergyRatio ,   wt);
        FillHisto("FatJet_tau1_vertexDeltaR"    ,      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau1_vertexDeltaR ,   wt);
        FillHisto("FatJet_tau1_flightDistance2dSig" ,      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau1_flightDistance2dSig ,   wt);

        FillHisto("FatJet_tau2_vertexMass"      ,      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau2_vertexMass ,   wt);
        FillHisto("FatJet_tau2_vertexEnergyRatio"   ,      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau2_vertexEnergyRatio ,   wt);
        FillHisto("FatJet_tau2_flightDistance2dSig" ,      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, tau2_flightDistance2dSig ,   wt);

        FillHisto("FatJet_jetNTracks"       ,      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, jetNTracks ,   wt);
        FillHisto("FatJet_nSV"  ,      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, nSV ,   wt);

        FillHisto("FatJet_DoubleB",      JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, DoubleB  ,   wt);

        FillHisto2D("FatJet_DoubleB_trackSip3dSig_0" ,JetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,DoubleB ,trackSip3dSig_0 ,wt);
        FillHisto2D("FatJet_DoubleB_trackSip3dSig_1" ,JetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,DoubleB ,trackSip3dSig_1 ,wt);
        FillHisto2D("FatJet_DoubleB_trackSip3dSig_2" ,JetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,DoubleB ,trackSip3dSig_2 ,wt);
        FillHisto2D("FatJet_DoubleB_trackSip3dSig_3" ,JetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,DoubleB ,trackSip3dSig_3 ,wt);

        FillHisto2D("FatJet_DoubleB_jetNTracks" ,JetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,DoubleB ,jetNTracks ,wt);
        FillHisto2D("FatJet_jetNTracks_trackSip3dSig_0" ,JetInfo.Jet_flavour[iJet] ,isGSPbb ,isGSPcc ,jetNTracks ,trackSip3dSig_0 , wt);

        // ------------------------------------------------
        // ---------- fill fat jet Hbb tagger information (end) -------------- added by rizki - end
        // ------------------------------------------------

  
  }


void BTagValidation::fillJetHistos_DeepDoubleX(const JetInfoBranches& JetInfo, const int iJet, const bool isGSPbb, const bool isGSPcc, const TString& histoTag, const double wt) {

      TLorentzVector fatjet_p4;
      fatjet_p4.SetPtEtaPhiM(JetInfo.Jet_pt[iJet], JetInfo.Jet_eta[iJet], JetInfo.Jet_phi[iJet], JetInfo.Jet_mass[iJet]);

    FillHisto("FatJet_DeepDoubleX_jetNTracks"              , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_jetNTracks[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_nSV"  , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_jetNSecondaryVertices[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_trackSip3dSig_0", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_trackSip3dSig_0[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_trackSip3dSig_1", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_trackSip3dSig_1[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_trackSip3dSig_2", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_trackSip3dSig_2[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_trackSip3dSig_3", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_trackSip3dSig_3[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau1_trackSip3dSig_0", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau1_trackSip3dSig_0[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau1_trackSip3dSig_1", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau1_trackSip3dSig_1[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau2_trackSip3dSig_0", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau2_trackSip3dSig_0[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau2_trackSip3dSig_1", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau2_trackSip3dSig_1[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_trackSip2dSigAboveBottom_0", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_trackSip2dSigAboveBottom_0[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_trackSip2dSigAboveBottom_1", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_trackSip2dSigAboveBottom_1[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_trackSip2dSigAboveCharm", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_trackSip2dSigAboveCharm[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau1_trackEtaRel_0", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau1_trackEtaRel_0[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau1_trackEtaRel_1", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau1_trackEtaRel_1[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau1_trackEtaRel_2", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau1_trackEtaRel_2[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau2_trackEtaRel_0", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau2_trackEtaRel_0[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau2_trackEtaRel_1", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau2_trackEtaRel_1[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau2_trackEtaRel_2", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau2_trackEtaRel_2[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau1_vertexMass"             , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau1_vertexMass[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau1_vertexEnergyRatio"      , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau1_vertexEnergyRatio[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau1_flightDistance2dSig"    , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau1_flightDistance2dSig[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau1_vertexDeltaR"           , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau1_vertexDeltaR[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau2_vertexMass"             , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau2_vertexMass[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau2_vertexEnergyRatio"      , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau2_vertexEnergyRatio[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau2_flightDistance2dSig"    , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau2_flightDistance2dSig[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_tau2_vertexDeltaR"           , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_tau2_vertexDeltaR[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_z_ratio"           , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_z_ratio[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_charged_EtaRel", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_charged_EtaRel[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_charged_PtRatio", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_charged_PtRatio[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_charged_PParRatio", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_charged_PParRatio[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_charged_PtRatio", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_charged_Sip2dVal[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_charged_Sip2dVal", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_charged_Sip2dSig[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_charged_Sip2dSig", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_charged_Sip2dSig[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_charged_Sip3dVal", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_charged_Sip3dVal[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_charged_Sip3dSig", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_charged_Sip3dSig[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_charged_JetDistVal", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_charged_JetDistVal[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_sv_d3d"            , JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_sv_d3d[iJet], wt );
    FillHisto("FatJet_DeepDoubleX_sv_d3dsig", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepDoubleXInput_sv_d3dsig[iJet], wt );

    FillHisto("FatJet_DeepBoostedJet_pf_puppiw", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_puppiw[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_hcalFrac", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_hcalFrac[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_VTX_ass", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_VTX_ass[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_lostInnerHits", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_lostInnerHits[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_quality", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_quality[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_charge", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_charge[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_isEl", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_isEl[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_isMu", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_isMu[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_isChargedHad", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_isChargedHad[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_isGamma", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_isGamma[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_isNeutral", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_isNeutralHad[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_phirel", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_phirel[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_etarel", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_etarel[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_deltaR", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_deltaR[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_abseta", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_abseta[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_ptrel_log", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_ptrel_log[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_erel_log", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_erel_log[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_pt_log", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_pt_log[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_drminsv", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_drminsv[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_drsubjet1", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_drsubjet1[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_drsubjet2", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_drsubjet2[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_normchi2", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_normchi2[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dz", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dz[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dzsig", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dzsig[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dxy", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dxy[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dxysig", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dxysig[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dptdpt", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dptdpt[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_detadeta", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_detadeta[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dphidphi", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dphidphi[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dxydxy", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dxydxy[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dzdz", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dzdz[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dxydz", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dxydz[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dphidxy", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dphidxy[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_dlambdadz", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_dlambdadz[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_btagEtaRel", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_btagEtaRel[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_btagPtRatio", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_btagPtRatio[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_btagPParRatio", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_btagPParRatio[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_btagSip2dVal", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_btagSip2dVal[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_btagSip2dSig", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_btagSip2dSig[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_btagSip3dVal", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_btagSip3dVal[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_btagSip3dSig", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_btagSip3dSig[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_pf_btagJetDistVal", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_pf_btagJetDistVal[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_phirel", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_phirel[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_etarel", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_etarel[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_deltaR", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_deltaR[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_abseta", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_abseta[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_mass", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_mass[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_ptrel_log", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_ptrel_log[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_erel_log", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_erel_log[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_pt_log", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_pt_log[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_ntracks", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_ntracks[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_normchi2", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_normchi2[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_dxy", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_dxy[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_dxysig", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_dxysig[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_d3d", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_d3d[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_d3dsig", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_d3dsig[iJet], wt );
    FillHisto("FatJet_DeepBoostedJet_sv_costhetasvpv", JetInfo.Jet_flavour[iJet], isGSPbb, isGSPcc, JetInfo.DeepBoostedJetInput_sv_costhetasvpv[iJet], wt );

  }


  void BTagValidation::fillJetHistos_SF(const JetInfoBranches& JetInfo, const int iJet, const bool isGSPbb, const bool isGSPcc, 
                                        const TString& histoTag, const TString& LTSVvar, const bool useSVmass,
                                        const TString& tagger, const std::map<TString,double>&  WPmap, const std::map<TString,std::vector<int>>& ptbinmap, 
                                        const double wt) {
    
    if (DEBUG_) std::cout << "FILLING SF HISTOS with " << LTSVvar << std::endl;

    float jetpt      = JetInfo.Jet_pt[iJet];
    float jeteta     = JetInfo.Jet_eta[iJet];
    int   flav       = JetInfo.Jet_flavour[iJet];

    float LTSV_value  = JetInfo.Jet_Proba[iJet];
    if (useSVmass) {
        LTSV_value  = JetInfo.Jet_tau1_vertexMass_corrected[iJet];
    }
    float tagger_value = 0.;
    if (tagger == TString("DoubleB")) tagger_value  = JetInfo.Jet_DoubleSV[iJet];
    if (tagger == TString("DDBvL"))   tagger_value  = JetInfo.Jet_MassIndDeepDoubleBvLHbb[iJet];
    if (tagger == TString("DDCvL"))   tagger_value  = JetInfo.Jet_MassIndDeepDoubleCvLHcc[iJet];
    if (tagger == TString("DDCvB"))   tagger_value  = JetInfo.Jet_MassIndDeepDoubleCvBHcc[iJet];
    if (tagger == TString("DeepAK8bb"))   tagger_value  = JetInfo.Jet_DeepBoostedJetbbvsLight[iJet];
    if (tagger == TString("DeepAK8cc"))   tagger_value  = JetInfo.Jet_DeepBoostedJetccvsLight[iJet];
    if (tagger == TString("DeepAK8ZHbb"))   tagger_value  = JetInfo.Jet_DeepBoostedJetZHbbvsQCD[iJet];
    if (tagger == TString("DeepAK8ZHcc"))   tagger_value  = JetInfo.Jet_DeepBoostedJetZHccvsQCD[iJet];
    
    if (DEBUG_) std::cout << "FILLING JP/SV = "<< LTSV_value << " Filling tagger = "<< tagger_value << std::endl;

    // for SF LT method calculation - start
    double jesup(1.0), jesdown(1.0);
    if ( doJECUncert_ ) {
        jesup = GetJESUncert(1.0, jetpt, jeteta, jesup) ; 
        jesdown = GetJESUncert(-1.0, jetpt, jeteta, jesdown) ; 
    }
            if (DEBUG_) {
            edm::LogInfo("JEC") << " jetpt = " << jetpt 
              << " jes uncert up = " << jesup 
              << " jes uncert down = " << jesdown
              << " jetpt_up = " << jetpt*(1 + jesup)
              << " jetpt_down = " << jetpt*(1+jesdown) ; 
              }
    double jetpt_up, jetpt_down;
    jetpt_up =  jetpt*(1 + jesup);
    jetpt_down = jetpt*(1+jesdown) ;
                        
    std::vector<TString> JECstring = {"JECup","JECdown"};
    
    std::vector<TString> ptStr;
    boost::copy(ptbinmap | boost::adaptors::map_keys, std::back_inserter(ptStr));
    std::vector<TString> WPStr;
    boost::copy(WPmap | boost::adaptors::map_keys, std::back_inserter(WPStr));

    //new - JP nominal - start 
    for(unsigned int ipt=0; ipt < ptStr.size(); ipt++ ){
        if(DEBUG_ && DEBUGlevel_>1 )std::cout << ptbinmap.at(ptStr.at(ipt)).at(0) << ","<< ptbinmap.at(ptStr.at(ipt)).at(1) << ", JP notag ," << ptStr.at(ipt) << " nominal " <<std::endl;
        if ( jetpt > ptbinmap.at(ptStr.at(ipt)).at(0) && jetpt <= ptbinmap.at(ptStr.at(ipt)).at(1) ) FillHisto(histoTag+"_"+LTSVvar+"_all_"+ptStr.at(ipt), flav, isGSPbb, isGSPcc ,LTSV_value  ,wt); //400-450
    }

    for(unsigned int iWP=0; iWP < WPStr.size(); iWP++ ){
        if(DEBUG_ && DEBUGlevel_>1 )std::cout << "WP = " << WPmap.at(WPStr.at(iWP)) << "(" << iWP <<")" << std::endl;
        if( tagger_value >= WPmap.at(WPStr.at(iWP)) ){
          
            for(unsigned int ipt=0; ipt < ptStr.size(); ipt++ ){
                if(DEBUG_ && DEBUGlevel_>1 )std::cout << ptbinmap.at(ptStr.at(ipt)).at(0) << ","<< ptbinmap.at(ptStr.at(ipt)).at(1) << "," << WPStr.at(iWP) << ","<< ptStr.at(ipt) << " pass - nominal"<<std::endl;
                if ( jetpt > ptbinmap.at(ptStr.at(ipt)).at(0) && jetpt <= ptbinmap.at(ptStr.at(ipt)).at(1) ) {
                    FillHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_pass_"+ptStr.at(ipt), flav, isGSPbb, isGSPcc ,LTSV_value  ,wt);
                }
            }
        }
        else if ( tagger_value < WPmap.at(WPStr.at(iWP)) ) {
            for(unsigned int ipt=0; ipt < ptStr.size(); ipt++ ){
                if(DEBUG_ && DEBUGlevel_>1 )std::cout << ptbinmap.at(ptStr.at(ipt)).at(0) << ","<< ptbinmap.at(ptStr.at(ipt)).at(1) << "," << WPStr.at(iWP) << ","<< ptStr.at(ipt) << " fail - nominal"<<std::endl;
                if ( jetpt > ptbinmap.at(ptStr.at(ipt)).at(0) && jetpt <= ptbinmap.at(ptStr.at(ipt)).at(1) ){
                    FillHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_fail_"+ptStr.at(ipt), flav, isGSPbb, isGSPcc ,LTSV_value  ,wt); 
                }
            }
        }
        
    }
    //new - JP nominal - end

    //new - JEC up - start 
    for(unsigned int ipt=0; ipt < ptStr.size(); ipt++ ){
        if(DEBUG_ && DEBUGlevel_>1 )std::cout << ptbinmap.at(ptStr.at(ipt)).at(0) << ","<< ptbinmap.at(ptStr.at(ipt)).at(1) << "," << ptStr.at(ipt) << " notag - JESup"<<std::endl;
        if ( jetpt_up > ptbinmap.at(ptStr.at(ipt)).at(0) && jetpt_up <= ptbinmap.at(ptStr.at(ipt)).at(1) ) FillHisto(histoTag+"_"+LTSVvar+"_all_"+ptStr.at(ipt)+"_JESup", flav, isGSPbb, isGSPcc , LTSV_value  ,wt);
    }

    for(unsigned int iWP=0; iWP < WPStr.size(); iWP++ ){
        if(DEBUG_ && DEBUGlevel_>1 )std::cout << "WP = " << WPmap.at(WPStr.at(iWP)) << "(" << iWP <<")" << std::endl;
        if( tagger_value >= WPmap.at(WPStr.at(iWP)) ){

            for(unsigned int ipt=0; ipt < ptStr.size(); ipt++ ){
                if(DEBUG_ && DEBUGlevel_>1 )std::cout << ptbinmap.at(ptStr.at(ipt)).at(0) << ","<< ptbinmap.at(ptStr.at(ipt)).at(1) << "," << WPStr.at(iWP) << ","<< ptStr.at(ipt) << " pass - JECup"<<std::endl;
                if ( jetpt_up > ptbinmap.at(ptStr.at(ipt)).at(0) && jetpt_up <= ptbinmap.at(ptStr.at(ipt)).at(1) ) FillHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_pass_"+ptStr.at(ipt)+"_JESup", flav, isGSPbb, isGSPcc , LTSV_value  ,wt);
            }
        }
        else if ( tagger_value < WPmap.at(WPStr.at(iWP)) ) {

            for(unsigned int ipt=0; ipt < ptStr.size(); ipt++ ){
                if(DEBUG_ && DEBUGlevel_>1 )std::cout << ptbinmap.at(ptStr.at(ipt)).at(0) << ","<< ptbinmap.at(ptStr.at(ipt)).at(1) << "," << WPStr.at(iWP) << ","<< ptStr.at(ipt) << " fail - JECup"<<std::endl;
                if ( jetpt_up > ptbinmap.at(ptStr.at(ipt)).at(0) && jetpt_up <= ptbinmap.at(ptStr.at(ipt)).at(1) ) FillHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_fail_"+ptStr.at(ipt)+"_JESup", flav, isGSPbb, isGSPcc ,LTSV_value  ,wt); 
            }
        }
        
    }
    //new - JEC up - end

    //new - JECdown - start 
    for(unsigned int ipt=0; ipt < ptStr.size(); ipt++ ){
        if(DEBUG_ && DEBUGlevel_>1 )std::cout << ptbinmap.at(ptStr.at(ipt)).at(0) << ","<< ptbinmap.at(ptStr.at(ipt)).at(1) << "," << ptStr.at(ipt) << " notag - JESdown"<<std::endl;
        if ( jetpt_down > ptbinmap.at(ptStr.at(ipt)).at(0) && jetpt_down <= ptbinmap.at(ptStr.at(ipt)).at(1) ) FillHisto(histoTag+"_"+LTSVvar+"_all_"+ptStr.at(ipt)+"_JESdown", flav, isGSPbb, isGSPcc , LTSV_value  ,wt);
    }

    for(unsigned int iWP=0; iWP < WPStr.size(); iWP++ ){
        if(DEBUG_ && DEBUGlevel_>1 )std::cout << "WP = " << WPmap.at(WPStr.at(iWP)) << "(" << iWP <<")" << std::endl;
        if( tagger_value >= WPmap.at(WPStr.at(iWP)) ){

            for(unsigned int ipt=0; ipt < ptStr.size(); ipt++ ){
                if(DEBUG_ && DEBUGlevel_>1 )std::cout << ptbinmap.at(ptStr.at(ipt)).at(0) << ","<< ptbinmap.at(ptStr.at(ipt)).at(1) << "," << WPStr.at(iWP) << ","<< ptStr.at(ipt) << " pass - JECdown"<<std::endl;
                if ( jetpt_down > ptbinmap.at(ptStr.at(ipt)).at(0) && jetpt_down <= ptbinmap.at(ptStr.at(ipt)).at(1) ) FillHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_pass_"+ptStr.at(ipt)+"_JESdown", flav, isGSPbb, isGSPcc , LTSV_value  ,wt);
            }
        }
        else if ( tagger_value < WPmap.at(WPStr.at(iWP)) ) {

            for(unsigned int ipt=0; ipt < ptStr.size(); ipt++ ){
                if(DEBUG_ && DEBUGlevel_>1 )std::cout << ptbinmap.at(ptStr.at(ipt)).at(0) << ","<< ptbinmap.at(ptStr.at(ipt)).at(1) << "," << WPStr.at(iWP) << ","<< ptStr.at(ipt) << " fail - JECdown"<<std::endl;
                if ( jetpt_down > ptbinmap.at(ptStr.at(ipt)).at(0) && jetpt_down <= ptbinmap.at(ptStr.at(ipt)).at(1) ) FillHisto(histoTag+"_"+LTSVvar+"_"+tagger+"_"+WPStr.at(iWP)+"_fail_"+ptStr.at(ipt)+"_JESdown", flav, isGSPbb, isGSPcc ,LTSV_value  ,wt); 
            }
        }
        
    }
    //new - JECdown -end

    // for SF LT method calculation - end

  }

  // 


  // ------------ method called once each job just after ending the event loop  ------------
  void BTagValidation::endJob() {

    h1_CutFlow->SetBinContent(1, nEventsAll); //// strictly speaking not correct since event weights not applied
    h1_CutFlow->SetBinContent(2, nEventsStored); //// strictly speaking not correct since event weights not applied
    h1_CutFlow_unw->SetBinContent(1, nEventsAll);
    h1_CutFlow_unw->SetBinContent(2, nEventsStored);
    h1_CutFlow->SetBinError(1, TMath::Sqrt(nEventsAll)); //// strictly speaking not correct since event weights not applied
    h1_CutFlow->SetBinError(2, TMath::Sqrt(nEventsStored)); //// strictly speaking not correct since event weights not applied
    h1_CutFlow_unw->SetBinError(1, TMath::Sqrt(nEventsAll));
    h1_CutFlow_unw->SetBinError(2, TMath::Sqrt(nEventsStored));
  }

  // ------------ method called when starting to processes a run  ------------
  void BTagValidation::beginRun(edm::Run const&, edm::EventSetup const&) {
  }

  // ------------ method called when ending the processing of a run  ------------
  void BTagValidation::endRun(edm::Run const&, edm::EventSetup const&) {
  }

  // ------------ method called when starting to processes a luminosity block  ------------
  void BTagValidation::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
  }

  // ------------ method called when ending the processing of a luminosity block  ------------
  void BTagValidation::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
  }

  // ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
  void BTagValidation::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
  }

  // ------------------------------------------------------------------------------
  void BTagValidation::AddHisto(const TString& name, const TString& title, const int nbins, const double min, const double max)  {

    TH1D* h_b;
    TH1D* h_bfromg;
    TH1D* h_c;
    TH1D* h_cfromg;
    TH1D* h_l;
    TH1D* h_mc;

    if( useFlavorCategories_ )
    {
      h_b      = fs->make<TH1D>(name+"_b"     ,title+"_b"     ,nbins,min,max);
      h_bfromg = fs->make<TH1D>(name+"_bfromg",title+"_bfromg",nbins,min,max);
      h_c      = fs->make<TH1D>(name+"_c"     ,title+"_c"     ,nbins,min,max);
      h_cfromg = fs->make<TH1D>(name+"_cfromg",title+"_cfromg",nbins,min,max);
      h_l      = fs->make<TH1D>(name+"_l"     ,title+"_l"     ,nbins,min,max);

      h_b      ->Sumw2();
      h_bfromg ->Sumw2();
      h_c      ->Sumw2();
      h_cfromg ->Sumw2();
      h_l      ->Sumw2();

      HistoBtag_map[name+"_b"]      = h_b;
      HistoBtag_map[name+"_bfromg"] = h_bfromg;
      HistoBtag_map[name+"_c"]      = h_c;
      HistoBtag_map[name+"_cfromg"] = h_cfromg;
      HistoBtag_map[name+"_l"]      = h_l;
    }
    else
    {
      h_mc = fs->make<TH1D>(name+"_mc",title+"_mc",nbins,min,max);
      h_mc->Sumw2();
      HistoBtag_map[name+"_mc"] = h_mc;
    }

    TH1D* h_data= fs->make<TH1D>(name+"_data",title+"_data",nbins,min,max);
    h_data->Sumw2();
    HistoBtag_map[name+"_data"] = h_data;
  }

  // ------------------------------------------------------------------------------
  void BTagValidation::AddHisto2D(const TString& name, const TString& title, const int nbins, const double min, const double max, const int nbins2, const double min2, const double max2) {
    TH2D* h_b;
    TH2D* h_bfromg;
    TH2D* h_c;
    TH2D* h_cfromg;
    TH2D* h_l;
    TH2D* h_mc;

    if( useFlavorCategories_ ) {
      h_b      = fs->make<TH2D>(name+"_b"     ,title+"_b"     ,nbins,min,max,nbins2,min2,max2);
      h_bfromg = fs->make<TH2D>(name+"_bfromg",title+"_bfromg",nbins,min,max,nbins2,min2,max2);
      h_c      = fs->make<TH2D>(name+"_c"     ,title+"_c"     ,nbins,min,max,nbins2,min2,max2);
      h_cfromg = fs->make<TH2D>(name+"_cfromg",title+"_cfromg",nbins,min,max,nbins2,min2,max2);
      h_l      = fs->make<TH2D>(name+"_l"     ,title+"_l"     ,nbins,min,max,nbins2,min2,max2);

      h_b      ->Sumw2();
      h_bfromg ->Sumw2();
      h_c      ->Sumw2();
      h_cfromg ->Sumw2();
      h_l      ->Sumw2();

      HistoBtag2D_map[name+"_b"]      = h_b;
      HistoBtag2D_map[name+"_bfromg"] = h_bfromg;
      HistoBtag2D_map[name+"_c"]      = h_c;
      HistoBtag2D_map[name+"_cfromg"] = h_cfromg;
      HistoBtag2D_map[name+"_l"]      = h_l;
    } else {
      h_mc = fs->make<TH2D>(name+"_mc",title+"_mc",nbins,min,max,nbins2,min2,max2);
      h_mc->Sumw2();
      HistoBtag2D_map[name+"_mc"] = h_mc;
    }

    TH2D* h_data= fs->make<TH2D>(name+"_data",title+"_data",nbins,min,max,nbins2,min2,max2);
    h_data->Sumw2();
    HistoBtag2D_map[name+"_data"] = h_data;
  }

  // ------------------------------------------------------------------------------
  template <class Type>
    void BTagValidation::FillHisto(const TString& name, const int flavour, const bool isGSPbb , const bool isGSPcc , const Type value, const double weight) {
      if (!isData) {
        if( useFlavorCategories_ ) {
          if( isGSPbb ) HistoBtag_map[name+"_bfromg"]->Fill(double(value),weight);
          else if( isGSPcc ) HistoBtag_map[name+"_cfromg"]->Fill(double(value),weight);
          //RS>>else if (isGSPcc&&abs(flavour)==4)        HistoBtag_map[name+"_cfromg"]->Fill(double(value),weight);
          else {
            if (abs(flavour)==5)                          HistoBtag_map[name+"_b"]->Fill(double(value),weight);
            else if (abs(flavour)==4)                     HistoBtag_map[name+"_c"]->Fill(double(value),weight);
            else if (abs(flavour)< 4 || abs(flavour)==21) HistoBtag_map[name+"_l"]->Fill(double(value),weight);
          }
        }
        else HistoBtag_map[name+"_mc"]->Fill(double(value),weight);
      }
      else {
        HistoBtag_map[name+"_data"]->Fill(double(value));
      }
    }

  // ------------------------------------------------------------------------------
  template <class Type1, class Type2>
    void BTagValidation::FillHisto2D(const TString& name, const int flavour, const bool isGSPbb ,const bool isGSPcc , const Type1 value, const Type2 value2, const double weight) {
      if (!isData) {
        if( useFlavorCategories_ ) {
          if( isGSPbb ) HistoBtag2D_map[name+"_bfromg"]->Fill(double(value),double(value2),weight);
          else if( isGSPcc ) HistoBtag2D_map[name+"_cfromg"]->Fill(double(value),double(value2),weight);
          //RS>>else if (isGSPcc && abs(flavour)==4)          HistoBtag2D_map[name+"_cfromg"]->Fill(double(value),double(value2),weight);
          else {
            if (abs(flavour)==5)                          HistoBtag2D_map[name+"_b"]->Fill(double(value),double(value2),weight);
            else if (abs(flavour)==4)                     HistoBtag2D_map[name+"_c"]->Fill(double(value),double(value2),weight);
            else if (abs(flavour)< 4 || abs(flavour)==21) HistoBtag2D_map[name+"_l"]->Fill(double(value),double(value2),weight);
          }
        }
        else HistoBtag2D_map[name+"_mc"]->Fill(double(value),double(value2),weight);
      }
      else HistoBtag2D_map[name+"_data"]->Fill(double(value),double(value2));
    }

  // ------------------------------------------------------------------------------
  bool BTagValidation::passTrigger() {
  bool ret = false;

  if(triggerSelection_.size()==0) ret = true;
  else {
    std::vector<bool> triggerBits;
    for(unsigned i=0; i<triggerSelection_.size(); ++i) {
      triggerBits.push_back(false);
      //if(DEBUG_)std::cout << "i = " << i << std::endl;
      std::string trigpath = triggerSelection_.at(i) ; 
      std::vector<std::string>::const_iterator it ;
      for ( it = triggerPathNames_.begin(); it != triggerPathNames_.end(); ++it) {
        if(DEBUG_ && DEBUGlevel_>=1)std::cout << "Comparing triggers : " << *it << " -- " << trigpath << " = " << it->find(trigpath) <<std::endl;
        if ( it->find(trigpath) < std::string::npos ) {
          int triggerIdx = ( it - triggerPathNames_.begin() );
          int bitIdx = int(triggerIdx/32);
          if(DEBUG_ && DEBUGlevel_>=1)std::cout << "Checking (matching) trigger bits : " << *it << std::endl;
          if(DEBUG_ && DEBUGlevel_>=1)std::cout << "                                   " << " triggerIdx                        = " << triggerIdx << std::endl;
          if(DEBUG_ && DEBUGlevel_>=1)std::cout << "                                   " << " bitIdx                            = " << bitIdx << std::endl;
          if(DEBUG_ && DEBUGlevel_>=1)std::cout << "                                   " << " (triggerIdx - bitIdx*32)          = " << (triggerIdx - bitIdx*32) << std::endl;
          if(DEBUG_ && DEBUGlevel_>=1)std::cout << "                                   " << " ( 1 << (triggerIdx - bitIdx*32) ) = " << ( 1 <<(triggerIdx - bitIdx*32) )<< std::endl;
          if(DEBUG_ && DEBUGlevel_>=1)std::cout << "                                   " << " EvtInfo.BitTrigger[bitIdx]        = " << EvtInfo.BitTrigger[bitIdx] << std::endl;
          if(DEBUG_ && DEBUGlevel_>=1)std::cout << "                                   " << " ( EvtInfo.BitTrigger[bitIdx] & ( 1 << (triggerIdx - bitIdx*32) ) )  = " << ( EvtInfo.BitTrigger[bitIdx] & ( 1 << (triggerIdx - bitIdx*32) ) ) << std::endl;
          if ( EvtInfo.BitTrigger[bitIdx] & ( 1 << (triggerIdx - bitIdx*32) ) ) {
            if(DEBUG_)std::cout << "fired trigger : " << *it << std::endl;
            triggerBits.at(i) = true;
            break; 
          }
        }
      }
    }
    
    bool isOR = triggerLogicIsOR_; 
    
    for(unsigned i=0; i<triggerSelection_.size(); ++i) {

        // makes trigger logic: AND
        if(!triggerBits.at(i) && !isOR){
            ret=false;
            break;
        }
        else if(!isOR){
            ret=true;
        }

        // makes trigger logic: OR
        if(triggerBits.at(i) && isOR){
            ret=true;
            break;
        } 
    }
    
    
  }

  return ret;
}

  // ------------------------------------------------------------------------------
  bool BTagValidation::passMuonSelection(const int muIdx, const JetInfoBranches& JetInfo, const int iJet, const double deltaR) {
    TLorentzVector muon, jet;

    jet.SetPtEtaPhiM(JetInfo.Jet_pt[iJet], JetInfo.Jet_eta[iJet], JetInfo.Jet_phi[iJet], JetInfo.Jet_mass[iJet]);
    muon.SetPtEtaPhiM(JetInfo.PFMuon_pt[muIdx], JetInfo.PFMuon_eta[muIdx], JetInfo.PFMuon_phi[muIdx], 0);

    bool cut_mu_pass = false;
    if (JetInfo.PFMuon_pt[muIdx] > muonPtMin_ && fabs(JetInfo.PFMuon_eta[muIdx]) < 2.4 
        && JetInfo.PFMuon_isGlobal[muIdx] == 1 
        && JetInfo.PFMuon_nMuHit[muIdx] > 0 
        && JetInfo.PFMuon_nMatched[muIdx] > 1 
        && JetInfo.PFMuon_nTkHit[muIdx] > (useRelaxedMuonID_ ? 7 : 10) 
        && JetInfo.PFMuon_nPixHit[muIdx] > (useRelaxedMuonID_ ? 0 : 1) 
        && JetInfo.PFMuon_nOutHit[muIdx] < (useRelaxedMuonID_ ? 99 : 3) 
        && JetInfo.PFMuon_chi2Tk[muIdx] < 10 
        && JetInfo.PFMuon_chi2[muIdx] < 10  
        && (jet.DeltaR(muon) < deltaR && muon.Pt()/jet.Pt() < MuonJetPtRatio_ )
        //&& JetInfo.PFMuon_vz[muIdx]< 2 
        //DM&& fabs(JetInfo.PFMuon_vz[muIdx]-EvtInfo.PVz) < 2.
       )
      cut_mu_pass = true ;

    return cut_mu_pass;
  }

  // ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor  ------------
  double BTagValidation::scaleFactor(const int partonFlavor, const double jetPt, const double jetEta, const bool isCSVM) {
    if( isCSVM ) {
      if( abs(partonFlavor)==5 ) return scaleFactorB_CSVM(jetPt,jetEta);
      else if( abs(partonFlavor)==4 ) return scaleFactorC_CSVM(jetPt,jetEta);
      else return scaleFactorUDSG_CSVM(jetPt,jetEta);
    }
    else {
      if( abs(partonFlavor)==5 ) return scaleFactorB_CSVL(jetPt,jetEta);
      else if( abs(partonFlavor)==4 ) return scaleFactorC_CSVL(jetPt,jetEta);
      else return scaleFactorUDSG_CSVL(jetPt,jetEta);
    }
  }

  // ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for b-jets and CSVL tagger  ------------
  double BTagValidation::scaleFactorB_CSVL(const double jetPt, const double jetEta) {
    double Pt = jetPt;
    // for scale factor extrapolation
    if(Pt<20.) Pt = 20.;
    if(Pt>800.) Pt = 800.;

    return CSVL_SFb_0to2p4->Eval(Pt) + SFbShift_*CSVL_SFb_errors->GetBinContent(CSVL_SFb_errors->GetXaxis()->FindBin(jetPt));
  }

  // ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for c-jets and CSVL tagger  ------------
  double BTagValidation::scaleFactorC_CSVL(const double jetPt, const double jetEta) {
    double Pt = jetPt;
    // for scale factor extrapolation
    if(Pt<20.) Pt = 20.;
    if(Pt>800.) Pt = 800.;

    return CSVL_SFb_0to2p4->Eval(Pt) + 2*SFbShift_*CSVL_SFb_errors->GetBinContent(CSVL_SFb_errors->GetXaxis()->FindBin(jetPt));
  }

  // ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for light flavor jets and CSVL tagger ------------
  double BTagValidation::scaleFactorUDSG_CSVL(const double jetPt, const double jetEta) {
    double SF = 1.;
    double Pt = jetPt;
    double absEta = fabs(jetEta);
    // for scale factor extrapolation
    if(Pt<20.) Pt = 20.;

    if(absEta<0.5) {
      if(Pt>1000.) Pt = 1000.;

      SF = CSVL_SFl_0to0p5->Eval(Pt) + ( (jetPt<20. || jetPt>1000.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVL_SFl_0to0p5_max->Eval(Pt) - CSVL_SFl_0to0p5->Eval(Pt)) : (CSVL_SFl_0to0p5_min->Eval(Pt) - CSVL_SFl_0to0p5->Eval(Pt)) );
    }
    else if(absEta>=0.5 && absEta<1.) {
      if(Pt>1000.) Pt = 1000.;

      SF = CSVL_SFl_0p5to1p0->Eval(Pt) + ( (jetPt<20. || jetPt>1000.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVL_SFl_0p5to1p0_max->Eval(Pt) - CSVL_SFl_0p5to1p0->Eval(Pt)) : (CSVL_SFl_0p5to1p0_min->Eval(Pt) - CSVL_SFl_0p5to1p0->Eval(Pt)) );
    }
    else if(absEta>=1. && absEta<1.5) {
      if(Pt>1000.) Pt = 1000.;

      SF = CSVL_SFl_1p0to1p5->Eval(Pt) + ( (jetPt<20. || jetPt>1000.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVL_SFl_1p0to1p5_max->Eval(Pt) - CSVL_SFl_1p0to1p5->Eval(Pt)) : (CSVL_SFl_1p0to1p5_min->Eval(Pt) - CSVL_SFl_1p0to1p5->Eval(Pt)) );
    }
    else {
      if(Pt>850.) Pt = 850.;

      SF = CSVL_SFl_1p5to2p4->Eval(Pt) + ( (jetPt<20. || jetPt>850.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVL_SFl_1p5to2p4_max->Eval(Pt) - CSVL_SFl_1p5to2p4->Eval(Pt)) : (CSVL_SFl_1p5to2p4_min->Eval(Pt) - CSVL_SFl_1p5to2p4->Eval(Pt)) );
    }

    return SF;
  }

  // ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for b-jets and CSVM tagger  ------------
  double BTagValidation::scaleFactorB_CSVM(const double jetPt, const double jetEta) {
    double Pt = jetPt;
    // for scale factor extrapolation
    if(Pt<20.) Pt = 20.;
    if(Pt>800.) Pt = 800.;

    return CSVM_SFb_0to2p4->Eval(Pt) + SFbShift_*CSVM_SFb_errors->GetBinContent(CSVM_SFb_errors->GetXaxis()->FindBin(jetPt));
  }

  // ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for c-jets and CSVM tagger  ------------
  double BTagValidation::scaleFactorC_CSVM(const double jetPt, const double jetEta) {
    double Pt = jetPt;
    // for scale factor extrapolation
    if(Pt<20.) Pt = 20.;
    if(Pt>800.) Pt = 800.;

    return CSVM_SFb_0to2p4->Eval(Pt) + 2*SFbShift_*CSVM_SFb_errors->GetBinContent(CSVM_SFb_errors->GetXaxis()->FindBin(jetPt));
  }

  // ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for light flavor jets and CSVM tagger ------------
  double BTagValidation::scaleFactorUDSG_CSVM(const double jetPt, const double jetEta) {
    double SF = 1.;
    double Pt = jetPt;
    double absEta = fabs(jetEta);
    // for scale factor extrapolation
    if(Pt<20.) Pt = 20.;

    if(absEta<0.8) {
      if(Pt>1000.) Pt = 1000.;

      SF = CSVM_SFl_0to0p8->Eval(Pt) + ( (jetPt<20. || jetPt>1000.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVM_SFl_0to0p8_max->Eval(Pt) - CSVM_SFl_0to0p8->Eval(Pt)) : (CSVM_SFl_0to0p8_min->Eval(Pt) - CSVM_SFl_0to0p8->Eval(Pt)) );
    }
    else if(absEta>=0.8 && absEta<1.6) {
      if(Pt>1000.) Pt = 1000.;

      SF = CSVM_SFl_0p8to1p6->Eval(Pt) + ( (jetPt<20. || jetPt>1000.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVM_SFl_0p8to1p6_max->Eval(Pt) - CSVM_SFl_0p8to1p6->Eval(Pt)) : (CSVM_SFl_0p8to1p6_min->Eval(Pt) - CSVM_SFl_0p8to1p6->Eval(Pt)) );
    }
    else {
      if(Pt>850.) Pt = 850.;

      SF = CSVM_SFl_1p6to2p4->Eval(Pt) + ( (jetPt<20. || jetPt>850.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVM_SFl_1p6to2p4_max->Eval(Pt) - CSVM_SFl_1p6to2p4->Eval(Pt)) : (CSVM_SFl_1p6to2p4_min->Eval(Pt) - CSVM_SFl_1p6to2p4->Eval(Pt)) );
    }

    return SF;
  }

  // ---- Method returns MC event weight for for reweighting to the NPV distribution in the data: substitute for official PU reweighting ----
  double BTagValidation::GetLumiWeightsPVBased (const std::string file, const std::string hist, const int npv) { 
    double wtPU(1) ;
    TFile* f = new TFile(file.c_str()) ;
    TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) ); 
    wtPU = npv > 0 && npv <= 100 ? hwt->GetBinContent(npv) : 1.;
    f->Close() ; 
    delete f ;
    delete hwt ; 
    return wtPU ;
  }

  // ----For calculating MC event weight for reweighting to the jetPt distribution in the data (or signal, depending)
  double BTagValidation::GetWeightsJetPtBased (const std::string file, const std::string hist, const double jetpt) {
    double wtPt(1) ;
    TFile* f = new TFile(file.c_str()) ;
    TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) );
    wtPt = jetpt > 0 && jetpt <= 5000 ? hwt->GetBinContent(hwt->GetXaxis()->FindBin(jetpt)) : 1.;
    f->Close() ;
    delete f ;
    delete hwt ;
    return wtPt ;
  }

  // ----For calculating MC event weight for reweighting to the ntracks distribution in data
  double BTagValidation::GetWeightsNtracksBased (const std::string file, const std::string hist, const double jetpt) {
    double wtPt(1) ;
    TFile* f = new TFile(file.c_str()) ;
    TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) );
    wtPt = jetpt > -1 && jetpt <= 90 ? hwt->GetBinContent(hwt->GetXaxis()->FindBin(jetpt)) : 1.;
    f->Close() ;
    delete f ;
    delete hwt ;
    return wtPt ;
  }

  // ----For calculating MC event weight for reweighting to the subjetPtBalance distribution in the data
  double BTagValidation::GetWeightsSubJetPtBalanceBased (const std::string file, const std::string hist, const double jetptbalance) {
    double wtPtbalance(1) ;
    TFile* f = new TFile(file.c_str()) ;
    TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) );
    wtPtbalance = jetptbalance >= 0 && jetptbalance <= 1 ? hwt->GetBinContent(hwt->GetXaxis()->FindBin(jetptbalance)) : 1.;
    f->Close() ;
    delete f ;
    delete hwt ;
    return wtPtbalance ;
  }

  // ----For calculating bkg MC event weight for reweighting to the mass distribution in signal MC
  double BTagValidation::GetWeightsMassSoftDropBased (const std::string file, const std::string hist, const double jetmass) {
    double wtMass(1) ;
    TFile* f = new TFile(file.c_str()) ;
    TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) );
    wtMass = jetmass >= 0 && jetmass <= 400 ? hwt->GetBinContent(hwt->GetXaxis()->FindBin(jetmass)) : 1.;
    f->Close() ;
    delete f ;
    delete hwt ;
    return wtMass ;
  }


  // ----For calculating bkg MC event weight for reweighting to the jetNTracks distribution in signal MC
  double BTagValidation::GetWeightsJetNTracksBased (const std::string file, const std::string hist, const double jetNTracks) {
    double wtNTracks(1) ;
    TFile* f = new TFile(file.c_str()) ;
    TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) );
    wtNTracks = jetNTracks >= 0 && jetNTracks <= 40 ? hwt->GetBinContent(hwt->GetXaxis()->FindBin(jetNTracks)) : 1.;
    f->Close() ;
    delete f ;
    delete hwt ;
    return wtNTracks ;
  }

  // ----For calculating bkg MC event weight for reweighting to the IPSig1stAboveB distribution in signal MC
  double BTagValidation::GetWeightsIPSig1stAboveBBased (const std::string file, const std::string hist, const double IPSig) {
    double wtIPSig(1) ;
    TFile* f = new TFile(file.c_str()) ;
    TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) );
    wtIPSig = IPSig >= -20 && IPSig <= 20 ? hwt->GetBinContent(hwt->GetXaxis()->FindBin(IPSig)) : 1.;
    f->Close() ;
    delete f ;
    delete hwt ;
    return wtIPSig ;
  }

  // ----For calculating bkg MC event weight for reweighting to the Zratio distribution in signal MC
  double BTagValidation::GetWeightsZratioBased (const std::string file, const std::string hist, const double ratio) {
    double wtRatio(1) ;
    TFile* f = new TFile(file.c_str()) ;
    TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) );
    wtRatio = ratio >= 0. && ratio <= 60 ? hwt->GetBinContent(hwt->GetXaxis()->FindBin(ratio)) : 1.;
    f->Close() ;
    delete f ;
    delete hwt ;
    return wtRatio ;
  }


  // ----For calculating MC event weight for reweighting to the subjetPtBalance distribution in the data
  double BTagValidation::GetWeightsSV1EnergyRatioBased (const std::string file, const std::string hist, const double ratio) {
    double wtRatio(1) ;
    TFile* f = new TFile(file.c_str()) ;
    TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) );
    wtRatio = ratio >= -1 && ratio <= 4 ? hwt->GetBinContent(hwt->GetXaxis()->FindBin(ratio)) : 1.;
    f->Close() ;
    delete f ;
    delete hwt ;
    return wtRatio ;
  }

  void BTagValidation::ApplyJES(TLorentzVector jetp4, double jetarea, double jetrho, double jes, int npv, double& newjec) {
    newjec = 1.0; 
    TLorentzVector uncorrJetP4 = jetp4;
    uncorrJetP4 *= jes != 0. ? 1./jes : 1. ; 
    ptr_newJEC_->setJetPt ( uncorrJetP4.Pt()     );
    ptr_newJEC_->setJetEta( uncorrJetP4.Eta()    );
    ptr_newJEC_->setJetE  ( uncorrJetP4.Energy() );
    ptr_newJEC_->setJetA  ( jetarea );
    ptr_newJEC_->setRho   ( jetrho ) ;  
    ptr_newJEC_->setNPV   ( npv ) ; 
    newjec = ptr_newJEC_->getCorrection();
  }

  double BTagValidation::GetJESUncert( int jecShift, double jetpt, double jeteta, double& jesunc ) {
    ptr_jecUnc_->setJetPt ( jetpt );
    ptr_jecUnc_->setJetEta( jeteta);
    jesunc = ptr_jecUnc_->getUncertainty(true);
    if(DEBUG_ && DEBUGlevel_>=1)std::cout<<"jesunc = " << jesunc << std::endl;
    if(DEBUG_ && DEBUGlevel_>=1)std::cout<<"jecShift = " << jecShift << std::endl;
    jesunc *= jecShift ; 
    if(DEBUG_ && DEBUGlevel_>=1)std::cout<<"jesunc (after) = " << jesunc << std::endl;
    return jesunc;
  }

  //define this as a plug-in
  DEFINE_FWK_MODULE(BTagValidation);
