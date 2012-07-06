(function() {
  var canvas, ctx, draw, drawDot, radius, ready, rgbToHsv;

  canvas = null;

  ctx = null;

  radius = 500;

  rgbToHsv = function(r, g, b) {
    var delta, h, max, min, s, v;
    r = r / 0xF;
    g = g / 0xF;
    b = b / 0xF;
    min = Math.min(r, g, b);
    max = Math.max(r, g, b);
    delta = max - min;
    v = max;
    if (max !== 0) {
      s = delta / max;
    } else {
      s = 0;
      h = -1;
      return [h, s, v];
    }
    if (r === max) {
      h = (g - b) / delta;
    } else if (g === max) {
      h = 2 + (b - r) / delta;
    } else {
      h = 4 + (r - g) / delta;
    }
    h *= 60;
    if (h < 0) h += 360;
    h = h / 180 * Math.PI;
    return [h, s, v];
  };

  ready = function() {
    canvas = document.getElementById('wheel');
    return ctx = canvas.getContext('2d');
  };

  drawDot = function(r, g, b) {
    var color, h, s, v, width, x, y, _ref;
    _ref = rgbToHsv(r, g, b), h = _ref[0], s = _ref[1], v = _ref[2];
    r <<= 4;
    g <<= 4;
    b <<= 4;
    color = "rgb(" + r + "," + g + "," + b + ")";
    width = 16;
    r = radius * v;
    x = radius + r * Math.cos(h);
    y = radius + r * Math.sin(h);
    x -= width / 2;
    y -= width / 2;
    ctx.fillStyle = color;
    return ctx.fillRect(x, y, width, width);
  };

  draw = function() {
    var blue, green, mult, r, red, size, strokeSize, _results;
    strokeSize = 3;
    size = Math.min(window.innerWidth, window.innerHeight);
    ctx.canvas.width = ctx.canvas.height = size;
    radius = (size - 2 * strokeSize) / 2;
    for (mult = 0; mult <= 16; mult++) {
      r = mult * (radius / 16);
      ctx.beginPath();
      ctx.arc(radius, radius, r, 0, 2 * Math.PI, false);
      ctx.lineWidth = strokeSize;
      ctx.strokeStyle = "white";
      ctx.stroke();
    }
    for (red = 0; 0 <= 0xF ? red <= 0xF : red >= 0xF; 0 <= 0xF ? red++ : red--) {
      for (green = 0; 0 <= 0xF ? green <= 0xF : green >= 0xF; 0 <= 0xF ? green++ : green--) {
        drawDot(red, green, 0);
      }
    }
    for (green = 0; 0 <= 0xF ? green <= 0xF : green >= 0xF; 0 <= 0xF ? green++ : green--) {
      for (blue = 0; 0 <= 0xF ? blue <= 0xF : blue >= 0xF; 0 <= 0xF ? blue++ : blue--) {
        drawDot(0, green, blue);
      }
    }
    _results = [];
    for (blue = 0; 0 <= 0xF ? blue <= 0xF : blue >= 0xF; 0 <= 0xF ? blue++ : blue--) {
      _results.push((function() {
        var _results2;
        _results2 = [];
        for (red = 0; 0 <= 0xF ? red <= 0xF : red >= 0xF; 0 <= 0xF ? red++ : red--) {
          _results2.push(drawDot(red, 0, blue));
        }
        return _results2;
      })());
    }
    return _results;
  };

  document.addEventListener("DOMContentLoaded", function() {
    document.removeEventListener("DOMContentLoaded", arguments.callee, false);
    ready();
    return draw();
  }, false);

  window.addEventListener("resize", draw, false);

}).call(this);
