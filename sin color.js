var len = 0;
var init = function(str) {
    //whole string passed in case it is needed
    //generally ignore this
    //no return value
    len = str.length;
}
var i = 0;
var format = function(char) {
    //called with current character
    var a = {r: 255*Math.pow(Math.sin(Math.PI*i/(2*len)),1/2)};
    ++i;
    return a;
}
