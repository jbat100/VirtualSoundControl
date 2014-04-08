#!/usr/local/bin/python

from pyPdf import PdfFileReader,PdfFileWriter
import argparse
import copy
    
def main():
    
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", type=str, help="The path to the input file")
    parser.add_argument("output_file", type=str, help="The path to the output file")
    parser.add_argument("-t", "--threshold", type=float, help="Minimum width for splitting")
    args = parser.parse_args()
    
    input_file = args.input_file
    output_file = args.output_file
    threshold = args.threshold
    
    if threshold == None:
        threshold = 0.0
    
    input_file_handle = open(input_file, 'rb')
    output_file_handle = open(output_file, 'wb')
    
    reader = PdfFileReader(input_file_handle)
    writer = PdfFileWriter()
    
    for i in range(reader.getNumPages()):
        p = reader.getPage(i)
        if p.mediaBox[2] >= threshold:
            q = copy.copy(p)
            q.mediaBox = copy.copy(p.mediaBox)
            bl = p.mediaBox.lowerLeft
            ur = p.mediaBox.upperRight
            p.mediaBox.upperRight = (ur[0]/2, ur[1])
            p.mediaBox.lowerLeft = bl
            q.mediaBox.upperRight = ur
            q.mediaBox.lowerLeft = (ur[0]/2, bl[1])
            p.cropBox = p.mediaBox
            q.cropBox = q.mediaBox
            p.trimBox = p.mediaBox
            q.trimBox = q.mediaBox
            p.compressContentStreams()
            q.compressContentStreams()
            writer.addPage(p)
            writer.addPage(q)
        else:
            writer.addPage(p)
    
    writer.write(output_file_handle)
    
    input_file_handle.close()
    output_file_handle.close()
    
    # run: gs -o SplitSan.pdf -dTextFormat=3 -sDEVICE=pdfwrite -f Split.pdf (TO SANITIZE)

    
if __name__ == "__main__":
    main()