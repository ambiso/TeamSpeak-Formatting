import hsv from 'color-space/hsv';

function pad2(s, n) {
    while (s.length < n) {
        s = '0'.concat(s);
    }
    return s;
}

const colorvalues = [
    [0x00, 0xFF, 0xFF], [0xF0, 0xFF, 0xFF], [0xF5, 0xF5, 0xDC],
    [0xFF, 0xE4, 0xC4], [0x00, 0x00, 0xFF], [0xA5, 0x2A, 0x2A],
    [0xFF, 0x7F, 0x50], [0x00, 0xFF, 0xFF], [0xFF, 0xD7, 0x00],
    [0x80, 0x80, 0x80], [0x00, 0x80, 0x00], [0x4B, 0x00, 0x82],
    [0xFF, 0xFF, 0xF0], [0xF0, 0xE6, 0x8C], [0xFA, 0xF0, 0xE6],
    [0x80, 0x00, 0x00], [0x00, 0x00, 0x80], [0x80, 0x80, 0x00],
    [0xFF, 0xA5, 0x00], [0xDA, 0x70, 0xD6], [0xCD, 0x85, 0x3F],
    [0xFF, 0xC0, 0xCB], [0xDD, 0xA0, 0xDD], [0x80, 0x00, 0x80],
    [0xFF, 0x00, 0x00], [0xFA, 0x80, 0x72], [0xA0, 0x52, 0x2D],
    [0xC0, 0xC0, 0xC0], [0xFF, 0xFA, 0xFA], [0xD2, 0xB4, 0x8C],
    [0x00, 0x80, 0x80], [0xFF, 0x63, 0x47], [0xEE, 0x82, 0xEE],
    [0xF5, 0xDE, 0xB3], [0xFF, 0xFF, 0xFF], [0xFF, 0xFF, 0x00],
    [0x00, 0x00, 0x00]
];

const colornames = [
    "Aqua", "Azure", "Beige", "Bisque", "Blue", "Brown",
    "Coral", "Cyan", "Gold", "Gray", "Green", "Indigo",
    "Ivory", "Khaki", "Linen", "Maroon", "Navy", "Olive",
    "Orange", "Orchid", "Peru", "Pink", "Plum", "Purple",
    "Red", "Salmon", "Sienna", "Silver", "Snow", "Tan",
    "Teal", "Tomato", "Violet", "Wheat", "White", "Yellow",
    "Black"
];

/**
 * Return a string representing the color
 * @param rgb array of rgb values in [0,255]
 * @param compression euclidean distance allowed to a matching color with a shorter name
 * @returns {string}
 */
function rgb2str(rgb, compression) {
    // Try to find a string representation of the color with same or equal length
    let best_dist2 = Infinity;
    let best_color;
    for (let i = 0; i < colorvalues.length; ++i) {
        const color = colorvalues[i];
        let dist2 = 0;
        for (let j = 0; j < color.length; ++j) {
            dist2 += (color[j] - rgb[j]) ** 2;
        }
        if (dist2 <= best_dist2) {
            best_dist2 = dist2;
            best_color = i;
        }
    }
    if (best_dist2 <= compression) {
        return colornames[best_color];
    }
    return '#' + rgb.map(Math.floor).map((n) => pad2(n.toString(16), 2)).reduce((a, x) => {
        return a + x;
    });
}

const OptimizationStage = {
    PRE: 1, // Do preparation
    AST: 2, // Do AST optimizations, may be called multiple times, may return an item with which to replace it with
    RENDER: 3, // Return string representation
};

/**
 * Represents some text
 */
class Component {
    /**
     * @param compression level of compression
     * @param last whether this component is the last for the current rendering scope
     * @param stage OptimizationStage
     * @param conform whether to output valid bb codes
     * if true, closing tags can be omitted
     */
    run_stage(compression, last, stage, conform) {
    }

