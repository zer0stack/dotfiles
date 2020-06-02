#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

alias ls='ls --color=auto'

BOLD="\[$(tput bold)\]"
CYAN="\[$(tput setaf 6)\]"
RESET="\[$(tput sgr0)\]"

PS1="${BOLD}${BLACK}[\u@\h \W]${RESET}$ "

