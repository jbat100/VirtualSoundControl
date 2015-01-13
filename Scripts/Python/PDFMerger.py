#!/usr/local/bin/python

import argparse
import vsc.pdf.tools
    
def main():
    
    parser = argparse.ArgumentParser()
    parser.add_argument("output_file", type=str, help="The path to the output file")
    parser.add_argument("input_files", type=str, nargs='+', help="The paths to the input files in order")
    args = parser.parse_args()
    
    input_files = args.input_files
    output_file = args.output_file
    
    vsc.pdf.tools.merge_pdf_files(input_files, output_file)
    
if __name__ == "__main__":
    main()
