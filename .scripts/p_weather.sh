#! /bin/sh

# Get current temperature in °C
GetTemp() {
		URL='wttr.in/?format=1'
		RES=$(curl -m 4 -s ${URL})
		if [[ $? -eq 0 ]] && [[ ${RES} =~ [+-][0-9]{1,2}°C$ ]] ; then
				echo -n ${BASH_REMATCH[0]}
		else
				return 1	
		fi

		return 0
}

while true; do
	temp=$(GetTemp)
	[[ $? -eq 0 ]] && echo "$1$temp"
    sleep 1800
done
