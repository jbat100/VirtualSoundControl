
import subprocess

def EncodeMedia(sourcePath, destinationPath, encoding='Normal'):
    
    # TODO: file checks (existing, file type) 
    #print "Encoding file at " + sourcePath + " to path " + destinationPath + " using encoding: " + encoding
    
    commandLine = ["HandBrakeCLI", "-i", sourcePath, "-o", destinationPath, "--preset=\"" + encoding + "\""]
    
    print str(commandLine)
    
    subprocess.check_call(commandLine)
    
    