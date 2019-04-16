import os
import copy
import subprocess as sp
import numpy as np

import ROOT

from RecoBTag.BTagValidation.MiscTool import *

class MergeTool(object):
  '''
  Loop over all root files and merge them by sample/version using hadd.
  '''
  def __init__(self, configuration):

    Print('python_info', '\nCreated instance of MergeHandler class')

    # Get campaign name
    self.campaigns                 = configuration.general.campaigns
    self.campaigns_info            = configuration.campaigns.info

    # Force all regardless of already existing files
    self.force_all                = configuration.general.force_all

    # Work with local files or remote
    self.work_locally             = configuration.general.work_locally

    # ------ Paths -------
    self.path_main                = configuration.paths.main
    self.path_logical_file_names  = configuration.paths.logical_file_names
    self.path_histograms          = make_directory( configuration.paths.histograms)
    self.path_samples             = configuration.paths.samples
    self.path_plots_final         = make_directory( configuration.paths.plots_final)

    # ------ Samples -------
    self.samples_info             = configuration.samples.info

    # ------ Browsing options -------
    self.remote_locations         = configuration.general.remote_locations

    # Merge datasets configs
    self.luminosity               = configuration.general.luminosity
    self.number_of_files          = configuration.general.number_of_files
    self.analyzer_module          = configuration.general.analyzer_module
    self.groups                   = configuration.general.groups

  def merge_histograms(self):

    Print('python_info', '\nCalled merge_histograms function.')

    # Main loop over list of campaigns which are defined in general.py
    for _c in self.campaigns:

      _path_histograms = make_directory(os.path.join( self.path_histograms, _c))

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

            # Using this procedure since we've used it before to group files for each histogram
            _file = os.path.join( self.path_logical_file_names, _location, _l, _s, _subsample + '.txt')
            with open( _file, 'r') as _f:
              _f          = _f.readlines()
              _histograms = [ _f[_i].replace( self.path_samples, _path_histograms).replace( self.remote_locations['path'][_l], _path_histograms).replace( self.remote_locations['storage_element'][_l], '').rstrip()
                              for _i in xrange(0, len(_f), self.number_of_files) ]

            # Check if histogram exists
            _histograms = [ _h for _h in _histograms if check_root_file( _h)]

            # Create root file for merging
            _file_merge   =  os.path.join( _path_histograms, _subsample + '.root' )

            if not len(_histograms):
               Print('error', 'No input histograms')
               continue

            if check_root_file(_file_merge) and not self.force_all:
              Print('python_info', 'File {0} already exists.'.format(_file_merge))
              continue

            # Merge files using hadd command
            _command = ['hadd', '-f', _file_merge] + _histograms
            Print('status','{0}'.format(' '.join(_command)))
            sp.check_output(_command)

  def merge_datasets(self):

    Print('python_info', '\nCalled merge_datasets function.')

    # Main loop over list of campaigns which are defined in general.py
    for _c in self.campaigns:

      Print('status', '\nCampaign: {0}'.format(_c))

      _path_histograms    = os.path.join( self.path_histograms, _c)

      # Make root file with merged histograms
      print  self.path_plots_final, self.campaigns_info[_c]['final_output']
      _final = ROOT.TFile.Open( os.path.join( self.path_plots_final, self.campaigns_info[_c]['final_output']), 'recreate')

      # Loop over groups
      for _g in self.groups:

        _final_histograms = {}

        Print('status', '\nGroup: {0}'.format(_g))

        # Loop over samples
        for _s in self.campaigns_info[_c]['samples']:

          # Loop over all subsamples
          for _ss in self.campaigns_info[_c]['samples'][_s]:

            _subsample = self.samples_info[_s]['subsample'][_ss].replace('/', '__')

            # Check if subsample in the current group
            if not self.samples_info[_s]['group'] == _g:
              continue

            _input_file = os.path.join( _path_histograms, _subsample + '.root' )

            if not check_root_file( _input_file):
              continue

            Print('status', 'Sample: {0}'.format(_subsample))

            _input_file = ROOT.TFile.Open( _input_file, 'read')

            _htemp          = _input_file.Get( os.path.join( self.analyzer_module,'h1_CutFlow_unw'))
            _n_events_all   = _htemp.GetBinContent(1)

            # Setup sample normalization
            if self.samples_info[_s]['type'] == 'MC':
              _scale = float(self.samples_info[_s]['xs'][_ss]*self.luminosity/_n_events_all)
            else:
              _scale = 1.

            # get the number of histograms
            _n_histograms = _input_file.Get( self.analyzer_module).GetListOfKeys().GetEntries()

            # Loop over all histograms
            for _h in xrange(_n_histograms):

              _histogram_name = _input_file.Get( self.analyzer_module).GetListOfKeys()[_h].GetName()
              _htemp          = _input_file.Get( os.path.join( self.analyzer_module, _histogram_name) )

              if _histogram_name not in _final_histograms.keys():
                _final_histograms[_histogram_name] = copy.deepcopy( _htemp)
                _final_histograms[_histogram_name].SetName(_g + '__' + _histogram_name)
                _final_histograms[_histogram_name].Scale( _scale)
              else:
                _final_histograms[_histogram_name].Add( _htemp, _scale)

            Print('analysis_info', 'Normalization: {0}'.format(_scale))
            Print('analysis_info', 'Number of histograms: {0}'.format(_n_histograms))

            _input_file.Close()

        #Write histograms part
        _final.cd()
        for _h in sorted(_final_histograms.keys()):
          _final_histograms[_h].Write()

      # To check smootheness:
      #try:
      if True:
        _pt_hat = _final.Get(_g+'__h1_pt_hat_sel')
        _cont, _bins = [], []
        _nbins = _pt_hat.GetNbinsX()
        for i in range(_nbins):
          _cont.append(_pt_hat.GetBinContent(i))
          _bins.append(_pt_hat.GetBinCenter(i))
        for ix, _cont_val in enumerate(_cont[::-1]):
          if _cont_val > 0.: break
        for ix, _bin in enumerate(_bins[::-1]):
          if _bin < 1400.: break
        _set_last = ix
        _cont = _cont[3:-_set_last]
        _bins = _bins[3:-_set_last]
        _rebin = np.round(len(_cont)/40)
        _print_bin, _print_cont = [], [] 
        for i in range(40):
          _print_cont.append(np.sum(_cont[i*_rebin : (i+1)*_rebin]))
          _print_bin.append(_bins[i*_rebin -_rebin])
        _print_cont = np.array(_print_cont)
        _print_cont = np.sqrt(np.interp(_print_cont, (_print_cont.min(), _print_cont.max()), (1, 10000)))-1
        print "Sqrt count histogram to account for larger scale variation -- should be smooth"
        print str("  pT").ljust(6), "sqrt count".rjust(10)
        print "-"*50
        for _bin, _val in zip(_print_bin, _print_cont):
          print str(_bin).ljust(6), str(np.round(_val,4)).rjust(10), "|" , "+" * int(np.round(_val))

      #except:
      #  Print('error', "pt_hat hist print failed  , should check merged sample continuity otherwise") 
      _final.Close()

      Print('status', 'Final histograms saved in {0}'.format( os.path.join( self.path_plots_final, self.campaigns_info[_c]['final_output'])))
