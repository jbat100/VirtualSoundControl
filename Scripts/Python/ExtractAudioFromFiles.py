#!/usr/local/bin/python2.7
# encoding: utf-8
'''
 -- Encodes media files in a directory tree 
'''

import sys
import os
import vsc.media.encode
import argparse
import subprocess

def main(argv=None): # IGNORE:C0111
    '''Command line options.'''
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input_directory", help="Base directory", type=str, required=True)
    parser.add_argument("-r", "--recursive", action="store_true", help="Recurse into sub-directories")
    parser.add_argument("-p", "--preffix", default='audio_', type=str, help="Preffix for output files")
    
    args = parser.parse_args()
    
    for root, dirs, files in os.walk(args.input_directory):
        for f in files:
            input_file_path = os.path.join(root, f)
            file_path, file_name = os.path.split(input_file_path)
            file_name_no_ext, ext = os.path.splitext(file_name)
            output_file_name = args.preffix + file_name_no_ext + '.m4a'
            output_file_path = os.path.join(root, output_file_name)
            print "\n-----------------------------------------\n"
            print "Extracting audio from " + input_file_path + " to " + output_file_path 
            try:
                vsc.media.encode.ExtractAudio(input_file_path, output_file_path)
            except subprocess.CalledProcessError, e:
                print 'ERROR while processing ', input_file_path, ' ', e


if __name__ == "__main__":
    main()