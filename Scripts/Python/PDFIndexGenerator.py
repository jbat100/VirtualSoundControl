#!/usr/local/bin/python

from pyPdf import PdfFileReader
import re
import argparse
from unidecode import unidecode

def pre_process_string_for_search(s):
    
    if isinstance(s, unicode) == False:
        su = unicode(s, "UTF-8")
    else:
        su = s
    sud = unidecode(su)
    
    return sud

def main():
    
    
    parser = argparse.ArgumentParser()
    parser.add_argument("pdf_file", type=str, help="The path to the pdf file to process")
    parser.add_argument("words_file", type=str, help="The path to a file containing the words to count")
    parser.add_argument("output_file", type=str, help="The path to a file for writing the results to")
    parser.add_argument("-p", "--page", type=int, help="Pdf page number of page 1)")
    args = parser.parse_args()
    
    input_file = args.pdf_file
    output_file = args.output_file
    words_file = args.words_file
    pdf_page = args.page # so that it doesn't go down to 0 but 1
    
    if pdf_page == None:
        offset = 0
    else:
        offset = 1-pdf_page
        
    
    words_file_handle = open(words_file, 'r')
    words_string = words_file_handle.read()
    
    #print "words_string: ", words_string
    
    words_string = words_string.replace('\r\n', '\n')
    words_string = words_string.replace('\r', '\n')
    
    temp_words = words_string.split('\n')
    words = [word for word in temp_words if len(word) > 0]
    
    print "Will look for: "
    for word in words:
        print "    ", word
    
    pdf = PdfFileReader(file(input_file, "rb"))
    pageTextStrings = [None] * pdf.getNumPages()
    
    output_file_handle = open(output_file, 'w')
    
    print "Reading in data..."
    
    for pageNumber in range(0, pdf.getNumPages()):
        page = pdf.getPage(pageNumber)
        text = page.extractText()
        #print "Page ", pageNumber, ": ", len(text) 
        pageTextStrings[pageNumber] = pre_process_string_for_search(text)
        
    for word in words:
        #print "Looking for '", word, "' ..."
        #pattern = re.compile(word, re.IGNORECASE)
        pword = pre_process_string_for_search(word)
        pattern = re.compile(pword, re.IGNORECASE | re.UNICODE | re.LOCALE)
        output_file_handle.write(word)
        foundSomething = False
        total = 0
        for pageNumber in range(0, pdf.getNumPages()):
            pageTextString = pageTextStrings[pageNumber]
            matches = re.findall(pattern, pageTextString)
            count = len(matches)
            if count > 0:
                foundSomething = True
                total += count
                #print "Found ", count, " instances on pdf page ", pageNumber+1, " real page (", pageNumber+offset+1, ")"
                output_file_handle.write(', %r(%r)'%(pageNumber+offset+1, count))
        if foundSomething == False:
            output_file_handle.write(' <None>')
        else:
            output_file_handle.write(', total: %r'%total)
        print "Found %r instances of: "%(total), word
        output_file_handle.write('\n\n');
        
    output_file_handle.close()

if __name__ == "__main__":
    main()
    