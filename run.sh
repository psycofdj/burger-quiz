#!/bin/bash

set -e

function log {
  #logger -t burger-quiz "$@"
  echo "$(date -R): $@"
}

function pulse_get_source {
  pactl stat | grep "Default Source:" | cut -d: -f2 | awk '{print $1}'
}

function pulse_get_sink {
  pactl stat | grep "Default Sink:" | cut -d: -f2 | awk '{print $1}'
}

function pulse_set_source {
  local l_source=$1; shift
  local l_retry=5
  log "configuring pulseaudio source to: ${l_source}"
  while [ ${l_retry} -gt 0 ]; do
    pacmd set-default-source ${l_source} >/dev/null 2>&1 || true
    l_current=$(pulse_get_source)
    if [ "${l_current}" == "${l_source}" ]; then
        return 0
    fi
    log "== source still configured to ${l_current}, retrying in 5s..."
    sleep 5
    l_retry=$((l_retry-1))
  done
  log "== could not configure source, exiting"
  return 1
}

function pulse_set_sink {
  local l_sink=$1; shift
  local l_retry=5
  log "configuring pulseaudio sink to: ${l_sink}"
  while [ ${l_retry} -gt 0 ]; do
    pacmd set-default-sink ${l_sink}  >/dev/null 2>&1 || true
    l_current=$(pulse_get_sink)
    if [ "${l_current}" == "${l_sink}" ]; then
        return 0
    fi
    log "== sink still configured to ${l_current}, retrying in 5s..."
    sleep 5
    l_retry=$((l_retry-1))
  done
  log "== could not configure sink, exiting"
  return 1
}

function run {
  PULSE_SOURCE_NAME="alsa_input.usb-C-Media_Electronics_Inc._USB_Audio_Device-00-Device.analog-mono"
  PULSE_SINK_NAME="alsa_output.usb-C-Media_Electronics_Inc._USB_Audio_Device-00-Device.analog-stereo"

  log "   starting at: $(date -R)"
  log "desired source: ${PULSE_SOURCE_NAME}"
  log "  desired sink: ${PULSE_SINK_NAME}"
  log ""

  export USER=$(whoami)
  export USERID=$(id -u)
  export LOGNAME=${USER}
  export XDG_RUNTIME_DIR=/run/user/${USERID}
  export PYTHONPATH=${HOME}/burger-quiz/

  sleep 30
  pulse_set_source ${PULSE_SOURCE_NAME}
  pulse_set_sink ${PULSE_SINK_NAME}
  pactl set-sink-volume ${PULSE_SINK_NAME} 100%
  pactl stat | grep "Default Sink:"

  log "starting client"
  /usr/bin/python3 -m client
}

run