    /**
     * @param next next component to be processed
     * @returns {*} merged version of both iff this component can be merged with its successor
     * @note must be called after OptimizationStage.PRE
     */
    merge(next) {
        return undefined;
    }
}

class TextComponent extends Component {
    constructor(text) {
        super();
        this.text = text;
    }

    run_stage(compression, last, stage) {
        if (stage === OptimizationStage.RENDER) {
            return this.text;
        }
    }
}

class ColorTag extends Component {
    constructor(rgb, child) {
        super();
        this.rgb = rgb;
        this.child = child;
    }

    run_stage(compression, last, stage, conform) {
        switch (stage) {
            case OptimizationStage.PRE:
                this.color = rgb2str(this.rgb, compression);
                this.child.run_stage(compression, true, stage, conform);
                break;
            case OptimizationStage.AST:
                if (this.rgb === [0, 0, 0]) {
                    return this.child;
                }
                const child = this.child.run_stage(compression, true, stage, conform);
                if (child) {
                    this.child = child;
                }
                break;
            case OptimizationStage.RENDER:
                const end_tag = (!conform && last) ? '' : '[/COLOR]';
                return `[COLOR=${this.color}]${this.child.run_stage(compression, true, stage, conform)}` + end_tag;
        }
    }

    merge(next) {
        if (next.color === this.color) {
            this.child = new TagCollection([this.child, next.child]);
            return this;
        }
    }
}

class TagCollection extends Component {
    constructor(children) {
        super();
        this.children = children;
    }

    run_stage(compression, last, stage, conform) {
        switch (stage) {
            case OptimizationStage.PRE:
                for (let i = 0; i < this.children.length; ++i) {
                    this.children[i].run_stage(compression, i === this.children.length - 1, stage, conform);
                }
                break;
            case OptimizationStage.AST:
                for (let i = 0; i < this.children.length - 1;) {
                    const merged = this.children[i].merge(this.children[i + 1]);
                    if (merged) {
                        this.children = this.children.splice(i, 2, merged);
                        continue;
                    }
                    const child = this.children[i].run_stage(compression, i === this.children.length - 1, stage, conform);
                    if (child) {
                        this.children.splice(i, 1, child);
                    }
                    ++i;
                }
                break;
            case OptimizationStage.RENDER:
                let result = '';
                for (let i = 0; i < this.children.length; ++i) {
                    result += this.children[i].run_stage(compression, i === this.children.length - 1, stage, conform);
                }
                return result;
                break;
        }
    }
}

function render(component, compression, conform) {
    const stages = [
        OptimizationStage.PRE,
        OptimizationStage.AST,
        OptimizationStage.RENDER,
    ];
    let result;
    for (let i = 0; i < stages.length; ++i) {
        result = component.run_stage(compression, true, stages[i], conform);
        if (stages[i] === OptimizationStage.AST && result) {
            component = result;
        }
    }
    return result;
}

function rainbow(text) {
    const step = 255 / text.length;
    let hue = 0;
    const components = [];
    for (let i = 0; i < text.length; ++i) {
        const color = hsv.rgb([hue, 100, 100]);
        components.push(new ColorTag(color, new TextComponent(text[i])));
        hue += step;
    }
    return new TagCollection(components);
}

function ghostly(text) {
    const components = [];
    const m = 255 / text.length;
    for (let i = 0; i < text.length; ++i) {
        const lightness = m * i;
        const color = [lightness, lightness, lightness];
        components.push(new ColorTag(color, new TextComponent(text[i])));
    }
    return new TagCollection(components);
}

function modemap() {
    let map = new Map();
    map.set('rainbow', rainbow);
    map.set('ghostly', ghostly);
    map.set('more coming soon!!', null);
    return map;
}

export function modeList() {
    return Array.from(modemap().keys());
}

export function transformText(text, mode, compression, conform) {
    const modefunc = modemap().get(mode.toLowerCase());
    if (modefunc) {
        return render(modefunc(text), compression, conform);
    }
    console.error('unknown mode');
    return text;
}
