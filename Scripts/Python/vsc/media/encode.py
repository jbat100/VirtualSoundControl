
import subprocess

def EncodeMedia(source_path, destination_path, encoding='Normal'):
    if encoding == 'Normal':
        command_line = ["HandBrakeCLI", "-i", source_path, "-o", destination_path, "--preset=\"" + encoding + "\""]
    elif encoding == 'Mobile':
        command_line = ['ffmpeg', '-i', source_path, '-c:v', 'copy', '-c:a', 'libfdk_aac', '-b:a', '256k', destination_path]
    elif encoding == 'Audio':
        command_line = ["ffmpeg", "-i", source_path, "-threads",  "0", "-vn", "-ab", '256k', destination_path]
    print str(command_line)
    try:
        subprocess.check_call(command_line)
    except subprocess.CalledProcessError, e:
        print 'Error while processing ', source_path, ' : ', e