canvas = null
ctx = null
radius = 500

rgbToHsv = (r, g, b) ->
  ###
  r = r / 0xF
  g = g / 0xF
  b = b / 0xF
  ###

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
  # window.addEventListener("resize", draw, false)

  value = 0
  setInterval () ->
    draw value
    value++
    if value > 15
      value = 0
  , 500
  return

  #
  hues = []
  for red in [0..0xF]
    for green in [0..0xF]
      for blue in [0..0xF]
        hues.push rgbToHsv(red, green, blue)[0]

  console.log _.uniq(hues)

  i = 0
  setInterval () ->
    i = drawPaths(i)
  , 100

drawPaths = (start) ->
  canvas.width = canvas.width
  allPoints = []
  outerPoints = []

  drawSomething (r, g, b) ->
    [h, s, v] = rgbToHsv(r, g, b)

    x = radius + (radius * s / 0xF) * Math.cos(h)
    y = radius + (radius * s / 0xF) * Math.sin(h)
    obj =
      hsv : [h, s, v]
      rgb : [r, g, b]
      xy  : [x, y]
    allPoints.push(obj)

  vObj = {}
  allPoints.forEach (point) ->
    [h, s, v] = point.hsv
    vObj[v] = true
  vs = Object.keys(vObj)



  vs.sort()
  vs = vs.reverse()

  allPoints.forEach (point) ->
    [h, s, v] = point.hsv
    if v == 15
      outerPoints.push(point)

  outerPoints = _.sortBy outerPoints, (p) ->
    return p.hsv[0]
  console.log outerPoints.length

  if start > outerPoints.length - 2
    start = 0

  outerPoints.slice(start, start+1).forEach (p) ->
    prevClosest = closestPointWithValueAndHue(allPoints, p.hsv[2] - 1, p.hsv[0])
    for i in [2..15]
      closest = closestPointWithValueAndHue(allPoints, p.hsv[2] - i, p.hsv[0])
      if !closest?
        return

      ctx.beginPath()
      [x, y] = prevClosest.xy
      ctx.moveTo(x,y)
      [x, y] = closest.xy
      ctx.lineTo(x,y)
      ctx.stroke()
      ctx.closePath()

      drawDot.apply(null, prevClosest.rgb)
      drawDot.apply(null, closest.rgb)
      prevClosest = closest
  return start + 1


closestPointWithValueAndHue  = (points, value, hue) ->
  potentials = points.filter (p) ->
    [h, s, v] = p.hsv
    return v == value

  sorted = _.sortBy potentials, (p) ->
    a = Math.abs(p.hsv[0] - hue)
    if a > 180
      a = 360 - a
    return Math.abs(a)
  return sorted[0]



drawDot = (r, g, b) ->
  [h, s, v] = rgbToHsv(r, g, b)
  r <<= 4
  g <<= 4
  b <<= 4

  color = "rgb(#{r},#{g},#{b})"
  width = 12
  r = radius * s * v / 15
  x = radius + r * Math.cos(h)
  y = radius + r * Math.sin(h)

  ctx.beginPath()
  ctx.arc(x, y, width, 0, 2 * Math.PI, false)
  ctx.fillStyle = color
  ctx.fill()

drawLine = (r, g, b) ->
  [h, s, v] = rgbToHsv(r, g, b)

  if( v != 1 )
    return

  r <<= 4
  g <<= 4
  b <<= 4

  color = "rgb(#{r},#{g},#{b})"
  width = 16
  r = radius * v
  x = radius + r * Math.cos(h)
  y = radius + r * Math.sin(h)

  ctx.beginPath()
  ctx.moveTo(x,y)
  ctx.lineTo(radius,radius)
  ctx.stroke()

drawSomethingWhen = (fn, whenTrue) ->
  for red in [0..0xF]
    for green in [0..0xF]
      for blue in [0..0xF]
        if whenTrue(red, green, blue)
          fn(red, green, blue)

draw = (value) ->
  strokeSize = 3
  size = Math.min(window.innerWidth, window.innerHeight)
  ctx.canvas.width = ctx.canvas.height = size
  radius = (size - 2 * strokeSize) / 2

  ###
  for mult in [0..15]
    r = mult * (radius / 15)
    ctx.beginPath()
    ctx.arc(radius, radius, r, 0, 2 * Math.PI, false)
    ctx.lineWidth = strokeSize
    ctx.strokeStyle = "white"
    ctx.stroke()
  ###

  valueMatches = (r, g, b) ->
    return rgbToHsv(r, g, b)[2] == value

  # drawSomethingWhen(drawLine, valueMatches)
  drawSomethingWhen(drawDot, valueMatches)


document.addEventListener "DOMContentLoaded", () ->
  document.removeEventListener( "DOMContentLoaded", arguments.callee, false )
  ready()
, false

