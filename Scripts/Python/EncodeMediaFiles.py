#!/usr/local/bin/python2.7
# encoding: utf-8
'''
 -- Encodes media files in a directory tree 
'''

import sys
import os
import vsc.media.encode
import argparse




def main(argv=None): # IGNORE:C0111
    '''Command line options.'''
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input_directory", help="Base directory", type=str, required=True)
    parser.add_argument("-r", "--recursive", action="store_true", help="Recurse into sub-directories")
    parser.add_argument("-s", "--suffix", default='_pyencoded', type=str, help="Suffix to add to the filenames for output files")
    parser.add_argument("-e", "--encoding", default='Normal', type=str, help="HandbrakeCLI encoding preset to use")
    
    args = parser.parse_args()
    
    for root, dirs, files in os.walk(args.input_directory):
        #level = root.replace(startpath, '').count(os.sep)
        #indent = ' ' * 4 * (level)
        #print('{}{}/'.format(indent, os.path.basename(root)))
        #subindent = ' ' * 4 * (level + 1)
        for f in files:
            #print('{}{}'.format(subindent, f))
            #vsc.media.EncodeMedia()
            fullInputFile = os.path.join(root, f)
            filePathNoExt, fileExt = os.path.splitext(fullInputFile)
            outputFilePath = filePathNoExt + args.suffix + fileExt
            print "\n-----------------------------------------\n"
            print "Encoding file " + fullInputFile + " to " + outputFilePath 
            vsc.media.encode.EncodeMedia(fullInputFile, outputFilePath, args.encoding)

if __name__ == "__main__":
    main()