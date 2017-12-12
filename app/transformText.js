import hsv from 'color-space/hsv';

function pad2(s, n) {
    while (s.length < n) {
        s = '0'.concat(s);
    }
    return s;
}

function rgb2str(rgb) {
    return rgb.map(Math.floor).map((n) => pad2(n.toString(16), 2)).reduce((a, x) => { return a + x; });
}

function rainbow(text) {
    let res = "";
    const step = 255 / text.length;
    let hue = 0;
    for (let i = 0; i < text.length; ++i) {
        const color = hsv.rgb([hue, 100, 100]);
        res += `[COLOR=#${rgb2str(color)}]${text[i]}[/COLOR]`;
        hue += step;
    }
    return res;
}

function modemap() {
    let map = new Map();
    map.set('rainbow', rainbow);
    map.set('more coming soon!!', null);
    return map;
}

export function modeList() {
    return Array.from(modemap().keys());
}

export function transformText(text, mode) {
    const modefunc = modemap().get(mode.toLowerCase());
    if (modefunc) {
        console.log('text', text, typeof text);
        return modefunc(text);
    }
    console.log('unknown mode');
    return text;
}
