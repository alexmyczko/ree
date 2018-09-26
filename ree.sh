#! /bin/sh

# rom extension extractor (ported to shell from the pascal version of 1997)
# see http://www.firmware.com/support/bios/romext.htm for more info
# extracts system, scsi, video bios
# alex@aiei.ch

# scan from c0000 - f0000 in 512 byte blocks
# Uª{l}{code}
# l*512=length of the code including headers

# 200 hex = 512, dd wants 512 blocks
case "$@" in
    -nbc|--i-dont-have-no-bc)
	start=1536
        last=1920
    ;;
    *)
	start=$(echo "ibase=16;C0000/200"|bc)
        last=$(echo "ibase=16;F0000/200"|bc)
    ;;
esac

for a in $(seq $start 1 $last); do
    b=$(dd if=/dev/mem skip=$a count=1 2>/dev/null |head -c2)

    case "$@" in
        -nbc|--i-dont-have-no-bc)
	    location=$[a*512]
        ;;
        *)
	    location=$(echo "obase=16; $a*512"|bc)
	;;
    esac

    echo -ne "\033[1GPlease wait, scanning... $location"

    if [ "$b" = "Uª" ]; then
	size=$(dd if=/dev/mem skip=$a count=1 2>/dev/null |tail -c+3)
	s=$(echo $size |perl -e 'print ord(<>);')
	
	case "$@" in
	    -nbc|--i-dont-have-no-bc)
		file=$[a*512]
		size=$[s*512]
            ;;
	    *)
	        file=$(echo "obase=16; $a*512"|bc)
		size=$(echo $s*512 |bc)
	    ;;
	esac
	
	echo -e "\nFound something at $file ($size)"
	dd if=/dev/mem of=${file}.rom skip=$a count=$s 2>/dev/null
	strings ${file}.rom |awk '{if (length($0)>40)print $0}' |grep "[A-Z]"
    fi
done

#save bios rom
#case "$@" in
#    -nbc|--i-dont-have-no-bc)
#	start=1920
#	count=128
#    ;;
#    *)
#	start=`echo "ibase=16;F0000/200"|bc`
#        count=`echo 1*65536/512|bc`
#    ;;
#esac
# echo -e "\nFound system bios at F0000 (65536)"
# dd if=/dev/mem of=F0000.rom skip=$start count=$count 2>/dev/null
