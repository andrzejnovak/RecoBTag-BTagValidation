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
// Original Author:  Devdatta Majumder,13 2-054,+41227671675,
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

#include "RecoBTag/PerformanceMeasurements/interface/VariableParser.h"
#include "RecoBTag/PerformanceMeasurements/interface/JetInfoBranches.h"
#include "RecoBTag/PerformanceMeasurements/interface/EventInfoBranches.h"

#include <TString.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TF1.h>

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <utility>

#include <boost/algorithm/string.hpp>

//
// class declaration
//

class BTagValidation : public edm::EDAnalyzer {
  public:
    explicit BTagValidation(const edm::ParameterSet&);
    ~BTagValidation();

  private:
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    bool passTrigger() ;
    bool passMuonSelection(const int muIdx, const JetInfoBranches& JetInfo, const int iJet, const double deltaR=0.4);

    //// Manage histograms
    void createJetHistos(const TString& histoTag);
    void AddHisto(const TString& name, const TString& title, const int nbins, const double min, const double max);
    void AddHisto2D(const TString& name, const TString& title, const int nbins, const double min, const double max, const int nbins2, const double min2, const double max2);

    template <class Type>
      void FillHisto(const TString& name, const int flavour, const bool isGSPbb, const bool isGSPcc ,const Type value, const double weight);

    template <class Type1, class Type2>
      void FillHisto2D(const TString& name, const int flavour, const bool isGSPbb, const bool isGSPcc, const Type1 value, const Type2 value2, const double weight);

    //// Fill jet histograms
    void fillJetHistos(const JetInfoBranches& JetInfo, const int iJet, const bool isGSPbb, const bool isGSPcc, const TString& histoTag, const int nmu, const int nselmuon, const int idxFirstMuon, const double wt);

    double GetEvtWtPV (const std::string file, const std::string hist, const int npv) ; 
    double GetEvtWtJetPt (const std::string file, const std::string hist, const double jetpt) ;

    void ApplyJES(TLorentzVector jetp4, double jetarea, double jetrho, double jes, int npv, double& newjec) ; 
    void GetJESUncert( double jetpt, double jeteta, double& jesunc ) ; 

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

    //// Lumi reweighting object
    edm::LumiReWeighting LumiWeights_;

    //// Configurables
    const int                       maxEvents_;
    const int                       reportEvery_;
    const bool                      useJetProbaTree_;
    const std::string               inputTTreeEvtInfo_;
    const std::string               inputTTree_;
    const std::vector<std::string>  inputFiles_;
    const bool                      usePrunedSubjets_ ;
    const bool                      useSoftDropSubjets_ ;
    const bool                      useFlavorCategories_;
    const bool                      useRelaxedMuonID_;
    bool                            runEventInfo_;
    bool                            runJetVariables_;
    const bool                      applyFatJetMuonTagging_;
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
    const double                    fatJetPtMin_;
    const double                    fatJetPtMax_;
    const double                    fatJetAbsEtaMax_;
    const double                    fatJetSoftDropMassMin_;
    const double                    fatJetSoftDropMassMax_;
    const double		                fatJetTau21Min_;
    const double		                fatJetTau21Max_;
    const double                    SFbShift_;
    const double                    SFlShift_;
    const double                    MuonJetPtRatio_;
    const std::vector<std::string>  triggerSelection_;
    const std::vector<std::string>  triggerPathNames_;
    const std::string               file_PVWt_ ; 
    const std::string               file_PUDistMC_ ;
    const std::string               file_PUDistData_ ;
    const std::string               file_FatJetPtWt_ ;
    const std::string               file_SubJetPtWt_ ;
    const std::string               hist_PVWt_ ; 
    const std::string               hist_PUDistMC_ ;
    const std::string               hist_PUDistData_ ;
    const std::string               hist_FatJetPtWt_ ;
    const std::string               hist_SubJetPtWt_ ;
    const bool                      doPUReweightingOfficial_ ;
    const bool                      doPUReweightingNPV_ ;
    const bool                      doFatJetPtReweighting_ ;
    const bool                      doSubJetPtReweighting_ ;
    const bool                      doGSPUp_;
    const bool                      doGSPDown_;
    const bool                      doBFrag_;
    const bool                      doBFragUp_; 
    const bool                      doBFragDown_;
    const bool                      doCDFrag_;
    const bool                      doCFrag_;
    const bool                      doK0L_;
    const bool                      removeJP0_;
    const bool                      applySFs_;
    const std::string               btagCSVFile_ ; 
    const int                       btagOperatingPoint_ ; 
    const std::string               btagMeasurementType_ ; 
    const std::string               btagSFType_  ; 
    const std::vector<std::string>  newJECPayloadNames_;
    const std::string               jecUncPayloadName_;
    const bool                      doNewJEC_;
    const bool                      doJECUncert_;
    const int                       jecshift_ ;
    const bool                      useRunRange_;
    const int                       runRangeMin_;
    const int                       runRangeMax_;

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

    static const std::vector<std::string> taggers_ ;
    static const std::map<std::string, std::pair<int, int>> pts_ ;
    static const std::vector<std::string> sels_ ;

