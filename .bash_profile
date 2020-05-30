#
# ~/.bash_profile
#

[[ -f ~/.bashrc ]] && . ~/.bashrc

export PATH=${PATH}:$HOME/.scripts
export BROWSER=chromium
export TERMINAL=xterm
export EDITOR=vim
export VISUAL=vim
export PAGER=less
export XDG_CONFIG_HOME=$HOME/.config

# Panel variables for lemonbar
export PANEL_FIFO=/tmp/panel-fifo
export PANEL_HEIGHT=24
export PANEL_FONT="-*-terminus-medium-*-*-20-*-*-*-*-*-*-*"
export PANEL_WM_NAME=bspwm_panel
export W3MIMGDISPLAY_PATH=/usr/lib/w3m/w3mimgdisplay
