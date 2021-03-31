#! /bin/sh
DIR=$HOME/slow_monitor/newage-slowmonitor-viewer
WORK_DIR=$HOME/status_tmp
EXE=${DIR}"/monitor"
#AUTOSENDER=autosender.sh
AUTOSENDER=NSMsender.sh

CONFIG_SKELTON="monitor_skelton.cfg"
CONFIG_SKELTON_ORG=${DIR}"/monitor_skelton.cfg"
CONFIGCN10="monitor_skelton.cfg"
CONFIGCN10_ORG=${DIR}"/monitor_slelton.cfg"
#CONFIGLiqCF4="monitor_LiqCF4_na13.cfg"
CONFIGLiqCF4_ORG=${DIR}"/monitor_LiqCF4_na13.cfg"
CONFIG03a="monitor_03a_na16.cfg"
CONFIG03a_ORG=${DIR}"/monitor_03a_na16.cfg"
CONFIG03b="monitor_03b_na2.cfg"
CONFIG03b_ORG=${DIR}"/monitor_03b_na2.cfg"

#CONFIG=$CONFIG_SKELTON
#CONFIG_ORG=$CONFIG_SKELTON_ORG
CONFIG=$CONFIG03b
CONFIG_ORG=$CONFIG03b_ORG


echo "SMviewer.sh [YYYYMMDD [hh:mm:ss]] [YYYYYMMDD [hh:mm:ss]]"

cd $WORK_DIR
if [ -e $CONFIG ]; then 
echo $CONFIG  exists. Using these parameters.
else
echo $CONFIG  does not exit. Cprying from ${DIR}. 
cp $CONFIG_ORG $CONFIG
fi
echo Edit ${WORK_DIR}/${CONFIG} and reload the monitor to change the settings.

if [ `ps -a | grep $AUTOSENDER | wc -l ` -ge 1  ]; then
echo $AUTOSENDER is running.
else 
echo "starting autosender."
$HOME/bin/$AUTOSENDER &
fi



if [ $# -ge 3 ] ;then
#xterm -exe $EXE $CONFIG03a $1 $2 $3 $4 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+620 &
xterm -e $EXE $CONFIG $1 $2 $3 $4 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+50 &
#xterm -e $EXE $CONFIGLiqCF4 $1 $2 $3 $4 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+50 &
elif [ $# -ge 1 ] ;then
#xterm -exe $EXE $CONFIG03a $1 $2 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+620 &
xterm -e $EXE $CONFIG $1 $2 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+50 &
#xterm -e $EXE $CONFIG0LiqCF4 $1 $2 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+50 &
else
#xterm -exe $EXE $CONFIG03a -bg black -fg yellow -title '48h monitor' -geo 50x10+50+450 &
#xterm -exe $EXE $CONFIG03a 0 -bg black -fg orange -title 'updating monitor ver'${VER} -geo 50x10+50+620 &
xterm -e $EXE $CONFIG &
    #-bg black -fg yellow -title '48h monitor' -geo 50x10+50+220 &
xterm -e $EXE $CONFIG 0 &
#-bg black -fg orange -title 'updating monitor ver'${VER} -geo 50x10+50+50 &
#xterm -e $EXE $CONFIGLiqCF4 0 -bg black -fg orange -title 'updating monitor ver'${VER} -geo 50x10+50+620 &
#xterm -e $EXE $CONFIGLiqCF4 -bg black -fg yellow -title '48h monitor' -geo 50x10+50+220 &
fi
