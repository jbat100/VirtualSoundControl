#!/usr/local/bin/python

from pyPdf import PdfFileReader,PdfFileWriter
import argparse
    
def main():
    
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", type=str, help="The path to the pdf file")
    parser.add_argument("-c", "--characters", action="store_true", default=False, help="Count page characters (Warning: slow)")
    args = parser.parse_args()
    
    input_file = args.input_file
    count_characters = args.characters
    
    input_file_handle = open(input_file, 'rb')
    input_file_reader = PdfFileReader(input_file_handle)
    
    count = 1
    for page in input_file_reader.pages:
        stat_string = "Size of page %r : (%r)"%(count, page.mediaBox)
        if count_characters == True:
            text = page.extractText()
            stat_string += ", %r characters"%(len(text))
        print stat_string
        count += 1
        
    input_file_handle.close()
    
if __name__ == "__main__":
    main()