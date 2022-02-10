#!/bin/bash

POSIXLY_CORRECT=y

optstring=":h"

pomoc(){
	#vypisat pomoc
	echo "====================PRIKAZY:================================="
	echo "   list-tick pre vypis burzovnych tickerov                   "
	echo "   profit pre profit                                         "
	echo "   pos pre vypis hodnot aktualnych pozicii                   "
	echo "   last-price vypis poslednej znamej ceny pre kazdy ticker   "
	echo "   hist-ord pre vypis histogramu poctu tranzakcii            "
	echo "   graph-pos vypis grafu drzanych pozicii podla tickeru      "
}

tick_list(){
	declare -a TICKS_ARR
	FILE=$1
	if [ $# -ge 2 ]
	then
		arr=("$@")
	fi
	while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
	do
		if [[ ! " ${TICKS_ARR[@]} " =~ " ${TICKER} " ]]
		then
			TICKS_ARR+=($TICKER)
			if [ $# -ge 2 ]
			then
				for i in ${arr[*]}
				do
					echo $TICKER | grep $i
				done
			else
				echo $TICKER
			fi
		fi
	done < $FILE | sort
}

profit(){
	CELKOVY_PROFIT=0
	
	while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
	do
		if [[ "$TYP_TRANZAKCIE" = "buy" ]]
		then
			CELKOVY_PROFIT=$(awk "BEGIN {printf \"%.2f\",${CELKOVY_PROFIT}-${JC}*${OBJEM}}")
		else
			CELKOVY_PROFIT=$(awk "BEGIN {printf \"%.2f\",${CELKOVY_PROFIT}+${JC}*${OBJEM}}")
		fi
	done < $1

	echo $CELKOVY_PROFIT
}

pos(){
	declare -A TICKERS
	while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
	do
		if [[ ! " ${TICKERS[@]} " =~ " ${TICKER} " ]]
    then
			TICKERS["$TICKER"]=0
		fi
	done < $1
	
	while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
	do
		if [[ "$TYP_TRANZAKCIE" = buy ]]
		then
			TICKERS["$TICKER"]=$(awk "BEGIN {printf \"%.2f\",${TICKERS["$TICKER"]}+${JC}*${OBJEM}}")
		else
			TICKERS["$TICKER"]=$(awk "BEGIN {printf \"%.2f\",${TICKERS["$TICKER"]}-${JC}*${OBJEM}}")
		fi
	done < $1

	for K in "${!TICKERS[@]}"
	do
    echo $K ' : ' ${TICKERS["$K"]}
	done |
	sort -k1
}

last_price(){
	declare -A TICKERS
	while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
	do
		TICKERS["$TICKER"]=$JC
	done < $1
	
	for K in "${!TICKERS[@]}"
	do
		echo $K ' : ' ${TICKERS["$K"]}
	done |
	sort -k1
}

hist_ord(){
	declare -A TRANZAKCIE
	if [ $2 -lt 1 ]
	then
		declare -A HASHE
		while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
		do
			if [[ ! " ${TRANZAKCIE[@]} " =~ " ${TICKER} " ]]
    	then
				TRANZAKCIE["$TICKER"]=0
			fi
		done < $1

	
		while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
		do
				TRANZAKCIE["$TICKER"]=$((${TRANZAKCIE["$TICKER"]}+1)) 
		done < $1

		for K in "${!TRANZAKCIE[@]}"
		do
			for (( E = 0; E < ${TRANZAKCIE["$K"]}; E++ ))
			do
				HASHE["$K"]="${HASHE["$K"]}#"
			done
		done
	
		for Y in "${!HASHE[@]}"
		do
			echo $Y ' : ' ${HASHE["$Y"]}
		done | sort -k1
	else
		COLS=$2
		MAX_HIST_POS=`eval printf '\#%.0s' {1..$COLS}; echo;`
		MAX_HIST_NEG=`eval printf '\!%.0s' {1..$COLS}; echo;`

		while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
			do
				if [[ ! " ${TRANZAKCIE[@]} " =~ " ${TICKER} " ]]
    		then
					TRANZAKCIE["$TICKER"]=0
				fi
			done < $1
	
		while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
		do
				TRANZAKCIE["$TICKER"]=$((${TRANZAKCIE["$TICKER"]}+1)) 
		done < $1

		HIG_VALUE=0

		for TICKER in ${!TRANZAKCIE[@]}
		do
			if [ ${TRANZAKCIE["$TICKER"]} -gt $HIG_VALUE ]
			then
				HIG_VALUE=${TRANZAKCIE["$TICKER"]}
			fi
		done

		for TICKER in ${!TRANZAKCIE[@]}
		do
			TMP=$((${TRANZAKCIE["$TICKER"]}*100/$HIG_VALUE))
			echo $TICKER ' : ' ${MAX_HIST_POS:0:$TMP}
		done | sort -k1
	fi
}

graph_pos(){
	declare -A TICKERS
	if [ $2 -lt 1 ]
	then
		while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
		do
			if [[ ! " ${TICKERS[@]} " =~ " ${TICKER} " ]]
    	then
				TICKERS["$TICKER"]=0
			fi
		done < $1
	
		while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
		do
			if [[ "$TYP_TRANZAKCIE" = buy ]]
			then
				TICKERS["$TICKER"]=$(awk "BEGIN {printf \"%.0f\",${TICKERS["$TICKER"]}+${JC}*${OBJEM}}")
			else
				TICKERS["$TICKER"]=$(awk "BEGIN {printf \"%.0f\",${TICKERS["$TICKER"]}-${JC}*${OBJEM}}")
			fi
		done < $1

		for TICKER in "${!TICKERS[@]}"
		do
			if [ ${TICKERS["$TICKER"]} -gt 0 ]
			then
				TMP="#"
				for (( i=0; i < $((${TICKERS["$TICKER"]}/1000)); i++ ))
				do
					TMP="${TMP}#"
				done
			else
				TICKERS["$TICKER"]=$((${TICKERS["$TICKER"]}*-1))	
				TMP="!"
				for ((  i=0; i < $((${TICKERS["$TICKER"]}/1000)); i++ ))
				do
					TMP="${TMP}!"
				done			
			fi
				echo $TICKER ' : ' $TMP
		done

	else
		COLS=$2
		MAX_HIST_POS=`eval printf '\#%.0s' {1..$COLS}; echo;`
		MAX_HIST_NEG=`eval printf '\!%.0s' {1..$COLS}; echo;`

		while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
			do
				if [[ ! " ${TICKERS[@]} " =~ " ${TICKER} " ]]
    		then
					TICKERS["$TICKER"]=0
				fi
			done < $1
	
			while read DAC TICKER TYP_TRANZAKCIE JC MENA OBJEM ID
			do
				if [[ "$TYP_TRANZAKCIE" = buy ]]
				then
					TICKERS["$TICKER"]=$(awk "BEGIN {printf \"%.0f\",${TICKERS["$TICKER"]}+${JC}*${OBJEM}}")
				else
					TICKERS["$TICKER"]=$(awk "BEGIN {printf \"%.0f\",${TICKERS["$TICKER"]}-${JC}*${OBJEM}}")
				fi
			done < $1

			HIG_VALUE=0

			for TICKER in ${!TICKERS[@]}
			do
				if [ ${TICKERS["$TICKER"]} -gt 0 ]
				then
					if [ ${TICKERS["$TICKER"]} -gt $HIG_VALUE ]
					then
						HIG_VALUE=${TICKERS["$TICKER"]}
					fi
				else
					echo $((${TICKERS["$TICKER"]}*-1))
					if [ $((${TICKERS["$TICKER"]}*-1)) -gt $HIG_VALUE ]
					then
						HIG_VALUE=$((${TICKERS["$TICKER"]}*-1))
					fi
				fi
			done

			for TICKER in ${!TICKERS[@]}
			do
				if [ ${TICKERS["$TICKER"]} -gt 0 ]
				then
					TMP=$((${TICKERS["$TICKER"]}*100/$HIG_VALUE))
					echo $TICKER ' : ' ${MAX_HIST_POS:0:$TMP}
				else
					TMP=$((${TICKERS["$TICKER"]}*-100/$HIG_VALUE))
					echo $TICKER ' : ' ${MAX_HIST_NEG:0:$TMP}
				fi
			done | sort -k1
	fi
}
print_log(){
	IFS="\n"
	while read line
	do
		echo $line
	done < $1 | sort -t ';' -k2
}

OLDIFS=$IFS
IFS=";"
DATETIME="0000-00-00 00:00:00" #vytvaranie vars s defaultnymi values
PRED_ALEBO_PO=""
declare -a TICKERS
WIDTH=0
COMMAND="print-log"
FILE=""

for (( i=1; i<=$#; i++ )); do
	case ${!i} in		h)
      pomoc
			continue
    	;;
		-help)
			pomoc
			continue
			;;
		-a)
			#datum a cas po
			echo "WIP"
			continue
			;;
		-b)
			#datum a cas pred
			echo "WIP"
			continue
			;;
		-t)
			#filter na iste tickery
			TMP=$((${i}+1))
			TICKERS+=(${!TMP})
			continue
			;;
		-w)
			#sirka grafu, viac krat w je chyba
			if [ $WIDTH -eq 0 ]
			then
				TMP=$((${i}+1))
				WIDTH=${!TMP}
				continue
			else
				echo "Width treba zadat iba raz"
				exit 1
			fi
			;;
    :)
      #default
			continue
      ;;
    ?)
			if [ $OPTARG -eq $OPTARG ]
			then
				continue
			else
    		echo "Invalidny opt: -${OPTARG}."
				pomoc
      	exit 1
			fi
      ;;
  esac