    static const double CSVv2L_;
    static const double CSVv2M_;
    static const double DeepCSVL_;
    static const double DeepCSVM_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
const std::vector<std::string> BTagValidation::taggers_ = {"SV", "CSVv2L", "CSVv2M", "DeepCSVL", "DeepCSVM", "CSVv2L_SV", "CSVv2M_SV", "DeepCSVL_SV", "DeepCSVM_SV"};
const std::map<std::string, std::pair<int, int>> BTagValidation::pts_ = {
  {"pt0"  , std::make_pair(0  , 30   ) } , 
  {"pt1"  , std::make_pair(30 , 140  ) } , 
  {"pt2"  , std::make_pair(140, 180  ) } , 
  {"pt3"  , std::make_pair(180, 240  ) } , 
  {"pt4"  , std::make_pair(240, 10000) } , 
} ; 
const std::vector<std::string> BTagValidation::sels_ = {"pass", "fail"}; 

const double BTagValidation::CSVv2L_   = 0.5803; 
const double BTagValidation::CSVv2M_   = 0.8838; 
const double BTagValidation::DeepCSVL_ = 0.1522; 
const double BTagValidation::DeepCSVM_ = 0.4941; 

//
// constructors and destructor
//
BTagValidation::BTagValidation(const edm::ParameterSet& iConfig) :
  maxEvents_(iConfig.getParameter<int>("MaxEvents")),
  reportEvery_(iConfig.getParameter<int>("ReportEvery")),
  useJetProbaTree_(iConfig.getParameter<bool>("UseJetProbaTree")),
  inputTTreeEvtInfo_(iConfig.getParameter<std::string>("InputTTreeEvtInfo")),
  inputTTree_(iConfig.getParameter<std::string>("InputTTree")),
  inputFiles_(iConfig.getParameter<std::vector<std::string> >("InputFiles")),
  usePrunedSubjets_(iConfig.getParameter<bool>("UsePrunedSubjets")),
  useSoftDropSubjets_(iConfig.getParameter<bool>("UseSoftDropSubjets")), 
  useFlavorCategories_(iConfig.getParameter<bool>("UseFlavorCategories")),
  useRelaxedMuonID_(iConfig.getParameter<bool>("UseRelaxedMuonID")),
  runEventInfo_(iConfig.getParameter<bool>("runEventInfo")),
  runJetVariables_(iConfig.getParameter<bool>("runJetVariables")),
  applyFatJetMuonTagging_(iConfig.getParameter<bool>("ApplyFatJetMuonTagging")),
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
  fatJetPtMin_(iConfig.getParameter<double>("FatJetPtMin")),
  fatJetPtMax_(iConfig.getParameter<double>("FatJetPtMax")),
  fatJetAbsEtaMax_(iConfig.getParameter<double>("FatJetAbsEtaMax")),
  fatJetSoftDropMassMin_(iConfig.getParameter<double>("FatJetSoftDropMassMin")),
  fatJetSoftDropMassMax_(iConfig.getParameter<double>("FatJetSoftDropMassMax")),
  fatJetTau21Min_(iConfig.getParameter<double>("FatJetTau21Min")),
  fatJetTau21Max_(iConfig.getParameter<double>("FatJetTau21Max")),
  SFbShift_(iConfig.getParameter<double>("SFbShift")),
  SFlShift_(iConfig.getParameter<double>("SFlShift")),
  MuonJetPtRatio_(iConfig.getParameter<double>("MuonJetPtRatio")),
  triggerSelection_(iConfig.getParameter<std::vector<std::string> >("TriggerSelection")),
  triggerPathNames_(iConfig.getParameter<std::vector<std::string> >("TriggerPathNames")),
  file_PVWt_(iConfig.getParameter<std::string>("File_PVWt")),
  file_PUDistMC_(iConfig.getParameter<std::string>("File_PUDistMC")),
  file_PUDistData_(iConfig.getParameter<std::string>("File_PUDistData")),
  file_FatJetPtWt_(iConfig.getParameter<std::string>("File_FatJetPtWt")),
  file_SubJetPtWt_(iConfig.getParameter<std::string>("File_SubJetPtWt")),
  hist_PVWt_(iConfig.getParameter<std::string>("Hist_PVWt")),
  hist_PUDistMC_(iConfig.getParameter<std::string>("Hist_PUDistMC")),
  hist_PUDistData_(iConfig.getParameter<std::string>("Hist_PUDistData")),
  hist_FatJetPtWt_(iConfig.getParameter<std::string>("Hist_FatJetPtWt")),
  hist_SubJetPtWt_(iConfig.getParameter<std::string>("Hist_SubJetPtWt")),
  doPUReweightingOfficial_(iConfig.getParameter<bool>("DoPUReweightingOfficial")),
  doPUReweightingNPV_(iConfig.getParameter<bool>("DoPUReweightingNPV")),
  doFatJetPtReweighting_(iConfig.getParameter<bool>("DoFatJetPtReweighting")),
  doSubJetPtReweighting_(iConfig.getParameter<bool>("DoSubJetPtReweighting")),
  doGSPUp_(iConfig.getParameter<bool>("DoGSPUp")),
  doGSPDown_(iConfig.getParameter<bool>("DoGSPDown")),
  doBFrag_(iConfig.getParameter<bool>("DoBFrag")),
  doBFragUp_(iConfig.getParameter<bool>("DoBFragUp")),
  doBFragDown_(iConfig.getParameter<bool>("DoBFragDown")),
  doCDFrag_(iConfig.getParameter<bool>("DoCDFrag")),
  doCFrag_(iConfig.getParameter<bool>("DoCFrag")),
  doK0L_(iConfig.getParameter<bool>("DoK0L")),
  removeJP0_(iConfig.getParameter<bool>("RemoveJP0")),
  applySFs_(iConfig.getParameter<bool>("ApplySFs")),
  btagCSVFile_(iConfig.getParameter<std::string>("btagCSVFile")), 
  btagOperatingPoint_(iConfig.getParameter<int>("btagOperatingPoint")), 
  btagMeasurementType_(iConfig.getParameter<std::string>("btagMeasurementType")), 
  btagSFType_(iConfig.getParameter<std::string>("btagSFType")),
  newJECPayloadNames_(iConfig.getParameter<std::vector<std::string>>("newJECPayloadNames")),
  jecUncPayloadName_(iConfig.getParameter<std::string>("jecUncPayloadName")),
  doNewJEC_(iConfig.getParameter<bool>("doNewJEC")), 
  doJECUncert_(iConfig.getParameter<bool>("doJECUncert")), 
  jecshift_(iConfig.getParameter<int>("jecshift")),
  useRunRange_(iConfig.getParameter<bool>("useRunRange")), 
  runRangeMin_(iConfig.getParameter<int>("runRangeMin")), 
  runRangeMax_(iConfig.getParameter<int>("runRangeMax")), 
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

  if ( doNewJEC_ && newJECPayloadNames_.size() > 0 ) {
    std::vector<JetCorrectorParameters> vPar;  
    for ( std::vector<std::string>::const_iterator it = newJECPayloadNames_.begin(); it != newJECPayloadNames_.end(); ++it) {
      JetCorrectorParameters pars(*it);
      vPar.push_back(pars) ; 
    }
    ptr_newJEC_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) ); 

  }

  if ( doJECUncert_ ) ptr_jecUnc_ = boost::shared_ptr<JetCorrectionUncertainty>(new JetCorrectionUncertainty(jecUncPayloadName_) ) ; 

  if (doPUReweightingOfficial_) LumiWeights_ = edm::LumiReWeighting(file_PUDistMC_, file_PUDistData_, hist_PUDistMC_, hist_PUDistData_) ;

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
    JetTreeEvtInfo->Add(inputFiles_.at(i).c_str());
    JetTree->Add(inputFiles_.at(i).c_str());

    TFile *f = TFile::Open(inputFiles_.at(i).c_str(),"READ");
    if (f->IsOpen() == 0) continue;

    TH1D *hEventCountAll    = (TH1D*)f->Get("allEvents/hEventCount");
    TH1D *hEventCountStored = (TH1D*)f->Get("selectedEvents/hEventCount");

    nEventsAll+=hEventCountAll->GetBinContent(1);
    nEventsStored+=hEventCountStored->GetBinContent(1);

    f->Close();
  }

  variables = variableParser.parseGroupsAndVariables(groupSet, variableSet);
  variableParser.saveStoredVariablesToFile();

  EvtInfo.ReadBranches(JetTree, variableParser);

  FatJetInfo.ReadBranches(JetTree, variableParser, "FatJetInfo");
  SubJetInfo.ReadBranches(JetTree, variableParser, "FatJetInfo", "SoftDropPuppi");

  SubJets.ReadBranches(JetTree, variableParser, "SoftDropPuppiSubJetInfo") ;

  h1_CutFlow        = fs->make<TH1D>("h1_CutFlow",       "h1_CutFlow",        4,-0.5,3.5);
  h1_CutFlow->Sumw2();
  h1_CutFlow->SetDefaultSumw2(kTRUE); 
  h1_CutFlow_unw    = fs->make<TH1D>("h1_CutFlow_unw",   "h1_CutFlow_unw",    4,-0.5,3.5);
  h1_nPUtrue_mc     = fs->make<TH1D>("h1_nPUtrue_mc",    ";N(true PU in MC);;",     100,0.,100.);
  h1_nPUtrue_mc_unw = fs->make<TH1D>("h1_nPUtrue_mc_unw",";N(true PU in MC, unweighted);;", 100,0.,100.);
  h1_nPV_data       = fs->make<TH1D>("h1_nPV_data",      ";N(PV in data);;",       100,0.,100.);
  h1_nPV_mc         = fs->make<TH1D>("h1_nPV_mc",        ";N(PV in MC);;",         100,0.,100.);
  h1_nPV_mc_unw     = fs->make<TH1D>("h1_nPV_mc_unw",    ";N(PV in MC, unweighted)",     100,0.,100.);
  h1_pt_hat         = fs->make<TH1D>("h1_pt_hat",        ";#hat{p}_{T} before selection;;",         1400,0,7000);
  h1_pt_hat_sel     = fs->make<TH1D>("h1_pt_hat_sel",    ";#hat{p}_{T} after selection;;",         1400,0,7000);

  createJetHistos("FatJet");
  createJetHistos("SoftDropSubJet");

}

