(function() {
  var canvas, closestPointWithValueAndHue, ctx, draw, drawDot, drawLine, drawPaths, drawSomethingWhen, radius, ready, rgbToHsv;

  canvas = null;

  ctx = null;

  radius = 500;

  rgbToHsv = function(r, g, b) {
    /*
      r = r / 0xF
      g = g / 0xF
      b = b / 0xF
    */
    var delta, h, max, min, s, v;
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
    var blue, green, hues, i, red, value;
    canvas = document.getElementById('wheel');
    ctx = canvas.getContext('2d');
    value = 0;
    setInterval(function() {
      draw(value);
      value++;
      if (value > 15) return value = 0;
    }, 500);
    return;
    hues = [];
    for (red = 0; 0 <= 0xF ? red <= 0xF : red >= 0xF; 0 <= 0xF ? red++ : red--) {
      for (green = 0; 0 <= 0xF ? green <= 0xF : green >= 0xF; 0 <= 0xF ? green++ : green--) {
        for (blue = 0; 0 <= 0xF ? blue <= 0xF : blue >= 0xF; 0 <= 0xF ? blue++ : blue--) {
          hues.push(rgbToHsv(red, green, blue)[0]);
        }
      }
    }
    console.log(_.uniq(hues));
    i = 0;
    return setInterval(function() {
      return i = drawPaths(i);
    }, 100);
  };

  drawPaths = function(start) {
    var allPoints, outerPoints, vObj, vs;
    canvas.width = canvas.width;
    allPoints = [];
    outerPoints = [];
    drawSomething(function(r, g, b) {
      var h, obj, s, v, x, y, _ref;
      _ref = rgbToHsv(r, g, b), h = _ref[0], s = _ref[1], v = _ref[2];
      x = radius + (radius * s / 0xF) * Math.cos(h);
      y = radius + (radius * s / 0xF) * Math.sin(h);
      obj = {
        hsv: [h, s, v],
        rgb: [r, g, b],
        xy: [x, y]
      };
      return allPoints.push(obj);
    });
    vObj = {};
    allPoints.forEach(function(point) {
      var h, s, v, _ref;
      _ref = point.hsv, h = _ref[0], s = _ref[1], v = _ref[2];
      return vObj[v] = true;
    });
    vs = Object.keys(vObj);
    vs.sort();
    vs = vs.reverse();
    allPoints.forEach(function(point) {
      var h, s, v, _ref;
      _ref = point.hsv, h = _ref[0], s = _ref[1], v = _ref[2];
      if (v === 15) return outerPoints.push(point);
    });
    outerPoints = _.sortBy(outerPoints, function(p) {
      return p.hsv[0];
    });
    console.log(outerPoints.length);
    if (start > outerPoints.length - 2) start = 0;
    outerPoints.slice(start, start + 1).forEach(function(p) {
      var closest, i, prevClosest, x, y, _ref, _ref2;
      prevClosest = closestPointWithValueAndHue(allPoints, p.hsv[2] - 1, p.hsv[0]);
      for (i = 2; i <= 15; i++) {
        closest = closestPointWithValueAndHue(allPoints, p.hsv[2] - i, p.hsv[0]);
        if (!(closest != null)) return;
        ctx.beginPath();
        _ref = prevClosest.xy, x = _ref[0], y = _ref[1];
        ctx.moveTo(x, y);
        _ref2 = closest.xy, x = _ref2[0], y = _ref2[1];
        ctx.lineTo(x, y);
        ctx.stroke();
        ctx.closePath();
        drawDot.apply(null, prevClosest.rgb);
        drawDot.apply(null, closest.rgb);
        prevClosest = closest;
      }
    });
    return start + 1;
  };

  closestPointWithValueAndHue = function(points, value, hue) {
    var potentials, sorted;
    potentials = points.filter(function(p) {
      var h, s, v, _ref;
      _ref = p.hsv, h = _ref[0], s = _ref[1], v = _ref[2];
      return v === value;
    });
    sorted = _.sortBy(potentials, function(p) {
      var a;
      a = Math.abs(p.hsv[0] - hue);
      if (a > 180) a = 360 - a;
      return Math.abs(a);
    });
    return sorted[0];
  };

  drawDot = function(r, g, b) {
    var color, h, s, v, width, x, y, _ref;
    _ref = rgbToHsv(r, g, b), h = _ref[0], s = _ref[1], v = _ref[2];
    r <<= 4;
    g <<= 4;
    b <<= 4;
    color = "rgb(" + r + "," + g + "," + b + ")";
    width = 12;
    r = radius * s * v / 15;
    x = radius + r * Math.cos(h);
    y = radius + r * Math.sin(h);
    ctx.beginPath();
    ctx.arc(x, y, width, 0, 2 * Math.PI, false);
    ctx.fillStyle = color;
    return ctx.fill();
  };

  drawLine = function(r, g, b) {
    var color, h, s, v, width, x, y, _ref;
    _ref = rgbToHsv(r, g, b), h = _ref[0], s = _ref[1], v = _ref[2];
    if (v !== 1) return;
    r <<= 4;
    g <<= 4;
    b <<= 4;
    color = "rgb(" + r + "," + g + "," + b + ")";
    width = 16;
    r = radius * v;
    x = radius + r * Math.cos(h);
    y = radius + r * Math.sin(h);
    ctx.beginPath();
    ctx.moveTo(x, y);
    ctx.lineTo(radius, radius);
    return ctx.stroke();
  };

  drawSomethingWhen = function(fn, whenTrue) {
    var blue, green, red, _results;
    _results = [];
    for (red = 0; 0 <= 0xF ? red <= 0xF : red >= 0xF; 0 <= 0xF ? red++ : red--) {
      _results.push((function() {
        var _results2;
        _results2 = [];
        for (green = 0; 0 <= 0xF ? green <= 0xF : green >= 0xF; 0 <= 0xF ? green++ : green--) {
          _results2.push((function() {
            var _results3;
            _results3 = [];
            for (blue = 0; 0 <= 0xF ? blue <= 0xF : blue >= 0xF; 0 <= 0xF ? blue++ : blue--) {
              if (whenTrue(red, green, blue)) {
                _results3.push(fn(red, green, blue));
              } else {
                _results3.push(void 0);
              }
            }
            return _results3;
          })());
        }
        return _results2;
      })());
    }
    return _results;
  };

  draw = function(value) {
    var size, strokeSize, valueMatches;
    strokeSize = 3;
    size = Math.min(window.innerWidth, window.innerHeight);
    ctx.canvas.width = ctx.canvas.height = size;
    radius = (size - 2 * strokeSize) / 2;
    /*
      for mult in [0..15]
        r = mult * (radius / 15)
        ctx.beginPath()
        ctx.arc(radius, radius, r, 0, 2 * Math.PI, false)
        ctx.lineWidth = strokeSize
        ctx.strokeStyle = "white"
        ctx.stroke()
    */
    valueMatches = function(r, g, b) {
      return rgbToHsv(r, g, b)[2] === value;
    };
    return drawSomethingWhen(drawDot, valueMatches);
  };

  document.addEventListener("DOMContentLoaded", function() {
    document.removeEventListener("DOMContentLoaded", arguments.callee, false);
    return ready();
  }, false);

}).call(this);
