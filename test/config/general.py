# ------- General options ----------
# campaign name, needed for list of samples definition, etc...

campaigns       = [  #'Run2016DoubleB'
                    #,'Run2016DoubleB_BFRAGUP'
                    #,'Run2016DoubleB_BFRAGDOWN'
                    #,'Run2016DoubleB_PUUP'
                    #,'Run2016DoubleB_PUDOWN'                    
                    #,'Run2016DoubleB_K0L'
                    #,'Run2016DoubleB_CD'
                    #,'Run2016DoubleB_CFRAG'
                    #,'Run2016DoubleB_NTRACKS'

                    #,'Run2016DDBvL'
                    #,'Run2016DDBvL_BFRAGUP'
                    #,'Run2016DDBvL_BFRAGDOWN'
                    #,'Run2016DDBvL_PUUP'
                    #,'Run2016DDBvL_PUDOWN'                    
                    #,'Run2016DDBvL_K0L'
                    #,'Run2016DDBvL_CD'
                    #,'Run2016DDBvL_CFRAG'
                    #,'Run2016DDBvL_NTRACKS'

                    #,'Run2017DoubleB'
                    #,'Run2017DoubleB_BFRAGUP'
                    #,'Run2017DoubleB_BFRAGDOWN'
                    #,'Run2017DoubleB_PUUP'
                    #,'Run2017DoubleB_PUDOWN'                    
                    #,'Run2017DoubleB_K0L'
                    #,'Run2017DoubleB_CD'
                    #,'Run2017DoubleB_CFRAG'
                    #,'Run2017DoubleB_NTRACKS'

                    #,'Run2017DDBvL'
                    #,'Run2017DDBvL_BFRAGUP'
                    #,'Run2017DDBvL_BFRAGDOWN'
                    #,'Run2017DDBvL_PUUP'
                    #,'Run2017DDBvL_PUDOWN'                    
                    #,'Run2017DDBvL_K0L'
                    #,'Run2017DDBvL_CD'
                    #,'Run2017DDBvL_CFRAG'
                    #,'Run2017DDBvL_NTRACKS'

                    #'Run2017Comm'
					         ]

btagAna_groups  = [ 'EventInfo','Devdatta','DoubleBCommissioning' ]


# Choose True if you want to overwrite already existing files/results
force_all       = False
# If True all files are copied locally and used, else use files from original location
work_locally    = False

# -------- Merge options -----------
# Luminosity (this number doesn't mean anything since data/MC is "manually" scaled)
luminosity      = 1
# Name of the analyzer
analyzer_module = 'btagval'
# Groups for histogram merging
groups          = ['DATA', 'QCDMu+']


# -------- Batch options -----------
# Choose if you want to use batch: False, condor, lxbatch
batch_type      = 'condor'
# Number of jobs per sample: -1 = all, x = some arbitrary number
number_of_jobs  = -1
# Number of files per job
number_of_files = 10
# Send jobs switch
send_jobs       = True
# lxbatch options
queue_lxbatch   = 'cmscaf1nh' # LXBatch queue (choose among cmst3 8nm 1nh 8nh 1nd 1nw)
queue_condor   = 'espresso' # LXBatch queue (choose among cmst3 8nm 1nh 8nh 1nd 1nw)
# batch templates
batch_templates = {
  'copy'      : ['copy.txt','copy.py', 'copy.sh'],
  'histogram' : ['histogram.txt','btagvalidation_cfg.py', 'histogram.sh'],
  'check'     : ['check.txt','check.py', 'check.sh'],
}

# -------- Browse/copy options -----------
remote_locations = {
  'storage_element' : {
    'eos' : '', # added by rizki Jan10-2018
    #'eos' : '/eoscms.cern.ch/', # Used for gfal
    # 'eos' : '/srm-eoscms.cern.ch:8443/srm/v2/server?SFN=', # Used for lcg
  },
  'path'   : {
    'eos' : '/eos/cms/store/group/phys_btag/BoostedBTag/BTagNTuples/' #2017/9_4_X_v04/'
    #'eos' : '/eos/cms/store/group/phys_btag/BoostedBTag/BTagNTuples/2017/9_4_X',   # added by rizki Jan10-2018
    #'eos' : 'store/group/phys_btag/BoostedBTag/BTagNTuples/2017/9_4_X',   # Used for gfal
    # 'eos' : 'eos/cms/store/group/phys_btag/BoostedBTag/BTagNTuples/2017/9_4_X', # Used for lcg
  },
  'path_ex': {
    'eos' : '/eos/cms/store/group/phys_btag/BoostedBTag/BTagNTuples/' #2017/9_4_X_v04/',
  },
  'protocol': {
    'eos'  : '', # added by rizki Jan10-2018
    #'eos'  : 'root:/', # Used for gfal
    # 'eos'  : 'srm:/', # Used for lcg
  },
}
# Keywords which are going to be searched (or not) for.
search_keywords = {
  'all' : ['.root'],
  'any' : [],
  'none': ['failed', 'log']
}