void BTagValidation::createJetHistos(const TString& histoTag) {

  double PtMax = 5000.;
  double pi=TMath::Pi();

  AddHisto(histoTag+"_nJets"            ,";N(jets);;",20,0,20);

  AddHisto(histoTag+"_pt_all"           ,";p_{T}(all jets) [GeV];;"       ,PtMax/10  ,0      ,PtMax );
  AddHisto(histoTag+"_eta"              ,";#eta(all jets);;"              ,50        ,-2.5   ,2.5);
  AddHisto(histoTag+"_phi"              ,";#phi(all jets);;"              ,40        ,-1.*pi ,pi);
  AddHisto(histoTag+"_mass"             ,";M(all jets) [GeV];;"           ,200       ,0      ,400);
  AddHisto(histoTag+"_pt_sv"            ,";p_{T}(jets containing a SV) [GeV];;" ,PtMax/10  ,0      ,PtMax);

  AddHisto(histoTag+"_muon_multi"    ,";N(#mu);;",7,-0.5,6.5);
  AddHisto(histoTag+"_muon_multi_sel",";N(selected #mu);;",7,-0.5,6.5);

  AddHisto(histoTag+"_track_multi"   ,";number of tracks in the jets;;",80,-0.5,79.5);
  AddHisto(histoTag+"_trk_multi_sel" ,";number of selected tracks in the jets;;",80,-0.5,79.5);
  AddHisto(histoTag+"_track_IPs"     ,";3D IP significance of all tracks;;",      200,-50.,50.);
  AddHisto(histoTag+"_track_dist"    ,";distance to the jet axis;;",                 100,0.,0.08  );
  AddHisto(histoTag+"_track_len"     ,";track decay length;;",                       100,0,5.     );
  AddHisto(histoTag+"_track_nHit"   ,";number of hits ;;",                      35,-0.5,34.5);
  AddHisto(histoTag+"_track_HPix"   ,";number of hits in the Pixel;;",          10,-0.5,9.5);
  AddHisto(histoTag+"_track_pt"      ,";pT of all the tracks;;",                     200,0.,1000. );

  AddHisto(histoTag+"_sv_multi_0"    ,";N(SV);;",6,-0.5,5.5);
  AddHisto(histoTag+"_sv_flightSig2D",     ";Flight distance significance 2D;;",150,0.,150.);
  AddHisto(histoTag+"_TagVarCSV_sv_mass",  ";M(SV from TagVarCSV) [GeV];;",750,0.,15.);
  AddHisto(histoTag+"_svnTrk",             ";N(tracks) : SVnVertexTracks (centered);;",   15,-0.5,14.5 );
  AddHisto(histoTag+"_sv_en_ratio",        ";SV energy ratio;;",60,0.,1.2);
  AddHisto(histoTag+"_sv_deltaR_jet",      ";#DeltaR(SV, jet);;",50,0.,0.5);


  AddHisto(histoTag+"_JP"      ,";JP;;",50,0.,2.5);
  AddHisto(histoTag+"_JBP"     ,";JBP;;",50,0.,8.);
  AddHisto(histoTag+"_CSVIVFv2",";CSVIVFv2;;",50,0.,1.);
  AddHisto(histoTag+"_DeepCSV" ,";DeepCSV;;",50,0.,1.);
  AddHisto(histoTag+"_cMVAv2"  ,";cMVAv2;;",50,0.,1.);
  AddHisto(histoTag+"_DoubleB" ,";DoubleB;;",100,-1,1.);
  AddHisto(histoTag+"_deepDoubleBvLQCD", "Jet_deepDoubleBvLQCD", 100, -1, 1 );
  AddHisto(histoTag+"_deepDoubleBvLHbb", "Jet_deepDoubleBvLHbb", 100, -1, 1 );
  AddHisto(histoTag+"_deepDoubleCvLQCD", "Jet_deepDoubleCvLQCD", 100, -1, 1 );
  AddHisto(histoTag+"_deepDoubleCvLHcc", "Jet_deepDoubleCvLHcc", 100, -1, 1 );
  AddHisto(histoTag+"_deepDoubleCvBHcc", "Jet_deepDoubleCvBHcc", 100, -1, 1 );
  AddHisto(histoTag+"_deepDoubleCvBHbb", "Jet_deepDoubleCvBHbb", 100, -1, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleBvLQCD", "Jet_massIndDeepDoubleBvLQCD", 100, -1, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleBvLHbb", "Jet_massIndDeepDoubleBvLHbb", 100, -1, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleCvLQCD", "Jet_massIndDeepDoubleCvLQCD", 100, -1, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleCvLHcc", "Jet_massIndDeepDoubleCvLHcc", 100, -1, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleCvBHcc", "Jet_massIndDeepDoubleCvBHcc", 100, -1, 1 );
  AddHisto(histoTag+"_massIndDeepDoubleCvBHbb", "Jet_massIndDeepDoubleCvBHbb", 100, -1, 1 );
  AddHisto(histoTag+"_deepBoostedJetbbvsLight", "Jet_deepBoostedJetbbvsLight", 100, -1, 1 );
  AddHisto(histoTag+"_deepBoostedJetccvsLight", "Jet_deepBoostedJetccvsLight", 100, -1, 1 );
  AddHisto(histoTag+"_deepBoostedJetTvsQCD", "Jet_deepBoostedJetTvsQCD", 100, -1, 1 );
  AddHisto(histoTag+"_deepBoostedJetZHccvsQCD", "Jet_deepBoostedJetZHccvsQCD", 100, -1, 1 );
  AddHisto(histoTag+"_deepBoostedJetWvsQCD", "Jet_deepBoostedJetWvsQCD", 100, -1, 1 );
  AddHisto(histoTag+"_deepBoostedJetZHbbvsQCD", "Jet_deepBoostedJetZHbbvsQCD", 100, -1, 1 );

  if ( histoTag.Contains("SubJet") ) { 

    AddHisto(histoTag+"_JP_all_pt0", ";JP_all_pt0;;", 50, 0., 2.5);
    AddHisto(histoTag+"_JP_all_pt1", ";JP_all_pt1;;", 50, 0., 2.5);
    AddHisto(histoTag+"_JP_all_pt2", ";JP_all_pt2;;", 50, 0., 2.5);
    AddHisto(histoTag+"_JP_all_pt3", ";JP_all_pt3;;", 50, 0., 2.5);
    AddHisto(histoTag+"_JP_all_pt4", ";JP_all_pt4;;", 50, 0., 2.5);

    AddHisto(histoTag+"_SVmass_all_pt0", ";SVmass_all_pt0;;", 75, 0., 15.);
    AddHisto(histoTag+"_SVmass_all_pt1", ";SVmass_all_pt1;;", 75, 0., 15.);
    AddHisto(histoTag+"_SVmass_all_pt2", ";SVmass_all_pt2;;", 75, 0., 15.);
    AddHisto(histoTag+"_SVmass_all_pt3", ";SVmass_all_pt3;;", 75, 0., 15.);
    AddHisto(histoTag+"_SVmass_all_pt4", ";SVmass_all_pt4;;", 75, 0., 15.);

    for (std::string tagger : taggers_) {
      for (auto const pt : pts_) {
        for (std::string sel : sels_) {
          std::stringstream hname ;
          hname << histoTag << "_JP_" << tagger << sel << "_" << pt.first ; 
          AddHisto((hname.str()).c_str(), "; JP; Events;", 50, 0., 2.5);

          if (tagger.compare(std::string("SV")) == 0) continue;

          hname.str(std::string());
          hname << histoTag << "_SVmass_" << tagger << sel << "_" << pt.first ; 
          AddHisto((hname.str()).c_str(), "; SVmass; Events;", 75, 0., 15.);
        }
      }
    }

  }

}

// ------------ method called for each event  ------------
void BTagValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;

  if(JetTree == 0 && JetTreeEvtInfo == 0) return;

  Long64_t nEntries = JetTree->GetEntries();
  if(maxEvents_>=0) nEntries = maxEvents_;

  double BTemplateCorrections[100][20][2];

  if (doBFrag_) {
    int nPtRelPtBins(15);

    TString PtRelPtBin[] = {
      "Pt2030", "Pt3040", "Pt4050", "Pt5060", "Pt6070"
        , "Pt7080", "Pt80100", "Pt100120", "Pt120160", "Pt160210"
        , "Pt210260", "Pt260320", "Pt320400", "Pt400500", "Pt500" 
    };

    for (int ptb = 0; ptb < nPtRelPtBins; ptb++) {      
      for (int ib = 0; ib < 100; ib++) {
        BTemplateCorrections[ib][ptb][0] = 1.;
        BTemplateCorrections[ib][ptb][1] = 1.;
      }
      std::ifstream MnusCorrectionsFile;      
      MnusCorrectionsFile.open("/afs/cern.ch/work/d/devdatta/CMSREL/BTagging/CMSSW_9_4_1/src/RecoBTag/BTagValidation/test/PtRelFall12/EnergyFraction_" + PtRelPtBin[ptb] + "_m5.txt");
      while( MnusCorrectionsFile ) {
        double xBin, efcorr;
        MnusCorrectionsFile >> xBin >> efcorr;
        if ( efcorr > 4. ) efcorr = 1.;
        int ib = int(xBin/0.02);
        BTemplateCorrections[ib][ptb][0] = efcorr;
      }

      std::ifstream PlusCorrectionsFile; 
      PlusCorrectionsFile.open("/afs/cern.ch/work/d/devdatta/CMSREL/BTagging/CMSSW_9_4_1/src/RecoBTag/BTagValidation/test/PtRelFall12/EnergyFraction_" + PtRelPtBin[ptb] + "_p5.txt");
      while( PlusCorrectionsFile ) {
        double xBin, efcorr;
        PlusCorrectionsFile >> xBin >> efcorr;
        if ( efcorr > 4. ) efcorr = 1.;
        int ib = int(xBin/0.02);
        BTemplateCorrections[ib][ptb][1] = efcorr;
      }
    } //// Loop ovr PtRelBins
  } //// doBFrag 

  //---------------------------- Start event loop ---------------------------------------//
  for(Long64_t iEntry = 0; iEntry < nEntries; ++iEntry) {
    JetTreeEvtInfo->GetEntry(iEntry);

    JetTree->GetEntry(iEntry);

    int run = EvtInfo.Run ; 
    int lumi = EvtInfo.LumiBlock ; 
    if((iEntry%reportEvery_) == 0) 
      edm::LogInfo("EventNumber") << ">>>>> Processing event with run no. " 
        << run << " and lumisection " << lumi << iEntry << " of " << nEntries;

    if(run < 0) {
      isData = false;
      if ( iEntry == 0) edm::LogInfo("IsMC") << ">>>>> Running on simulation\n" ;
    }
    else if( iEntry == 0 ) edm::LogInfo("IsData") << ">>>>> Running on data\n" ;

    if(isData && useRunRange_){
      if( run < runRangeMin_ || run > runRangeMax_ ){
        continue;
      }
    }

    double wtPU = 1.;
    if ( doPUReweightingOfficial_ && !isData )
      wtPU *= LumiWeights_.weight(EvtInfo.nPUtrue);
    else if ( doPUReweightingNPV_ && !isData ) 
      wtPU *= GetEvtWtPV(file_PVWt_, hist_PVWt_, EvtInfo.nPV) ;  

    h1_CutFlow->Fill(2.,wtPU); //// count all events
    h1_CutFlow_unw->Fill(2.);

    if( !isData ) h1_pt_hat->Fill(EvtInfo.pthat,wtPU);

    if( !passTrigger() ) {
      continue; //// apply trigger selection
    }

    h1_CutFlow->Fill(3.,wtPU); //// count events passing trigger selection
    h1_CutFlow_unw->Fill(3.);

    if( isData )
      h1_nPV_data->Fill(EvtInfo.nPV);
    else {
      h1_nPUtrue_mc    ->Fill(EvtInfo.nPUtrue,wtPU);
      h1_nPUtrue_mc_unw->Fill(EvtInfo.nPUtrue);
      h1_nPV_mc        ->Fill(EvtInfo.nPV,wtPU);
      h1_nPV_mc_unw    ->Fill(EvtInfo.nPV);
    }

    if(FatJetInfo.nJet <= 0) continue; //// require at least 1 fat jet in the event

    int nFatJet(0);
    int nSubJet(0);

    //---------------------------- Start fat jet loop ---------------------------------------//
    for (int iJet = 0; iJet < FatJetInfo.nJet; ++iJet) {

      //double jetptuncorr(FatJetInfo.Jet_uncorrpt [iJet]) ;
      double jetpt(FatJetInfo.Jet_pt [iJet]) ;
      double jetabseta(fabs(FatJetInfo.Jet_eta[iJet])) ;
      double jetmass(FatJetInfo.Jet_massSoftDrop[iJet]);

      double jesunc(0);
      if ( doJECUncert_ ) 
        GetJESUncert( jetpt, jetabseta, jesunc  ) ; 
      jetpt *= 1 + jesunc;

      if ( jetpt < fatJetPtMin_ ||
          jetpt >= fatJetPtMax_ )            continue; //// apply jet pT cut
      if ( jetabseta > fatJetAbsEtaMax_ )     continue; //// apply jet eta cut
      /////////////////////if ( FatJetInfo.Jet_tightID[iJet]==0 )  continue; //// apply tight jet ID
      if ( jetmass < fatJetSoftDropMassMin_ ||
          jetmass > fatJetSoftDropMassMax_ )   continue; //// apply soft drop jet mass cut

      double tau1(FatJetInfo.Jet_tau1[iJet]);
      double tau2(FatJetInfo.Jet_tau2[iJet]);
      double tau21(tau1!=0 ? tau2/tau1 : 0.);

      if ( tau21 > fatJetTau21Max_ || tau21 < fatJetTau21Min_ ) continue ; ////apply jet substructure tau21 cut.

      TLorentzVector jet_p4;
      jet_p4.SetPtEtaPhiM(FatJetInfo.Jet_pt[iJet], FatJetInfo.Jet_eta[iJet], FatJetInfo.Jet_phi[iJet], FatJetInfo.Jet_mass[iJet]);

      /////////////////////if ( SubJetInfo.Jet_nSubJets[iJet] != 2 ) continue ;

      int iSubJet1(SubJetInfo.SubJetIdx[SubJetInfo.Jet_nFirstSJ[iJet]]) ; ////added by rizki
      int iSubJet2(SubJetInfo.SubJetIdx[SubJetInfo.Jet_nFirstSJ[iJet]+1]) ; ////added by rizki

      //// If  processing subjets, discard fat jet with any one subjet having pt = 0
      //////////////////////if( (usePrunedSubjets_ || useSoftDropSubjets_)
      //////////////////////    && (SubJets.Jet_pt[iSubJet1]==0. || SubJets.Jet_pt[iSubJet2]==0.) ) continue;

      TLorentzVector subjet1_p4, subjet2_p4;
      subjet1_p4.SetPtEtaPhiM(SubJets.Jet_pt[iSubJet1], SubJets.Jet_eta[iSubJet1], SubJets.Jet_phi[iSubJet1], SubJets.Jet_mass[iSubJet1]);
      subjet2_p4.SetPtEtaPhiM(SubJets.Jet_pt[iSubJet2], SubJets.Jet_eta[iSubJet2], SubJets.Jet_phi[iSubJet2], SubJets.Jet_mass[iSubJet2]);

      double subjet_dR(subjet1_p4.DeltaR(subjet2_p4));

      //// If processing subjets, skip infrared unsafe configurations
      //////////////////////if( (usePrunedSubjets_ || useSoftDropSubjets_)
      //////////////////////    && subjet_dR < (FatJetInfo.Jet_mass[iJet]/FatJetInfo.Jet_pt[iJet]) ) continue;

      int idxFirstMuon(-1);
      int nselmuon(0);
      int nmu(0);

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

      if( applyFatJetMuonTagging_ && nselmuon==0)    continue;

      if( applyFatJetBTagging_ ) { //// if enabled, select b-tagged fat jets
        if( fatJetDoubleBTagging_ && 
            !( SubJets.Jet_CombIVF[iSubJet1]>subJetBDiscrMin_ && SubJets.Jet_CombIVF[iSubJet1]<subJetBDiscrMax_ 
              && SubJets.Jet_CombIVF[iSubJet2]>subJetBDiscrMin_ && SubJets.Jet_CombIVF[iSubJet2]<subJetBDiscrMax_ ) 
          ) continue;
        else if( !fatJetDoubleBTagging_ && FatJetInfo.Jet_CombIVF[iJet]<=fatJetBDiscrCut_ ) continue;
      }

      if (fatJetDoubleSVBTagging_ && 
          (FatJetInfo.Jet_DoubleSV[iJet] < fatJetDoubleSVBDiscrMin_ || FatJetInfo.Jet_DoubleSV[iJet] > fatJetDoubleSVBDiscrMax_) 
         ) continue ;  

      //// apply b-tagging scale factors
      double wtFatJet = 1.;
      if( applySFs_ && !isData ) {
        if( applyFatJetBTagging_ && fatJetDoubleBTagging_ ) {
          wtFatJet *= reader.eval(BTagEntry::JetFlavor(SubJets.Jet_flavour[iSubJet1]), SubJets.Jet_eta[iSubJet1], SubJets.Jet_pt[iSubJet1]); 
          wtFatJet *= reader.eval(BTagEntry::JetFlavor(SubJets.Jet_flavour[iSubJet2]), SubJets.Jet_eta[iSubJet2], SubJets.Jet_pt[iSubJet2]); 
        }
        else if( applyFatJetBTagging_ && !fatJetDoubleBTagging_ )
          wtFatJet *= reader.eval(BTagEntry::JetFlavor(FatJetInfo.Jet_flavour[iJet]), FatJetInfo.Jet_eta[iJet], FatJetInfo.Jet_pt[iJet]); 
      }

      double wtJetPt = 1.;
      if (doFatJetPtReweighting_ && !isData) { 
        wtJetPt *= GetEvtWtJetPt(file_FatJetPtWt_, hist_FatJetPtWt_, FatJetInfo.Jet_pt[iJet]) ;
        wtFatJet *= wtJetPt ;
      }

      int jetabsflav(abs(FatJetInfo.Jet_flavour[iJet]));

      //// fat jet multiplicity
      ++nFatJet;

      //// test if the jet is a gluon splitting b jet
      bool isGSPbb(false);
      if ( FatJetInfo.Jet_nbHadrons[iJet] > 1 ) isGSPbb = true ;

      //// test if the jet is a gluon splitting c jet
      bool isGSPcc(false);
      if ( FatJetInfo.Jet_ncHadrons[iJet] > 1 ) isGSPcc = true ;

      FillHisto("FatJet_nJets", jetabsflav, isGSPbb , isGSPcc, nFatJet , wtPU*wtFatJet) ;
      fillJetHistos(FatJetInfo, iJet, isGSPbb, isGSPcc ,"FatJet", nmu, nselmuon, idxFirstMuon, wtPU*wtFatJet);

      //// ------- start process subjets --------------
      if( usePrunedSubjets_ || useSoftDropSubjets_ ) {

        for(int sj=0; sj<2; ++sj) {
          int iSubJet = SubJetInfo.Jet_nFirstSJ[iJet];
          if( sj==1 ) iSubJet = SubJetInfo.Jet_nLastSJ[iJet];

          double sjpt      (SubJets.Jet_pt[iSubJet]) ;
          double sjeta     (SubJets.Jet_eta[iSubJet]) ;
          //double sjabseta  (fabs(SubJets.Jet_eta[iSubJet])) ;
          double sjphi     (SubJets.Jet_phi[iSubJet]) ;
          double sjabsflav (abs(SubJets.Jet_flavour[iSubJet])) ; 
          int    sjnsv     (SubJets.Jet_SV_multi[iSubJet]);
          double sjcsvv2   (SubJets.Jet_CombIVF[iSubJet]) ; 
          double sjjp      (SubJets.Jet_Proba[iSubJet]) ;
          double sjsvmass  (SubJets.TagVarCSV_vertexMass[iSubJet]);
          double sjdeepcsv (SubJets.Jet_DeepCSVBDisc[iSubJet]) ;
          bool   sjIsGSPbb (false) ; 
          bool   sjIsGSPcc (false) ; 

          double sjjesunc(0);
          if ( doJECUncert_ ) 
            GetJESUncert( sjpt, sjeta, sjjesunc  ) ; 
          sjpt *= 1 + sjjesunc;

          int nselmuonSubJet(0);
          int nmuSubJet(0);

          if (SubJets.nPFMuon>0) {
            for (int iMu=0; iMu<SubJets.nPFMuon; ++iMu) {
              if (SubJets.PFMuon_IdxJet[iMu]==iSubJet ) {
                ++nmuSubJet;
                if (passMuonSelection(iMu, SubJets, iSubJet, (dynamicMuonSubJetDR_ ? subjet_dR/2 : 0.4 ))) {
                  ++nselmuonSubJet;
                }
              }
            }
          }

          //std::cout << " fatjet pt = " << SubJets.Jet_pt [iJet] 
          //  << " eta = " << fabs(SubJets.Jet_eta[iJet]) 
          //  << " sj deepcsv = " << sjdeepcsv
          //  << " nmu = " << SubJets.nPFMuon << " nselmu = " << nselmuonSubJet 
          //  << std::endl;

          if (removeJP0_ && std::signbit(sjjp) ) { 
            continue;
          }

          if(applySubJetMuonTagging_ && nselmuonSubJet==0) continue;  //// if enabled, select muon-tagged subjets
          if(applySubJetBTagging_ && 
              (SubJets.Jet_CombIVF[iSubJet]<=subJetBDiscrMin_ || SubJets.Jet_CombIVF[iSubJet]>subJetBDiscrMax_) 
            ) continue;  //// if enabled, select b-tagged subjets

          //// apply b-tagging scale factors
          double wtSubJet(1.);
          if( applySFs_ && !isData ) {
            if( applyFatJetBTagging_ && fatJetDoubleBTagging_ ) {
              wtSubJet *= wtFatJet;
            }
            else {
              wtSubJet *= reader.eval(BTagEntry::JetFlavor(SubJets.Jet_flavour[iSubJet]), SubJets.Jet_eta[iSubJet], SubJets.Jet_pt[iSubJet]);
              wtSubJet *= reader.eval(BTagEntry::JetFlavor(SubJets.Jet_flavour[iSubJet]), SubJets.Jet_eta[iSubJet], SubJets.Jet_pt[iSubJet]);
            }
          }

          double wtSubJetPt(1.);
          if (doSubJetPtReweighting_ && !isData) {
            wtSubJetPt *= GetEvtWtJetPt(file_SubJetPtWt_, hist_SubJetPtWt_, SubJets.Jet_pt[iSubJet]) ;
            wtSubJet *= wtSubJetPt ;
          }

          //// subjet multiplicity
          ++nSubJet;

          if (!isData && doGSPUp_ && isGSPbb) wtSubJet *= 1.25;
          if (!isData && doGSPDown_ && isGSPbb) wtSubJet *= 0.75;

          // -------For calculating b-fragmentation systematic
          if (sjabsflav == 5){
            if (doBFrag_){
              //double sfbFrag = 1.;
              double drMin = 0.8;   
              double WeightBFrag = 1.;
              double EnergyFraction = 0.; 
              int iB = -1, iptBin = 0, efbin = -1;
              if( sjpt > 500 ) iptBin = 14;
              else if( sjpt > 400 ) iptBin = 13;
              else if( sjpt > 320 ) iptBin = 12;  
              else if( sjpt > 260 ) iptBin = 11;
              else if( sjpt > 210 ) iptBin = 10;
              else if( sjpt > 160 ) iptBin =  9;
              else if( sjpt > 120 ) iptBin =  8;  
              else if( sjpt > 100 ) iptBin =  7;  
              else if( sjpt >  80 ) iptBin =  6;  
              else if( sjpt >  70 ) iptBin =  5;  
              else if( sjpt >  60 ) iptBin =  4;  
              else if( sjpt >  50 ) iptBin =  3;  
              else if( sjpt >  40 ) iptBin =  2;  
              else if( sjpt >  30 ) iptBin =  1;  
              else                 iptBin =  0;

              double B_Mass = 0.;
              for( int ib=0;ib<EvtInfo.nBHadrons;ib++ )
              {
                double drB = reco::deltaR(sjeta,sjphi,EvtInfo.BHadron_eta[ib],EvtInfo.BHadron_phi[ib]);
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
                EnergyFraction = EvtInfo.BHadron_pT[iB]/SubJets.Jet_genpt[iJet];
                efbin = int( EnergyFraction / 0.02 );
                if( efbin >= 0 && efbin < 100 ) 
                {
                  if( doBFragDown_ ) WeightBFrag = BTemplateCorrections[efbin][iptBin][0];
                  if( doBFragUp_ ) WeightBFrag = BTemplateCorrections[efbin][iptBin][1];
                }    
              }

              wtSubJet *= WeightBFrag;
            }
          } //// B fragmentation systematics  

          //// CD Frag Systematic
          if (sjabsflav == 5 || sjabsflav == 4) {
            if (doCDFrag_)
            {
              double sfCD = 1.;
              double drMin = 0.8;   
              bool isDplusMu = false, isDzeroMu = false, isDsubsMu = false;

              int ndaughters = 0;
              for( int k=0;k<EvtInfo.nDHadrons;k++ )
              {
                double dR = reco::deltaR(EvtInfo.DHadron_eta[k], 
                    EvtInfo.DHadron_phi[k], 
                    sjeta,
                    sjphi);
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

              wtSubJet *= sfCD;
            }
          } //// CD fragmentation systematics 

          //// c fragmentation
          if (sjabsflav == 4){
            if (doCFrag_){
              double sfC = 1.;
              double drMin = 0.8;   

              bool hasCquark = 0;
              for( int c=0;c<EvtInfo.ncQuarks;c++ )
              {
                double dRc = reco::deltaR(EvtInfo.cQuark_eta[c], 
                    EvtInfo.cQuark_phi[c], 
                    sjeta,
                    sjphi);
                if( dRc < drMin ) hasCquark = 1;
              }

              if( hasCquark )
              { 
                bool isDplus = false, isDzero = false, isDsubs = false;//, isDbary = false;
                for( int k=0;k<EvtInfo.nDHadrons;k++ )
                {
                  double dR = reco::deltaR(EvtInfo.DHadron_eta[k], 
                      EvtInfo.DHadron_phi[k], 
                      sjeta, 
                      sjphi);
                  if( dR > drMin ) continue;

                  if( abs(EvtInfo.DHadron_pdgID[k]) == 411 ) isDplus = true;
                  if( abs(EvtInfo.DHadron_pdgID[k]) == 421 ) isDzero = true;
                  if( abs(EvtInfo.DHadron_pdgID[k]) == 431 ) isDsubs = true;
                  //if((abs(EvtInfo.DHadron_pdgID[k])/1000)%10 == 4 ) isDbary = true;
                }       

                if( isDplus ) sfC *= 1.37; // PDG2008(0.246+-0.020)
                if( isDzero ) sfC *= 0.91; // PDG2008(0.565+-0.032)
                if( isDsubs ) sfC *= 0.67; // PDG2008(0.080+-0.017)
                // 0.185072, 0.58923, 0.115961
              }
              wtSubJet *= sfC;
            }    
          } //// c fragmentation systematics 

          //// K0s Lambda sys
          if (sjabsflav < 4 || sjabsflav == 21)
          {
            if (doK0L_)
            {
              double sfK0L = 1.;
              int nK0s = 0, nLambda = 0;
              for( int k=0;k<EvtInfo.nGenV0;k++ )
              {
                double dR = reco::deltaR(EvtInfo.GenV0_eta[k], 
                    EvtInfo.GenV0_phi[k], 
                    sjeta,
                    sjphi);
                if( dR > 0.3 ) continue;
                int pdgid = abs(EvtInfo.GenV0_pdgID[k]);
                if( pdgid == 310 )  nK0s++;
                if( pdgid == 3122 ) nLambda++;
              }
              if( nK0s > 0 )    sfK0L *= 1.3;
              if( nLambda > 0 ) sfK0L *= 1.5;
              wtSubJet *= sfK0L;
            }
          } //// KOs Lambda systematics

          //// Templates for LT method

          TString histoTag = "SoftDropSubJet";
          if( usePrunedSubjets_ ) histoTag = "PrunedSubJet";
          else if( useSoftDropSubjets_ ) histoTag = "SoftDropSubJet";

          FillHisto(histoTag+"_nJets", sjabsflav, 0 , 0, nSubJet , wtPU*wtSubJet) ;
          fillJetHistos(SubJets, iSubJet, 0, 0 ,histoTag, nmuSubJet, nselmuonSubJet, idxFirstMuon, wtPU*wtSubJet);

          for (auto const pt : pts_) {

            if (sjpt < pt.second.first || sjpt >= pt.second.second) continue;

            std::string sjptbin = pt.first;
            std::stringstream hname;

            hname.str(std::string());
            hname << histoTag << "_JP_all_" << sjptbin ;
            FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjjp  ,wtSubJet);

            hname.str(std::string());
            if ( sjnsv > 0  ) {
              hname << histoTag << "_JP_SVpass_" << sjptbin ;
            }
            else {
              hname << histoTag << "_JP_SVfail_" << sjptbin ;
            }
            FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjjp  ,wtSubJet);

            hname.str(std::string());
            if ( sjcsvv2 >= CSVv2L_  ) {
              hname << histoTag << "_JP_CSVv2Lpass_" << sjptbin ;
            }
            else {
              hname << histoTag << "_JP_CSVv2Lfail_" << sjptbin ;
            }
            FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjjp  ,wtSubJet);

            hname.str(std::string());
            if ( sjcsvv2 >= CSVv2M_  ) {
              hname << histoTag << "_JP_CSVv2Mpass_" << sjptbin ;
            }
            else {
              hname << histoTag << "_JP_CSVv2Mfail_" << sjptbin ;
            }
            FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjjp  ,wtSubJet);

            hname.str(std::string());
            if ( sjdeepcsv >= DeepCSVL_  ) {
              hname << histoTag << "_JP_DeepCSVLpass_" << sjptbin ;
            }
            else {
              hname << histoTag << "_JP_DeepCSVLfail_" << sjptbin ;
            }
            FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjjp  ,wtSubJet);

            hname.str(std::string());
            if ( sjdeepcsv >= DeepCSVM_  ) {
              hname << histoTag << "_JP_DeepCSVMpass_" << sjptbin ;
            }
            else {
              hname << histoTag << "_JP_DeepCSVMfail_" << sjptbin ;
            }
            FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjjp  ,wtSubJet);

            if (sjnsv > 0) {

              hname.str(std::string());
              hname << histoTag << "_SVmass_all_" << sjptbin ;
              FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjsvmass  ,wtSubJet);

              hname.str(std::string());
              if ( sjcsvv2 >= CSVv2L_  ) {
                hname << histoTag << "_SVmass_CSVv2Lpass_" << sjptbin ;
              }
              else {
                hname << histoTag << "_SVmass_CSVv2Lfail_" << sjptbin ;
              }
              FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjsvmass  ,wtSubJet);

              hname.str(std::string());
              if ( sjcsvv2 >= CSVv2M_  ) {
                hname << histoTag << "_SVmass_CSVv2Mpass_" << sjptbin ;
              }
              else {
                hname << histoTag << "_SVmass_CSVv2Mfail_" << sjptbin ;
              }
              FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjsvmass  ,wtSubJet);

              hname.str(std::string());
              if ( sjdeepcsv >= DeepCSVL_  ) {
                hname << histoTag << "_SVmass_DeepCSVLpass_" << sjptbin ;
              }
              else {
                hname << histoTag << "_SVmass_DeepCSVLfail_" << sjptbin ;
              }
              FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjsvmass  ,wtSubJet);

              hname.str(std::string());
              if ( sjdeepcsv >= DeepCSVM_  ) {
                hname << histoTag << "_SVmass_DeepCSVMpass_" << sjptbin ;
              }
              else {
                hname << histoTag << "_SVmass_DeepCSVMfail_" << sjptbin ;
              }
              FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjsvmass  ,wtSubJet);

              hname.str(std::string());
              if ( sjcsvv2 >= CSVv2L_  ) {
                hname << histoTag << "_JP_CSVv2L_SVpass_" << sjptbin ;
              }
              else {
                hname << histoTag << "_JP_CSVv2L_SVfail_" << sjptbin ;
              }
              FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjjp  ,wtSubJet);

              hname.str(std::string());
              if ( sjcsvv2 >= CSVv2M_  ) {
                hname << histoTag << "_JP_CSVv2M_SVpass_" << sjptbin ;
              }
              else {
                hname << histoTag << "_JP_CSVv2M_SVfail_" << sjptbin ;
              }
              FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjjp  ,wtSubJet);

              hname.str(std::string());
              if ( sjdeepcsv >= DeepCSVL_  ) {
                hname << histoTag << "_JP_DeepCSVL_SVpass_" << sjptbin ;
              }
              else {
                hname << histoTag << "_JP_DeepCSVL_SVfail_" << sjptbin ;
              }
              FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjjp  ,wtSubJet);

              hname.str(std::string());
              if ( sjdeepcsv >= DeepCSVM_  ) {
                hname << histoTag << "_JP_DeepCSVM_SVpass_" << sjptbin ;
              }
              else {
                hname << histoTag << "_JP_DeepCSVM_SVfail_" << sjptbin ;
              }
              FillHisto(hname.str(), sjabsflav, sjIsGSPbb, sjIsGSPcc ,sjjp  ,wtSubJet);

            } //// Filling histos for nSV > 0

          } //// Looping over pT bins 

        } //// process first two subjets

      } //// ------- end process subjets --------------

    } //// End fat jet loop 

    if( !isData && nFatJet>0 ) h1_pt_hat_sel->Fill(EvtInfo.pthat,wtPU);

  } //// End event loop 

} /// End analyze method

