#!/usr/bin/env bash
# Serial monitor that always shows the kit header.
#
# `pio device monitor` does NOT reset the board when it attaches, so opening
# it while the firmware is already running shows only the live color feed.
# This wrapper attaches the monitor, waits until it has the port open, then
# sends a newline to the board - the firmware replies by reprinting the full
# static header (same mechanism as pressing Enter in any serial monitor).
#
# Usage:
#   ./scripts/monitor_with_header.sh              # auto-detect port
#   ./scripts/monitor_with_header.sh --upload     # build+upload first, then monitor
#   MONITOR_PORT=/dev/ttyUSB0 ./scripts/monitor_with_header.sh
#   PIO_ENV=esp32s3dev ./scripts/monitor_with_header.sh
#
# --upload runs `pio run -t upload` BEFORE attaching: the upload's hard reset
# prints the header while no monitor is listening, and the helper below then
# reprints it as soon as the monitor has the port - so the flow
# "upload, then watch" always shows the full kit header.
set -u

PIO_ENV="${PIO_ENV:-esp32s3dev}"

UPLOAD=0
if [ "${1:-}" = "--upload" ]; then
  UPLOAD=1
  shift
fi

# Find a PlatformIO binary (prefer the active PATH, fall back to the penv).
if command -v pio >/dev/null 2>&1; then
  PIO=pio
elif [ -x "$HOME/.platformio/penv/bin/pio" ]; then
  PIO="$HOME/.platformio/penv/bin/pio"
else
  echo "monitor_with_header: pio not found on PATH" >&2
  exit 1
fi

# Resolve the serial port: env override, then first USB serial device.
PORT="${MONITOR_PORT:-}"
if [ -z "$PORT" ]; then
  for p in /dev/ttyACM* /dev/ttyUSB*; do
    if [ -e "$p" ]; then PORT="$p"; break; fi
  done
fi
if [ -z "$PORT" ]; then
  echo "monitor_with_header: no serial port found - set MONITOR_PORT=/dev/tty..." >&2
  exit 1
fi

# Upload first (if requested) - must finish BEFORE the helper starts polling,
# otherwise fuser would see esptool holding the port instead of the monitor.
if [ "$UPLOAD" = "1" ]; then
  "$PIO" run -e "$PIO_ENV" -t upload || exit 1
fi

# Background helper: once the monitor has the port open, trigger the reprint.
(
  if command -v fuser >/dev/null 2>&1; then
    # Wait until a host process (the monitor) holds the port, then send the
    # newline - fuser polls every 200 ms for up to 30 s.
    for _ in $(seq 1 150); do
      if fuser -s "$PORT" 2>/dev/null; then
        sleep 0.5   # let miniterm finish its banner/init
        break
      fi
      sleep 0.2
    done
  else
    sleep 3        # no fuser: best-effort fixed delay
  fi
  printf '\n' > "$PORT" 2>/dev/null || true
) &

exec "$PIO" device monitor -e "$PIO_ENV" -p "$PORT" "$@"
