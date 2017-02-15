#from flask import Flask
#import argparse
#import warnings
#import datetime
#import imutils
import json
#import time
#import cv2
import urllib2
import os
import sys
from espnbottomline import get_scores

#Data=json.load(urllib2.urlopen("https://hidden-hollows-61576.herokuapp.com/alarm"))

def write_scores():
	leagues = ["ncf","nfl","mlb","nba","nhl","mens-college-basketball"]
	for i in range(1, len(leagues)):
		Scores = {}
		Scores = get_scores(leagues[i])
		fileName = str(leagues[i]) + "Score"
		os.system("> " + fileName)
		f = open (fileName, 'w')
		counter = 1
		f.write("Listing " +" ".join(leagues[i]) + " scores:\n")
		for key in Scores:  
			f.write("Game: " + str(counter) +   " " + str(Scores[key][0]) + " " + str(Scores[key][1]) + " " + str(Scores[key][2]) + " " + str(Scores[key][3]) + " . Time: " + str(Scores[key][4]) + "\n")
			counter+= 1
		if (key in Scores) == 0:
			os.system("rm " + fileName)
		f.write("\n")
		f.close()
def write_news():
	newsData = json.load(urllib2.urlopen("https://newsapi.org/v1/articles?source=time&sortBy=top&apiKey=29764463ac4e41e4b2b0aadd6cf5f9c5"))
	#print newsData["articles"][1]["description"]
	os.system(">news.txt")
	f = open("news.txt",'w')
	for i in range(1, len(newsData["articles"])):
		f.write(((newsData["articles"][i]["title"]) + " " + (newsData["articles"][i]["description"]) + " \n").encode('utf-8'))
		if i+1 != len(newsData["articles"]):
			f.write("Next headline \n")
		


write_news()
write_scores()
