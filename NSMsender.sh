#! /bin/sh
TO=newage@ppwww.phys.sci.kobe-u.ac.jp:/home/newage/www/kamioka/inbox/03b
STADIR="/home/msgc/status_tmp"
LOGDIR="/home/msgc/status"
MONFILE="status.png"
MONFILEUP="status_up.png"
IMFILE="/home/msgc/status_snapshot/NA03b-snapshot_cam01.jpg"
FILES="$STADIR/$MONFILE $STADIR/$MONFILEUP $IMFILE"

#echo $FILES

while [ 0 ]; do
    DIR=`date +%Y%m`
#    echo directory $DIR
#    LODIR=$STADIR/$DIR
    LODIR=$LOGDIR/$DIR
if [ ! -d $LODIR ]; then  mkdir $LODIR
fi
BUMONFILE=`date +%Y%m%d%H%M`.png
BUIMFILE=`date +%Y%m%d%H%M`.jpg
cp $STADIR/status_up.png ${LODIR}/${BUMONFILE}
cp $IMFILE ${LODIR}/${BUIMFILE}
scp $FILES $TO
#scp $INFILES $MONFILES $BUMONFILE $BUIMFILE $TO
sleep 10
done
