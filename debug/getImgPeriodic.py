import requests
import datetime
import threading

urlSite='http://192.168.1.1:1880' #hostname
urlGet='/get'
urlParam=''
fnames='myimg.jpg'
fnameshead='myimg0_'
fnamesExt='.jpg'

WAIT_TIME_SEC = 2

myUrl = urlSite + urlGet+urlParam

def mygets(myUrl,fnameshead,fnamesExt):
	strIndx=datetime.datetime.today().strftime('%Y%m%d_%H%M%S')
	fnames = fnameshead+strIndx+fnamesExt

	with open(fnames,'wb') as fid:
		fid.write(requests.get(myUrl).content)
	
ticker = threading.Event()
while not ticker.wait(WAIT_TIME_SEC):
	mygets(myUrl,fnameshead,fnamesExt)
