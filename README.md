BTagValidation for 2016 data and MC with CMSSW_9_4_X reprocessing
==============

Boosted b-tag validation analysis 

To set up:

Follow the directions to clone cms-btv-pog/RecoBTag-PerformanceMeasurements at https://github.com/cms-btv-pog/RecoBTag-PerformanceMeasurements

After setting up the PerformanceMeasurements package:

```
cd CMSSW_9_4_12/src      

git clone -b boostedbb_SFComm_9xx_DeepDoubleX_varGroups git://github.com/cms-btv-pog/RecoBTag-BTagValidation.git RecoBTag/BTagValidation

scram b -j8

cd RecoBTag/BTagValidation/test/

cmsRun btagvalidation_cfg.py groups=DoubleBCommissioning useSoftDropSubjets=False doNewJEC=False fatJetTau21Max=1 maxEvents=1000
```

To produce SF templates for DDX (DDBvL, DDCvL, DDCvB) or DoubleB
```
cmsRun btagvalidation_cfg.py groups=EventInfo,Devdatta,DoubleBCommissioning doNewJEC=False fatJetTau21Max=1 maxEvents=-1 produceDDXSFtemplates=True DEBUG=False runOnData=False chooseDDXtagger=DDBvL fatJetDoubleTagging=True applyFatJetMuonTaggingV2=True useSoftDropSubjets=True
```  
To redefine WPs for DDX taggers go to (note these names should correspon to chooseDDXtagger) : `aux/DDX.json`

## To run systematics:
  `src/BTagValidation.cc` has b fragmentation, c fragmentation, ntracks, c->D fragmentation, K0/Lambda, and pileup up and down systematics implemented. For b fragmentation, in `test/btagvalidation_cfg.py`, `doBFrag=True` and you need to run once with `doBFragUp=True` and once with `doBFragDown=True`. For c fragmentation, `doCFrag=True`. For c->D fragmentation, `doCDFrag=True`. For K0/Lambda, `doK0L=True`.
  
  For ntracks reweighting, you have to run `test/dataMC_ntracks_reweight/nTracks_wt.py` or `test/dataMC_weight_calc/datamc_weight_calc.py` over a file that already contains the QCD and data distributions, in order to get a root file that will reweight the QCD ntracks distribution to look like the data ntracks distribution. That file is fed to `test/btagvalidation_cfg.py` as FileNtracksWt and you must set `doNtracksReweighting=True`.
  
  For pileup up and down, you use appropriate up/down PU Profile by setting `File_PUDistData` in `test/btagvalidation_cfg.py` properly. (usually +- 5% nominal minBias xsec)
  
 # Using PyWrapper Tools for to create campaigns and submit jobs
 - Open a proxy and copy it to a folder readable by condor (not `/tmp`)
 - In `config/paths.py` specify the path to proxy and it is recommended to specify a path for results that has sufficient space.
 - Define samples available to the wrapper in `config/samples.py`, they should be relatie to the overall paths specified in `config/general.py`
 - Define default `btagvalidation_cfg.py` options in `config/btagvalidation.py`
 - Define actual campaigns to run in `config/campaigns.py` including list of samples and configs differing from those in `config/btagvalidation.py`, \
 for similar campaigns such as running systematics using `copy.deepcopy` on the dictionary and modifying only the pertinent settings is recommended.
 - Finally in `config/general.py` specify
 	- overall paths to samples
 	- `number_of_files` to include in each job
 	- `number_of_jobs` to submit
 	- Set `force_all=True` to overwrite completed jobs, `force_all=False` allows you to softly resubmit failed jobs
