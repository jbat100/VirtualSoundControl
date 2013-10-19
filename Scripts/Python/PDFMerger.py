#!/usr/local/bin/python

from pyPdf import PdfFileReader,PdfFileWriter
import argparse

def merge_pdf_files(input_files, output_file):
    
    output_file_writer = PdfFileWriter()
    
    input_file_handles = []

    for input_file in input_files:
        print "Merging: ", input_file, "..."
        input_file_handle = open(input_file, 'rb')
        input_file_reader = PdfFileReader(input_file_handle)
        for page in input_file_reader.pages:
            output_file_writer.addPage(page)
        input_file_handles.append(input_file_handle)

    print "Writing output to: ", output_file, "..."

    output_file_handle = open(output_file, mode='wb')
    output_file_writer.write(output_file_handle)
    output_file_handle.close()
    
    # it seems we need to wait for the output operation to complete before closing the input files
    
    for input_file_handle in input_file_handles:
        input_file_handle.close()
        
    print "Done!"
    
def main():
    
    parser = argparse.ArgumentParser()
    parser.add_argument("output_file", type=str, help="The path to the output file")
    parser.add_argument("input_files", type=str, nargs='+', help="The paths to the input files in order")
    args = parser.parse_args()
    
    input_files = args.input_files
    output_file = args.output_file
    
    merge_pdf_files(input_files, output_file)
    
if __name__ == "__main__":
    main()
