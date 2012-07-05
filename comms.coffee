SerialPort = require("serialport").SerialPort
serial = new SerialPort("/dev/tty.usbmodemfd121", {baudrate:115200})

_ = require('underscore')

COMMAND =
  SET: 1
  FINISH: 2

finishBuf = new Buffer(5)
finishBuf[0] = COMMAND.FINISH

packData = (cmd, bulb, red, green, blue, brightness) ->
  buf = new Buffer(5)
  buf[0] = cmd
  buf[1] = bulb & 0x3F
  buf[2] = red & 0xF
  buf[3] = ((green & 0xF) << 4) | blue & 0xF
  buf[4] = brightness
  return buf

message = ''
serial.once 'data', (data) ->
  for i in [0..49]
    buf = packData(COMMAND.SET, i, 15, 11, 0x0, 0xCC)

    for thing in buf
      console.log '>>> ' + thing

    serial.write buf
  serial.write finishBuf

serial.on 'data', (data) ->
  message += data.toString('ascii')
  lines = message.split('\n')
  message = lines.pop()
  for line in lines
    console.log line
