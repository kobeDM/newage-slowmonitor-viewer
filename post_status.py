#! /usr/local/bin/python3
import subprocess
file="/home/msgc/slow_monitor/newage-slowmonitor-viewer/token_202112"
with open(file)as f:
    token=f.read()
#token= '"'"Authorization: Bearer "+ token+'"'
#print(token)
com="curl -F file=@/home/msgc/status_tmp/status_up.png -F "+'"'+"initial_comment=NEWAGE 0.3b status :smile:"+'"'+" -F channels=C02CS4T4SH2 -H"+ '"'+"Authorization: Bearer "+ token+'"'+" https://slack.com/api/files.upload"
print(com)
subprocess.run(com,shell=True)
