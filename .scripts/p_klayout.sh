#! /bin/sh

Keyboard() {
        RES=$(xkb-switch)
		if [ $? -eq 0 ] ; then
				echo -n "${RES^^}"
		else	
				return 1
		fi

        return 0
}

while true; do
        klayout=$(Keyboard)
		[[ $? -eq 0 ]] && echo "$1$klayout"
		sleep 1
done
