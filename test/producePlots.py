from RecoBTag.BTagValidation.BatchTool import *
from RecoBTag.BTagValidation.FileTool import *
from RecoBTag.BTagValidation.HistogramTool import *
from RecoBTag.BTagValidation.MergeTool import *
from RecoBTag.BTagValidation.MiscTool import *

import config

import argparse
parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('job', metavar='N', type=int, nargs=1, help='an integer for the job')
parser.add_argument('-b', '--batch', action='store_true', default=False, help="Don't print batch submit info")
parser.add_argument('-s', '--silent', action='store_true', default=False, help="Don't print batch submit info")
args = parser.parse_args()
args.silent = (args.batch or args.silent)

if __name__ == '__main__':

  # Choose which job you want to execute
  #job = int(setup_job())
  job = args.job[0]

  # Initalize handlers for each step
  file_tool       = FileTool(config)
  histogram_tool  = HistogramTool(config)
  merge_tool      = MergeTool(config)

  if job == 1:
    file_tool.save_logical_file_names_all_samples_remotely()

  elif job == 2:
    file_tool.check_missing_files_all_samples_remotely()

  elif job == 3:
    file_tool.save_logical_file_names_all_samples_locally()

  elif job == 4:
    file_tool.copy_files_all_samples_locally()

  elif job == 5:
    file_tool.check_missing_files_all_samples_locally()

  elif job == 6:
    histogram_tool.make_and_send_jobs(silent=args.silent)

  elif job == 7:
    merge_tool.merge_histograms()

  elif job == 8:
    merge_tool.merge_datasets()

  else:
    pass