// ------------------------------------------------------------------------------
void BTagValidation::fillJetHistos(const JetInfoBranches& JetInfo, const int iJet, const bool isGSPbb, const bool isGSPcc, const TString& histoTag, const int nmu, const int nselmuon, const int idxFirstMuon, const double wt) {

  double ptjet  (JetInfo.Jet_pt[iJet]);
  double etajet (JetInfo.Jet_eta[iJet]);
  double phijet (JetInfo.Jet_phi[iJet]);
  double mass   (JetInfo.Jet_mass[iJet]);
  int    flav   (JetInfo.Jet_flavour[iJet]);
  double ntrkjet(JetInfo.Jet_ntracks[iJet]);
  int    n_sv   (JetInfo.Jet_SV_multi[iJet]);

  FillHisto(histoTag+"_pt_all", flav, isGSPbb , isGSPcc, ptjet , wt) ;
  if (JetInfo.Jet_SV_multi[iJet] > 0) FillHisto(histoTag+"_pt_sv", flav, isGSPbb , isGSPcc ,ptjet , wt) ;
  FillHisto(histoTag+"_eta"         ,flav ,isGSPbb , isGSPcc ,etajet     ,wt) ;
  FillHisto(histoTag+"_phi"         ,flav ,isGSPbb , isGSPcc ,phijet     ,wt) ;
  FillHisto(histoTag+"_mass"        ,flav ,isGSPbb , isGSPcc ,mass       ,wt) ;

  int ntracksel   = 0;

  if ( useJetProbaTree_ ) {
    for (int iTrk=JetInfo.Jet_nFirstTrack[iJet]; iTrk<JetInfo.Jet_nLastTrack[iJet] ; ++iTrk) {

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

      if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D) {
        ++ntracksel;
        FillHisto(histoTag+"_track_nHit",    flav, isGSPbb , isGSPcc ,JetInfo.Track_nHitAll[iTrk]   ,wt);
        FillHisto(histoTag+"_track_HPix",    flav, isGSPbb , isGSPcc ,JetInfo.Track_nHitPixel[iTrk] ,wt);
        FillHisto(histoTag+"_track_IPs",     flav, isGSPbb , isGSPcc ,JetInfo.Track_IPsig[iTrk]     ,wt);
        FillHisto(histoTag+"_track_len",     flav, isGSPbb , isGSPcc ,JetInfo.Track_length[iTrk]    ,wt);
        FillHisto(histoTag+"_track_dist",    flav, isGSPbb , isGSPcc ,fabs(JetInfo.Track_dist[iTrk]),wt);
        FillHisto(histoTag+"_track_pt",      flav, isGSPbb , isGSPcc ,JetInfo.Track_pt[iTrk]        ,wt);
      }

    }
  }

  FillHisto(histoTag+"_muon_multi"  ,flav, isGSPbb ,isGSPcc  ,nmu        ,wt);
  FillHisto(histoTag+"_muon_multi_sel",  flav, isGSPbb ,isGSPcc ,nselmuon   ,wt);

  FillHisto(histoTag+"_track_multi" ,flav ,isGSPbb , isGSPcc ,ntrkjet    ,wt) ;
  FillHisto(histoTag+"_trk_multi_sel",     flav, isGSPbb , isGSPcc ,ntracksel           , wt);

  FillHisto(histoTag+"_sv_multi_0",      flav, isGSPbb , isGSPcc ,n_sv   ,         wt);

  if (n_sv>0) {

    float flight2DSig_sv = JetInfo.SV_flight2D[JetInfo.Jet_nFirstSV[iJet]]/JetInfo.SV_flight2DErr[JetInfo.Jet_nFirstSV[iJet]];
    float sv_dR_jet      = JetInfo.SV_deltaR_jet[JetInfo.Jet_nFirstSV[iJet]];
    float sv_en_rat      = JetInfo.SV_EnergyRatio[JetInfo.Jet_nFirstSV[iJet]];
    float sv_nTrk         = JetInfo.SV_nTrk[JetInfo.Jet_nFirstSV[iJet]] ;

    // --------- SV histograms --------
    FillHisto(histoTag+"_sv_deltaR_jet",   flav, isGSPbb ,isGSPcc ,sv_dR_jet,           wt);
    FillHisto(histoTag+"_sv_en_ratio",     flav, isGSPbb ,isGSPcc ,sv_en_rat,           wt);
    FillHisto(histoTag+"_svnTrk",            flav, isGSPbb ,isGSPcc ,sv_nTrk,             wt);
    FillHisto(histoTag+"_sv_flightSig2D",    flav, isGSPbb ,isGSPcc ,flight2DSig_sv,      wt);

  }

  double jetproba (JetInfo.Jet_Proba[iJet]);
  double jetbproba(JetInfo.Jet_Bprob[iJet]);
  double csvivfv2 (JetInfo.Jet_CombIVF[iJet]);
  double deepcsv  (JetInfo.Jet_DeepCSVBDisc[iJet]);
  double mass_TagVarCSV_sv (JetInfo.TagVarCSV_vertexMass[iJet]);
  double cmvav2   (JetInfo.Jet_cMVAv2[iJet]);
  double doubleb  (JetInfo.Jet_DoubleSV[iJet]);
  double deepDoubleBvLQCD  (JetInfo.Jet_DeepDoubleBvLQCD[iJet]);
  double deepDoubleBvLHbb  (JetInfo.Jet_DeepDoubleBvLHbb[iJet]);
  double deepDoubleCvLQCD  (JetInfo.Jet_DeepDoubleCvLQCD[iJet]);
  double deepDoubleCvLHcc  (JetInfo.Jet_DeepDoubleCvLHcc[iJet]);
  double deepDoubleCvBHcc  (JetInfo.Jet_DeepDoubleCvBHcc[iJet]);
  double deepDoubleCvBHbb  (JetInfo.Jet_DeepDoubleCvBHbb[iJet]);
  double massIndDeepDoubleBvLQCD  (JetInfo.Jet_MassIndDeepDoubleBvLQCD[iJet]);
  double massIndDeepDoubleBvLHbb  (JetInfo.Jet_MassIndDeepDoubleBvLHbb[iJet]);
  double massIndDeepDoubleCvLQCD  (JetInfo.Jet_MassIndDeepDoubleCvLQCD[iJet]);
  double massIndDeepDoubleCvLHcc  (JetInfo.Jet_MassIndDeepDoubleCvLHcc[iJet]);
  double massIndDeepDoubleCvBHcc  (JetInfo.Jet_MassIndDeepDoubleCvBHcc[iJet]);
  double massIndDeepDoubleCvBHbb  (JetInfo.Jet_MassIndDeepDoubleCvBHbb[iJet]);
  double deepBoostedJetbbvsLight (JetInfo.Jet_DeepBoostedJetbbvsLight[iJet]);
  double deepBoostedJetccvsLight (JetInfo.Jet_DeepBoostedJetccvsLight[iJet]);
  double deepBoostedJetTvsQCD (JetInfo.Jet_DeepBoostedJetTvsQCD[iJet]);
  double deepBoostedJetZHccvsQCD (JetInfo.Jet_DeepBoostedJetZHccvsQCD[iJet]);
  double deepBoostedJetWvsQCD (JetInfo.Jet_DeepBoostedJetWvsQCD[iJet]);
  double deepBoostedJetZHbbvsQCD (JetInfo.Jet_DeepBoostedJetZHbbvsQCD[iJet]);

  FillHisto(histoTag+"_JP",       flav, isGSPbb, isGSPcc ,jetproba  ,wt);
  FillHisto(histoTag+"_JBP",      flav, isGSPbb, isGSPcc ,jetbproba ,wt);
  FillHisto(histoTag+"_CSVIVFv2", flav, isGSPbb, isGSPcc ,csvivfv2  ,wt);
  FillHisto(histoTag+"_DeepCSV",  flav, isGSPbb, isGSPcc ,deepcsv   ,wt);
  FillHisto(histoTag+"_TagVarCSV_sv_mass", flav, isGSPbb ,isGSPcc ,mass_TagVarCSV_sv,   wt);
  FillHisto(histoTag+"_cMVAv2",   flav, isGSPbb, isGSPcc ,cmvav2    ,wt);
  FillHisto(histoTag+"_DoubleB",  flav, isGSPbb, isGSPcc ,doubleb   ,wt);
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
  FillHisto(histoTag+"_deepBoostedJetZHbbvsQCD",  flav, isGSPbb, isGSPcc ,deepBoostedJetZHbbvsQCD ,wt);

}

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
      //RS>>else if (isGSPcc && abs(flavour)==4) 		  HistoBtag2D_map[name+"_cfromg"]->Fill(double(value),double(value2),weight);
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
    for(unsigned i=0; i<triggerSelection_.size(); ++i) {
      std::string trigpath = triggerSelection_.at(i) ; 
      std::vector<std::string>::const_iterator it ;
      for ( it = triggerPathNames_.begin(); it != triggerPathNames_.end(); ++it) {
        if ( it->find(trigpath) < std::string::npos ) {
          int triggerIdx = ( it - triggerPathNames_.begin() );
          int bitIdx = int(triggerIdx/32);
          if ( EvtInfo.BitTrigger[bitIdx] & ( 1 << (triggerIdx - bitIdx*32) ) ) {
            //std::cout << " fired trigger " << *it << std::endl;
            ret = true;
            break;
          }
        }
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
  if (JetInfo.PFMuon_pt[muIdx] > 7 && fabs(JetInfo.PFMuon_eta[muIdx]) < 2.4 
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

// ---- Method returns MC event weight for for reweighting to the NPV distribution in the data: substitute for official PU reweighting ----
double BTagValidation::GetEvtWtPV (const std::string file, const std::string hist, const int npv) { 
  double wtPU(1) ;
  TFile* f = new TFile(file.c_str()) ;
  TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) ); 
  wtPU = npv > 0 && npv <= 60 ? hwt->GetBinContent(npv) : 1.; 
  f->Close() ; 
  delete f ;
  delete hwt ; 
  return wtPU ;
}

// ----For calculating MC event weight for reweighting to the jetPt distribution in the data (or signal, depending)
double BTagValidation::GetEvtWtJetPt (const std::string file, const std::string hist, const double jetpt) {
  double wtPt(1) ;
  TFile* f = new TFile(file.c_str()) ;
  TH1D* hwt = new TH1D( *(static_cast<TH1D*>(f->Get( hist.c_str() )->Clone() )) );
  wtPt = jetpt > 0 && jetpt <= 5000 ? hwt->GetBinContent(hwt->GetXaxis()->FindBin(jetpt)) : 1.;
  f->Close() ;
  delete f ;
  delete hwt ;
  return wtPt ;
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

void BTagValidation::GetJESUncert(double jetpt, double jeteta, double& jesunc ) {
  ptr_jecUnc_->setJetPt (jetpt);
  ptr_jecUnc_->setJetEta(jeteta);
  jesunc = ptr_jecUnc_->getUncertainty(true);
  jesunc *= jecshift_ ; 
}

//define this as a plug-in
DEFINE_FWK_MODULE(BTagValidation);
