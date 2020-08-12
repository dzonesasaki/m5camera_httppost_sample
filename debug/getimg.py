import requests

urlSite='http://192.168.1.2:1880' #host
urlGet='/get'
urlParam=''

fnames='myimg.jpg'

myUrl = urlSite + urlGet + urlParam

objGet = requests.get(myUrl)

with open(fnames,'wb') as fid:
	fid.write(objGet.content)
