#! /bin/sh
VER="4.7"
#DIR="/home/msgc/src/monitor/"${VER}
DIR="/home/msgc/slow_monitor/monitor/"${VER}
EXE=${DIR}"/monitor"
#CONFIG03b="monitor_03b.cfg"
CONFIG03b_ORG=${DIR}"/monitor_03b.cfg"
#CONFIGLiqCF4="monitor_LiqCF4_na13.cfg"
CONFIGLiqCF4_ORG=${DIR}"/monitor_LiqCF4_na13.cfg"
CONFIG03a="monitor_03a_na16.cfg"
CONFIG03a_ORG=${DIR}"/monitor_03a_na16.cfg"
CONFIG03b="monitor_03b_na2.cfg"
CONFIG03b_ORG=${DIR}"/monitor_03b_na2.cfg"
echo "monitor.sh [YYYYMMDD [hh:mm:ss]] [YYYYYMMDD [hh:mm:ss]]"

#if [ -e $CONFIGLiqCF4 ]; then
#echo $CONFIGLiqCF4  exists. Using these parameters.
#else
#echo $CONFIGLiqCF4  does not exit. Cprying from ${DIR}.
#cp $CONFIGLiqCF4_ORG $CONFIG03a
#fi
#echo Edit ${CONFIGLiqCF4} and reload the monitor to change the settings.

#if [ -e $CONFIG03a ]; then
#echo $CONFIG03a  exists. Using these parameters.
#else
#echo $CONFIG03a  does not exit. Cprying from ${DIR}.
#cp $CONFIG03a_ORG $CONFIG03a
#fi
#echo Edit ${CONFIG03a} and reload the monitor to change the settings.

if [ -e $CONFIG03b ]; then 
echo $CONFIG03b  exists. Using these parameters.
else
echo $CONFIG03b  does not exit. Cprying from ${DIR}. 
cp $CONFIG03b_ORG $CONFIG03b
fi
echo Edit ${CONFIG03b} and reload the monitor to change the settings.
 
if [ $# -ge 3 ] ;then
#xterm -exe $EXE $CONFIG03a $1 $2 $3 $4 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+620 &
xterm -e $EXE $CONFIG03b $1 $2 $3 $4 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+50 &
#xterm -e $EXE $CONFIGLiqCF4 $1 $2 $3 $4 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+50 &
elif [ $# -ge 1 ] ;then
#xterm -exe $EXE $CONFIG03a $1 $2 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+620 &
xterm -e $EXE $CONFIG03b $1 $2 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+50 &
#xterm -e $EXE $CONFIG0LiqCF4 $1 $2 -bg black -fg yellow -title 'long-term monitor' -geo 50x10+50+50 &
else
#xterm -exe $EXE $CONFIG03a -bg black -fg yellow -title '48h monitor' -geo 50x10+50+450 &
#xterm -exe $EXE $CONFIG03a 0 -bg black -fg orange -title 'updating monitor ver'${VER} -geo 50x10+50+620 &
xterm -e $EXE $CONFIG03b &
    #-bg black -fg yellow -title '48h monitor' -geo 50x10+50+220 &
xterm -e $EXE $CONFIG03b 0 &
#-bg black -fg orange -title 'updating monitor ver'${VER} -geo 50x10+50+50 &
#xterm -e $EXE $CONFIGLiqCF4 0 -bg black -fg orange -title 'updating monitor ver'${VER} -geo 50x10+50+620 &
#xterm -e $EXE $CONFIGLiqCF4 -bg black -fg yellow -title '48h monitor' -geo 50x10+50+220 &
fi
