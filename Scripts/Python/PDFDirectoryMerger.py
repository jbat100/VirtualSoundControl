#!/usr/local/bin/python

import argparse
from os import listdir
from os.path import isfile, join, splitext
import vsc.pdf.tools
    
import re

def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    '''
    alist.sort(key=natural_keys) sorts in human order
    http://nedbatchelder.com/blog/200712/human_sorting.html
    (See Toothy's implementation in the comments)
    '''
    return [ atoi(c) for c in re.split('(\d+)', text.lower()) ]

def main():
    
    parser = argparse.ArgumentParser()
    parser.add_argument("output_file", type=str, help="The path to the output file")
    parser.add_argument("input_directory", type=str, help="The path to the input directory")
    args = parser.parse_args()
    
    input_dir = args.input_directory
    input_files = [] 

    for f in listdir(input_dir):
        p = join(input_dir, f)
        if isfile(p):
            n, e = splitext(f)
            if e.lower() == '.pdf':
                input_files.append(p)

    input_files.sort(key=natural_keys)
            
    output_file = args.output_file
    
    vsc.pdf.tools.merge_pdf_files(input_files, output_file)
    
if __name__ == "__main__":
    main()
