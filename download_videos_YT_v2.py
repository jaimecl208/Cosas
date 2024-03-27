from pytube import YouTube as YT
import sys
import subprocess
import argparse

def downloadFromURL(url, video_format):
    video = YT(url).streams.first()

    title = video.title
    new_title = title.replace(' ','')
    new_title_mp3 = f'{new_title}.mp3'
    new_title_mp4 = f'{new_title}.mp4'

    video.download(filename=new_title_mp4)

    if not video_format:
        subprocess.run(['ffmpeg', '-y','-i', new_title_mp4, new_title_mp3])
        subprocess.run(['rm',new_title_mp4])

def processURLS(urls, video, file):
    if not file:
        for url in urls:
            downloadFromURL(url, video)
    else:
        for filename in urls:
            f = open(filename, 'r')
            for url in f:
                downloadFromURL(url, video)
            f.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Download a video from given url or multiple urls and converts it to mp3 format by default")
    parser.add_argument('urls', metavar='urls', type=str, nargs='+', 
                        help="an url or set of urls from youtube video(s)")
    parser.add_argument('-v', '--video', dest='video', action='store_true', default=False,
                        help="leaves the video in .mp4 format")
    parser.add_argument('-f', '--file', dest='file', action='store_true', default=False,
                        help="if this option is selected, then the urls argument will be treated as a file which contains the url(s)")
    args = parser.parse_args()

    processURLS(args.urls, args.video, args.file)
    
