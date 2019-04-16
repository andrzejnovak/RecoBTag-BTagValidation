import os
import subprocess as sp
import stat

from RecoBTag.BTagValidation.MiscTool import *

class BatchTool(object):
  '''
  Load existing batch templates, adjust and saves them, send job
  '''
  def __init__(self, path_templates, templates, arguments, batch, queue=None):

    self.batch          = batch
    self.path_templates = path_templates
    self.templates      = templates
    self.arguments      = arguments
    self.queue          = queue

    # Add aditional layer of directories for each job
    _new_path_batch                             = self.arguments['<path_batch_file_wo_ext>']
    self.arguments['<path_batch_file_wo_ext>']  = _new_path_batch.replace( self.arguments['<path_batch>'], _new_path_batch)
    self.arguments['<path_batch>']              = _new_path_batch

    # make batch directory
    make_directory(self.arguments['<path_batch>'])

  def make_scripts(self):

    # Loop over all the templates
    for _t in self.templates:

      # Open template
      _template = os.path.join( self.path_templates, _t)
      with open( _template, 'r') as _f:
        _template = _f.read()

      # make loop through all arguments and replace them
      for _arg, value in self.arguments.iteritems():
        _template = _template.replace( str(_arg), str(value))

      # Save new script
      _script = self.arguments['<path_batch_file_wo_ext>'] + '.' + _t.split('.')[-1]

      with open( _script, 'w') as _f:
        _f.write( _template)

  def send_job(self, silent=False):

    # Change permission so that it can be executed
    _sh = self.arguments['<path_batch_file_wo_ext>'] + '.sh'
    os.chmod(_sh, os.stat(_sh).st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)

    # Needed so that LSF output is stored in the same directory as job itself
    os.chdir(self.arguments['<path_batch>'])

    if self.batch == 'condor':
      _txt      = self.arguments['<path_batch_file_wo_ext>'] + '.txt'
      _command  = ['condor_submit', _txt]
      if not silent: Print('python_info','{0}'.format(' '.join(_command)))
      sp.call(' '.join(_command), shell=True)

    elif self.batch == 'lxbatch':
      _command  = ['bsub', '-q', self.queue, '-J', self.arguments['<path_batch_file_wo_ext>'], '<', _sh]
      if not silent: Print('python_info','{0}'.format(' '.join(_command)))
      sp.call( ' '.join(_command), shell=True)
