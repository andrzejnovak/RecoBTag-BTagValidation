import os
import subprocess as sp
import Queue
from RecoBTag.BTagValidation.MiscTool import *
from RecoBTag.BTagValidation.BatchTool import *

class FileTool(object):
  '''Handle files, find their location, check if there are non existing files, ...'''

  def __init__(self, configuration):

    Print('python_info', '\nCreated instance of FileTool class')

    # ------ Paths -------
    self.path_main                = configuration.paths.main
    self.path_samples             = make_directory( configuration.paths.samples)
    self.path_logical_file_names  = make_directory( configuration.paths.logical_file_names)
    self.path_batch_results       = make_directory( configuration.paths.batch_results)
    self.path_batch_templates     = make_directory( configuration.paths.batch_templates)
    self.path_proxy               = configuration.paths.proxy

    # ------ Batch options -------
    self.batch_type               = configuration.general.batch_type
    self.number_of_jobs           = configuration.general.number_of_jobs
    self.send_jobs                = configuration.general.send_jobs
    self.batch_templates          = configuration.general.batch_templates

    # ------ Samples -------
    self.samples_info             = configuration.samples.info
    self.samples                  = self.samples_info.keys()

    # ------ Browsing options -------
    self.remote_locations         = configuration.general.remote_locations
    self.search_keywords          = configuration.general.search_keywords

  # ------------------- Find files remotely ---------------------------
  def _wrapper_gfal_ls(self, location, protocol):
    ''' Wrapper for ls, which is used for browsing remote directories.'''

    Print('python_info', '\nCalled wrapper_gfal_ls function.')

    #_command = ['ls', '-l']
    _command = ['find']
    _command.append(protocol + location)

    #_command = ['gfal-ls', '-l'] #,'-Hl'] #commented by rizki
    #_command.append(protocol + location) #commented by rizki

    # _command = ['lcg-ls -b -D -Hl srmv2'] #,'-Hl']
    # _command.append('"' + protocol + location + '"')

    Print('python_info', 'Command: ' + ' '.join(_command))

    return sp.check_output(_command)

  def _wrapper_gfal_ls_r(self, location, protocol):
    ''' Wrapper for recursive ls, which is used for browsing remote directories.'''

    Print('python_info', '\nCalled wrapper_gfal_ls_r function.')

    _paths = Queue.Queue()
    _paths.put(location)

    # list to store logical file names
    _logical_file_names = []

    while not _paths.empty():

      # get one path
      _x = _paths.get()

      # do ls
      _y = self._wrapper_gfal_ls( _x, protocol)

      for _p in _y.splitlines():

        _type = _p[0]
        _name = _p.split(' ')[-1].rstrip()

        # If this particular ls result is directory, put in ls queue
        if _type == 'd':
          _paths.put( os.path.join(_x, _name))

        # Else just store in list
        else:
          _logical_file_names.append(os.path.join(_x, _name))

    return _logical_file_names

  def save_logical_file_names_all_samples_remotely(self):
    '''
    Tries to find all the files for the corresponding samples and saves them in .txt
    NOTE: this function overwrites files every time.
    '''

    Print('python_info', '\nCalled save_logical_file_names_all_samples function.')

    # Loop over samples
    for _s in self.samples:

      Print('status', '\nSample: {0}'.format(_s))

      # Loop over all remote locations
      for _l in self.remote_locations['path']:

        make_directory( self.path_logical_file_names, 'remote', _l, _s)

        # Browse for the sample files
        try:

          # Get LFNs
          _remote_path  = os.path.join( self.remote_locations['storage_element'][_l], self.remote_locations['path'][_l], _s)
          _remote_lfns  = self._wrapper_gfal_ls_r( _remote_path, self.remote_locations['protocol'][_l])

          # Filter logical files names so that only interesting ones pass
          _keywords_any  = self.samples_info[_s]['subsample'].values()  # filter lfn which has subsample string
          _remote_lfns   = [ _ll for _ll in _remote_lfns if
                              filter_keywords( _ll, self.search_keywords['all'], self.search_keywords['any'] + _keywords_any, self.search_keywords['none'])]

          # Group files according to subsample and save them in separate files
          _remote_lfns = { _ss : filter(lambda x: '/' + _ss + '/' in x, _remote_lfns) for _ss in self.samples_info[_s]['subsample'].values()}

          # Save into files
          for _ss, _r in _remote_lfns.iteritems():

            _file = os.path.join( self.path_logical_file_names, 'remote', _l, _s, _ss.replace('/', '__') + '.txt')

            # Save to a file
            with open(_file, 'w') as _output:
              _output.write('\n'.join(_r))

            Print('status', 'The LFNs were written to "{}".'.format( _file))

        # If something gets wrong notify
        except Exception, e:
          Print('error', "Error with sample {0} at {1}: {2}".format( _s, _l, e))

  def check_missing_files_all_samples_remotely(self):

    Print('python_info', '\nCalled check_missing_files_all_samples_remotely function.')

    # Loop over samples
    for _s in self.samples:

      # Loop over all remote locations
      for _l in self.remote_locations['path']:

        # Loop over all subsamples
        for _ss in self.samples_info[_s]['subsample'].values():

          Print('status', '\nSample: {0}, {1}, {2}'.format(_s, _l, _ss))

          _file = os.path.join( self.path_logical_file_names, 'remote', _l, _s, _ss.replace('/', '__') + '.txt')

          if os.path.isfile(_file):

            _file_missing = _file.replace('.txt', '_missing.txt')

            # Load file with remote lfns
            with open(_file, 'r') as _f:

              # strip newline character
              _f        = map( str.rstrip, _f)

              # extract file number only
              _start    = 'Commissioning_'
              _end      = '.root'
              _numbers  = sorted([ int(_x[_x.find(_start) + len(_start): _x.find(_end)]) for _x in _f])

            # If subsample file is empty skip
            if len(_f) == 0:
              _numbers_missing = []
              Print('error', 'File is empty {0}'.format(_file))

            else:
              # check if any file is missing in the sequence
              _numbers_missing = list(set(range( _numbers[0], _numbers[-1] + 1)) - set(_numbers))
              _numbers_missing = map(str, _numbers_missing)

            if not len(_numbers_missing) == 0:
              Print('error', 'Missing files {0}'.format(','.join( _numbers_missing )))

            # Save the result to a file
            with open(_file_missing, 'w') as _ff:
              _ff.write('\n'.join( _numbers_missing))

            Print('analysis_info', 'Result saved in file {0}'.format( _file_missing))

          else:
            Print('error', 'Missing file {0}'.format(_file))

  # ------------------- Find files localy ---------------------------
  def save_logical_file_names_all_samples_locally(self):
    Print('python_info', '\nCalled save_logical_file_names_all_samples function.')

    # Loop over samples
    for _s in self.samples:

      Print('status', '\nSample: {0}'.format(_s))

      # Loop over all remote locations
      for _l in self.remote_locations['path']:

        make_directory( self.path_logical_file_names, 'local', _l, _s)

        # Loop over all subsamples
        for _ss in self.samples_info[_s]['subsample'].values():

          Print('status', '\nSample: {0}, {1}, {2}'.format(_s, _l, _ss))

          _file_remote        = os.path.join( self.path_logical_file_names, 'remote', _l, _s, _ss.replace('/', '__') + '.txt')
          _file_local         = _file_remote.replace('remote', 'local')
          _file_local_missing = _file_local.replace('.txt', '_missing.txt')

          try:

            _file_remote        = open( _file_remote, 'r')
            _file_local         = open( _file_local, 'w')
            _file_local_missing = open( _file_local_missing, 'w')

            _protocol                 = self.remote_locations['protocol'][_l]
            _path_to_be_replaced      = os.path.join( self.remote_locations['storage_element'][_l], self.remote_locations['path'][_l])
            _path_to_be_replaced_with = self.path_samples

            for _f in _file_remote:

              # Create local path for each file
              _lfn = _f.strip().replace( _path_to_be_replaced, _path_to_be_replaced_with).replace( _protocol, '')

              if os.path.isfile(_lfn):
                _file_local.write(_lfn + '\n')
              else:
                _file_local_missing.write(_lfn + '\n')

            _file_remote.close()
            _file_local.close()
            _file_local_missing.close()

          except Exception, e:
            Print('error', 'Error: {0}'.format(e))

  def copy_files_all_samples_locally(self):

    Print('python_info', '\nCalled copy_files_all_samples_locally function.')

    _batch_templates_copy      = self.batch_templates['copy']
    _path_batch_results_copy   = make_directory( self.path_batch_results, 'copy')
    _path_batch_templates_copy = make_directory( self.path_batch_templates, 'copy')

    # Loop over samples
    for _s in self.samples:

      Print('status', '\nSample: {0}'.format(_s))

      # Loop over all remote locations
      for _l in self.remote_locations['path']:

        # Loop over all subsamples
        for _ss in self.samples_info[_s]['subsample'].values():

          _file_missing = os.path.join( self.path_logical_file_names, 'local', _l, _s, _ss.replace('/', '__') + '_missing.txt')
          _file_missing = open( _file_missing, 'r')

          for _n, _f in enumerate(_file_missing):

            _f = _f.rstrip()

            # Number of jobs handler
            if _n >= self.number_of_jobs and not self.number_of_jobs == -1:
              continue

            if os.path.isfile(_f):
              Print('python_info','File {0} already exists.'.format(_f))
              continue

            # To create batch jobs, one needs to edit template script which is done below
            _path_remote_samples      = os.path.join( self.remote_locations['storage_element'][_l], self.remote_locations['path'][_l])

            _x                        = _f.split('/')                                                      # split input file into chunks wrt to '/'
            _source                   = _f.replace( self.path_samples, _path_remote_samples)               # place from where to copy
            _destination              = '/'.join(_x[:-1])                                                  # path where to copy
            _batch_file_wo_ext        = _x[-1].replace('.root','')                                         # file name without any extension
            _path_batch               = _destination.replace( self.path_samples,  _path_batch_results_copy)# path to newly created batch script
            _path_batch_file_wo_ext   = os.path.join( _path_batch, _batch_file_wo_ext)                     # path and name of the script without any extension
            _protocol                 = self.remote_locations['protocol'][_l]                              # protocol used for copying
            _path_proxy               = self.path_proxy                                                    # one needs proxy file to give permission to condor
            _file_proxy               = self.path_proxy.split('/')[-1]

            _batch_arguments = {
              '<path_batch>'            : _path_batch,
              '<path_batch_file_wo_ext>': _path_batch_file_wo_ext,
              '<source>'                : _source,
              '<destination>'           : _destination,
              '<protocol>'              : _protocol,
              '<X509_USER_PROXY_path>'  : _path_proxy,
              '<X509_USER_PROXY_file>'  : _file_proxy,
              '<SCRAM_ARCH>'            : os.environ['SCRAM_ARCH'],
              '<CMSSW_dir>'             : os.path.join( os.environ['CMSSW_BASE'], 'src'),
            }

            # Create batch tool instance
            _batch = BatchTool( _path_batch_templates_copy, _batch_templates_copy, _batch_arguments, self.batch_type)
            # Make scripts first
            _batch.make_scripts()
            # Send jobs
            if self.send_jobs:
              _batch.send_job()

          _file_missing.close()

  def check_missing_files_all_samples_locally(self):
    ''' Loop over missing file and remove any file which exists and it has error.'''

    Print('python_info', '\nCalled check_missing_files_all_samples_locally function.')

    _batch_templates_check     = self.batch_templates['check']
    _path_batch_results_check  = make_directory( self.path_batch_results, 'check')
    _path_batch_templates_check= make_directory( self.path_batch_templates, 'check')

    # Loop over samples
    for _s in self.samples:

      Print('status', '\nSample: {0}'.format(_s))

      # Loop over all remote locations
      for _l in self.remote_locations['path']:

        # Loop over all subsamples
        for _ss in self.samples_info[_s]['subsample'].values():

          _file_missing = os.path.join( self.path_logical_file_names, 'local', _l, _s, _ss.replace('/', '__') + '_missing.txt')
          _file_missing = open( _file_missing, 'r')

          for _f in _file_missing:

            _f = _f.rstrip()

            # Send check files on batch
            if self.send_jobs:

              _x                        = _f.split('/')                                                      # split input file into chunks wrt to '/'
              _batch_file_wo_ext        = _x[-1].replace('.root','')                                         # file name without any extension
              _destination              = '/'.join(_x[:-1])
              _path_batch               = _destination.replace( self.path_samples, _path_batch_results_check)# path to newly created batch script
              _path_batch_file_wo_ext   = os.path.join( _path_batch, _batch_file_wo_ext)                     # path and name of the script without any extension
              _path_proxy               = self.path_proxy                                                    # one needs proxy file to give permission to condor
              _file_proxy               = self.path_proxy.split('/')[-1]

              _batch_arguments = {
                '<path_batch>'            : _path_batch,
                '<path_batch_file_wo_ext>': _path_batch_file_wo_ext,
                '<file_name>'             : _f,
                '<X509_USER_PROXY_path>'  : _path_proxy,
                '<X509_USER_PROXY_file>'  : _file_proxy,
                '<SCRAM_ARCH>'            : os.environ['SCRAM_ARCH'],
                '<CMSSW_dir>'             : os.path.join( os.environ['CMSSW_BASE'], 'src'),
              }

              # Create batch tool instance
              _batch = BatchTool( _path_batch_templates_check, _batch_templates_check, _batch_arguments, self.batch_type)
              # Make scripts first
              _batch.make_scripts()
              # Send job
              _batch.send_job()

            else:

              if os.path.isfile(_f):

                if check_root_file(_f):
                  Print('status', 'File OK {0}'.format(_f))
                else:
                  Print('error', 'Error with {0}. Removing ...'.format(_f))
                  sp.check_output(['rm', _f])

          _file_missing.close()

