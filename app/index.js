/**
 * Application entry point
 */

// Load application styles
import 'styles/index.scss';
import {transformText, modeList} from "./transformText";
import {render} from 'phoenix-bbcode';
function capitalizeFirst(string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
}

$(document).ready(() => {
    let mode;
    const menu = $('.modeselect').find('.menu');
    const modes = modeList();
    for (let i = 0; i < modes.length; ++i) {
        const item = $('<div/>');
        item.attr('class', 'item');
        item.attr('data-value', modes[i]);
        item.text(capitalizeFirst(modes[i]));
        menu.append(item);
    }
    let prev_mode = modes[0];
    if (localStorage.prev_mode) {
        prev_mode = localStorage.prev_mode;
        console.log('restoring mode', prev_mode);
    }
    $('.ui.dropdown')
        .dropdown({onChange: (e) => {
            mode = e;
            localStorage.prev_mode = e;
            $('#headertext').html(render(transformText('Text Style', e, 0, true)));
        }})
        .dropdown('set selected', prev_mode);
    const input = $('#textinput');
    const target = $('#styled');
    input.on('input', () => {
        const transformed = transformText(input.val(), mode, 10, true);
        target.val(transformed);

        $('#lengthlabel').text(`Length: ${transformed.length}`);
        let currentFocus = document.activeElement;
        const tgt = target.get(0);
        tgt.focus();
        tgt.setSelectionRange(0, tgt.value.length);
        if(!document.execCommand('copy')) {
            console.error('cannot copy data');
        }
        if (currentFocus && typeof currentFocus.focus === "function") {
            currentFocus.focus();
        }
    });
});
