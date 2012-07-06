canvas = null
ctx = null
radius = 500

rgbToHsv = (r, g, b) ->
  r = r / 0xF
  g = g / 0xF
  b = b / 0xF

  min = Math.min(r, g, b)
  max = Math.max(r, g, b)
  delta = max - min

  v = max

  if max != 0
    s = delta / max
  else
    s = 0
    h = -1
    return [h, s, v]

  if r == max
    h = ( g - b ) / delta
  else if g == max
    h = 2 + ( b - r ) / delta
  else
    h = 4 + ( r - g ) / delta

  h *= 60
  if h < 0
    h += 360

  h = h / 180 * Math.PI

  return [h, s, v]


ready = () ->
  canvas = document.getElementById('wheel')
  ctx = canvas.getContext('2d')

drawDot = (r, g, b) ->
  [h, s, v] = rgbToHsv(r, g, b)
  r <<= 4
  g <<= 4
  b <<= 4

  color = "rgb(#{r},#{g},#{b})"
  width = 16
  r = radius * v
  x = radius + r * Math.cos(h)
  y = radius + r * Math.sin(h)
  x -= width / 2
  y -= width / 2
  ctx.fillStyle = color
  ctx.fillRect(x,y,width,width)

draw = () ->
  strokeSize = 3
  size = Math.min(window.innerWidth, window.innerHeight)
  ctx.canvas.width = ctx.canvas.height = size
  radius = (size - 2 * strokeSize) / 2

  for mult in [0..16]
    r = mult * (radius / 16)
    ctx.beginPath()
    ctx.arc(radius, radius, r, 0, 2 * Math.PI, false)
    ctx.lineWidth = strokeSize
    ctx.strokeStyle = "white"
    ctx.stroke()

  for red in [0..0xF]
    for green in [0..0xF]
      drawDot(red, green, 0)

  for green in [0..0xF]
    for blue in [0..0xF]
      drawDot(0, green, blue)

  for blue in [0..0xF]
    for red in [0..0xF]
      drawDot(red, 0, blue)

document.addEventListener "DOMContentLoaded", () ->
  document.removeEventListener( "DOMContentLoaded", arguments.callee, false )
  ready()
  draw()
, false

window.addEventListener("resize", draw, false)
