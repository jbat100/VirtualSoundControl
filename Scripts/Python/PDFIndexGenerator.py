#!/usr/local/bin/python

from pyPdf import PdfFileReader
import re
import argparse
import sys
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
    parser.add_argument("-p", "--page", type=int, help="Pdf page number of page 1")
    parser.add_argument("-b", "--brief", action="store_true", default=False, help="Don't write number of instances or total numbers")
    args = parser.parse_args()
    
    input_file = args.pdf_file
    output_file = args.output_file
    words_file = args.words_file
    brief = args.brief
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
    unstripped_words = [word for word in temp_words if len(word) > 0]
    words = []
    
    print "Will look for: "
    for word in unstripped_words:
        words.append(word.strip())
        print "    ", word
    
    pdf = PdfFileReader(file(input_file, "rb"))
    pageTextStrings = [None] * pdf.getNumPages()
    
    output_file_handle = open(output_file, 'w')
    
    sys.stdout.write("Reading in data... [0/%r]"%(pdf.getNumPages()))
    sys.stdout.write('\r')
    sys.stdout.flush()
    count = 0
    for pageNumber in range(0, pdf.getNumPages()):
        count += 1
        if count%10 == 0:
            sys.stdout.write("Reading in data... [%r/%r]"%(count, pdf.getNumPages()))
            sys.stdout.write('\r')
            sys.stdout.flush()
        page = pdf.getPage(pageNumber)
        text = page.extractText()
        #print "Page ", pageNumber, ": ", len(text) 
        pageTextStrings[pageNumber] = pre_process_string_for_search(text)
        if pageNumber == 60:
            print pageTextStrings[pageNumber]
    
    print "Read in data"
    
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
            if pageNumber == 60:
                #print "Big page..."
                pass
            matches = re.findall(pattern, pageTextString)
            count = len(matches)
            if count > 0:
                foundSomething = True
                total += count
                #print "Found ", count, " instances on pdf page ", pageNumber+1, " real page (", pageNumber+offset+1, ")"
                output_file_handle.write(', %r'%(pageNumber+offset+1))
                if brief == False:
                    output_file_handle.write('(%r)'%(count))
        if foundSomething == False:
            output_file_handle.write(' <None>')
        elif brief == False:
            output_file_handle.write(', total: %r'%total)
        print "Found %r instances of: "%(total), word
        output_file_handle.write('\n\n');
        
    output_file_handle.close()

if __name__ == "__main__":
    main()
    