done

for (( i=1; i<=$#; i++ )); do
	case ${!i} in
	 	list-tick)
        #vypis zoznamu tickerov
				COMMAND="tick-list"
				continue
        ;;
      profit)
        #vypisat celkovy zisk(buy a sell)
				COMMAND="profit"
				continue
        ;;
      pos)
        #hodnoty aktualnych pozicii zostupne
				COMMAND="pos"
				continue
        ;;
      last-price)
        #posledne zname ceny pre ticker
				COMMAND="last-price"
				continue
        ;;
      hist-ord)
        #vypis poctu tranzakcii podla tickerov
				COMMAND="hist-ord"
				continue
        ;;
      graph-pos)
        #vypisat graf hodnot drzanych pozicii podla tickeru
				COMMAND="graph-pos"
				continue
 			 ;;
			*.gz)
				gzip -dk ${!i}
				FILE=${!i::${#i}-4}
				;;
			*.log)
				#zapisat filename logu
				FILE=${!i}
				;;
	esac
done

if [ -z $FILE ]
then
	echo "Prosim zadajte subor:"
	read FILE
fi

case $COMMAND in
	print-log)
		print_log $FILE
		;;
	tick-list)
		if [ ${#TICKERS[@]} -gt 0 ]
		then
			tick_list $FILE ${TICKERS[@]}
		else
			tick_list $FILE
		fi
		;;
	profit)
		profit $FILE
		;;
	pos)
		pos $FILE
		;;
	last-price)
		last_price $FILE
		;;
	hist-ord)
		hist_ord $FILE $WIDTH
		;;
	graph-pos)
		graph_pos $FILE $WIDTH
		;;
esac

IFS=$OLDIFS
