#!/bin/bash

set -e

function log {
  logger -t burger-quiz "$@"
}

function connect_bt {
  local l_device=$1; shift
  local l_retry=5
  log "connecting bluetooh device: ${l_device}"
  while [ ${l_retry} -gt 0 ]; do
    bt-audio -c ${l_device} || {
      log "== error while connecting bluetooh device: ${l_device}, retrying in 5s"
      sleep 5
      l_retry=$((l_retry-1))
    }
    break
  done
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
    pacmd set-default-source ${l_source}
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
    pacmd set-default-sink ${l_sink}
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
  BT_AUDIO_MAC="20:DF:B9:CF:C7:2F"
  PULSE_SOURCE_NAME="alsa_input.platform-aml_m8_sound_card.4.analog-stereo"
  PULSE_SINK_NAME="bluez_sink.$(echo ${BT_AUDIO_MAC} | tr ':' '_')"

  log "starting at: $(date -R)"
  log "  bt-device: ${BT_AUDIO_MAC}"
  log "     source: ${PULSE_SOURCE_NAME}"
  log "       sink: ${PULSE_SINK_NAME}"

  export USER=$(whoami)
  export USERID=$(id -u)
  export LOGNAME=${USER}
  export XDG_RUNTIME_DIR=/run/user/${USERID}
  export PYTHONPATH=${HOME}/burger-quiz/

  connect_bt ${BT_AUDIO_MAC}
  pulse_set_source ${PULSE_SOURCE_NAME}
  pulse_set_sink ${PULSE_SINK_NAME}

  /usr/bin/python3 -m client |& log
}

run
