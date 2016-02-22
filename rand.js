var str;
var init = function (s) {
	str = s;
	var hash = 0;
	for(var i = 0; i < s.length; ++i) {
			hash *= 31;
			hash += s.charCodeAt(i);
	}
	cur = hash;
}

var cur;
var max = 2147483647;
var rand = function () {
	cur = (cur * 127 + 18724)%max;
	if(cur < 0) { cur = max - cur; }
	return cur / max;
}

var format = function (c) {
	return {bold: rand() < 0.5,
			  italic: rand() < 0.5,
			  underline: rand() < 0.5,
			  strike: rand() < 0.5,
			  r: rand()*255,
			  g: rand()*255,
			  b: rand()*255}
}