#!/usr/local/bin/python

from pyPdf import PdfFileReader,PdfFileWriter
import argparse
    
def main():
    
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", type=str, help="The path to the pdf file")
    args = parser.parse_args()
    
    input_file = args.input_file
    
    input_file_handle = open(input_file, 'rb')
    input_file_reader = PdfFileReader(input_file_handle)
    
    count = 1
    for page in input_file_reader.pages:
        print "Size of page %r : (%r : %r)"%(count, page.mediaBox[2], page.mediaBox[3])
        count += 1
        
    input_file_handle.close()
    
if __name__ == "__main__":
    main()