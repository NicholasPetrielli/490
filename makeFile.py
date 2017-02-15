import sys
import os


def createFile(nhl = False,nba = False,nfl = False,news = False,weather = False,alarm = False,memo = False):
	os.system(">whatToRun")
	f = open("whatToRun",'w')
	if nhl:
		f.write("nhlScore \n")
	if nba:
                f.write("nbaScore \n")	
	if nfl:
                f.write("nflScore \n")
        if news:
                f.write("news.txt \n")
        if weather:
                f.write("weather.txt \n")
        if alarm:
                f.write("alarm.txt \n")
        if memo:
                f.write("memo.txt \n")
	f.close()	

