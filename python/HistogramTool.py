import os

from RecoBTag.BTagValidation.MiscTool import *
from RecoBTag.BTagValidation.BatchTool import *

class HistogramTool(object):
  ''' Creates histogram jobs, sends them to batch, checks if everything went well.'''

  def __init__(self, configuration):

    Print('python_info', '\nCreated instance of HistogramHandler class')

    # Get campaign name
    self.campaigns                 = configuration.general.campaigns
    self.campaigns_info            = configuration.campaigns.info

    # Force all regardless of already existing files
    self.force_all                = configuration.general.force_all

    # Work with local files or remote
    self.work_locally             = configuration.general.work_locally

    # ------ Paths -------
    self.path_main                = configuration.paths.main
    self.path_samples             = configuration.paths.samples
    self.path_logical_file_names  = configuration.paths.logical_file_names
    self.path_batch_results       = make_directory( configuration.paths.batch_results, 'histograms')
    self.path_batch_templates     = make_directory( configuration.paths.batch_templates, 'histograms')
    self.path_histograms          = make_directory( configuration.paths.histograms)
    self.path_proxy               = configuration.paths.proxy

    # ------ Batch options -------
    self.batch_type               = configuration.general.batch_type
    self.number_of_jobs           = configuration.general.number_of_jobs
    self.number_of_files          = configuration.general.number_of_files
    self.send_jobs                = configuration.general.send_jobs
    self.batch_templates          = configuration.general.batch_templates['histogram']
    self.queue_lxbatch            = configuration.general.queue_lxbatch
    self.queue_condor            = configuration.general.queue_condor

    # ------ Samples -------
    self.samples_info             = configuration.samples.info

    # ---- btag configuration ----
    self.config_btagvalidation    = configuration.btagvalidation.parameters
    self.btagAna_groups           = configuration.general.btagAna_groups

    # ------ Browsing options -------
    self.remote_locations         = configuration.general.remote_locations

  def make_and_send_jobs(self):

    Print('python_info', '\nCalled make_and_send_jobs function.')

    # Main loop over list of campaigns which are defined in general.py
    for _c in self.campaigns:

      _path_batch_results = os.path.join( self.path_batch_results, _c)
      _path_histograms    = os.path.join( self.path_histograms, _c)

      # Loop over samples
      for _s in self.campaigns_info[_c]['samples']:

        # Loop over all remote locations
        for _l in self.remote_locations['path']:

          # Loop over all subsamples
          for _ss in self.campaigns_info[_c]['samples'][_s]:

            _subsample = self.samples_info[_s]['subsample'][_ss].replace('/', '__')

            Print('status', '\nSample: {0}, {1}, {2}'.format(_s, _l, _subsample))

            if self.work_locally:
              _location = 'local'
            else:
              _location = 'remote'

            # Get list of files and group them wrt to number of files per job
            _file = os.path.join( self.path_logical_file_names, _location, _l, _s, _subsample + '.txt')
            with open( _file, 'r') as _f:
              _f     = _f.readlines()
              _files = [ map(str.strip, _f[_i:_i+self.number_of_files]) for _i in xrange(0, len(_f), self.number_of_files)]

            # Loop over list of files per one job
            for _n, _f in enumerate(_files):

              # Number of jobs limit
              if _n >= self.number_of_jobs and self.number_of_jobs != -1:
                continue

              # If file exists and it is ok, skip to next
              _output_file = _f[0].replace( self.path_samples, _path_histograms).replace( self.remote_locations['path'][_l], _path_histograms).replace( self.remote_locations['storage_element'][_l], '')
              if check_root_file( _output_file) and not self.force_all:
                Print('python_info','File {0} exists and skipped.'.format(_output_file))
                continue

              # If using remote files, add them protocol so that is possible to open them
              if not self.work_locally:
                _f = [ self.remote_locations['protocol'][_l] + _ff.replace( self.remote_locations['path'][_l], self.remote_locations['path_ex'][_l]) for _ff in _f]

              # Create output dir for histograms
              make_directory( '/'.join(_output_file.split('/')[:-1]))

              # To create batch jobs, one needs to edit template script which is done below
              _x                        = _output_file.split('/')                                                  # Split output file into chunks wrt to '/'
              _output_file              = "'" + _output_file + "'"                                                 # output of cmsRun
              _input_files              = "'" + "','".join(_f) + "'"                                               # merge all input files into one string, needed for cmsRun
              _batch_file_wo_ext        = _x[-1].replace('.root', '')                                              # Get only file name without extensions
              _path_batch               = '/'.join(_x[:-1]).replace( _path_histograms, _path_batch_results)         # path to newly created batch script
              _path_batch_file_wo_ext   = os.path.join( _path_batch, _batch_file_wo_ext)                           # path and name of the script without any extension
              _protocol                 = self.remote_locations['protocol'][_l]                                    # protocol used for copying
              _path_proxy               = self.path_proxy                                                          # one needs proxy file to give permission to condor
              _file_proxy               = self.path_proxy.split('/')[-1]

              _btagAna_groups           = self.btagAna_groups

              # Pull batch arguments from config/btagvlidation.py
              _batch_arguments = {'<' + _k + '>': _vv for _k,_vv in self.config_btagvalidation.iteritems()}
              # Setup some arguments manually
              _batch_arguments['<path_batch>']              = _path_batch
              _batch_arguments['<condor_queue>']              = self.queue_condor
              _batch_arguments['<path_batch_file_wo_ext>']  = _path_batch_file_wo_ext
              _batch_arguments['<input_files>']             = _input_files
              _batch_arguments['<output_file>']             = _output_file
              _batch_arguments['<protocol>']                = _protocol
              _batch_arguments['<X509_USER_PROXY_file>']    = _file_proxy
              _batch_arguments['<X509_USER_PROXY_path>']    = _path_proxy
              _batch_arguments['<SCRAM_ARCH>']              = os.environ['SCRAM_ARCH']
              _batch_arguments['<CMSSW_dir>']               = os.path.join( os.environ['CMSSW_BASE'], 'src')
              _batch_arguments['<btagAna_groups>']          = _btagAna_groups

              # Check if there is something to overwrite from campaigns.py for each sample
              if _s in self.campaigns_info[_c]['btagvalidation_cfg']:

                Print('status','Overwriting configs: {0}'.format(self.campaigns_info[_c]['btagvalidation_cfg'][_s]))

                for _b in self.campaigns_info[_c]['btagvalidation_cfg'][_s]:
                  _batch_arguments['<' + _b + '>'] = self.campaigns_info[_c]['btagvalidation_cfg'][_s][_b]

              # Create batch tool instance
              _batch = BatchTool( self.path_batch_templates, self.batch_templates, _batch_arguments, self.batch_type, self.queue_lxbatch)
              # Make scripts first
              _batch.make_scripts()
              # Send jobs
              if self.send_jobs:
                _batch.send_job()
