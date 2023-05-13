var codec_list = '\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">G.711 A-law</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">G.711 u-law</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">G.719</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">G.722</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">G.726</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">G.729A</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">GSM</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">ILBC</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">LPC-10</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">Speex</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">Linear PCM</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">VP9</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">VP8</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">JPEG</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">H.264</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">H.263+</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">H.263</td>\n\
</tr>\n\
<tr>\n\
<td class="cell" onclick="chose_codec(this)">H.261</td>\n\
</tr>'
socket.on("RES_CODEC_SETTING", msg => {
    codec_setting_status = false;
    console.log(msg);
    document.querySelector("#Available_Codes > tbody").innerHTML = '';
    document.querySelector("#Allowed_Codes > tbody").innerHTML = '';
    msg.avail_codec.forEach(element => {
        document.querySelector("#Available_Codes > tbody").innerHTML += '\n' +
            '<tr>\n' +
            '<td class="cell" onclick="chose_codec(this)">' + decode_codec_value(element) +'</td>\n' +
            '</tr>\n'
    });
    document.querySelector("#Allowed_Codes");
    msg.allow_codec.forEach(element => {
        document.querySelector("#Allowed_Codes > tbody").innerHTML += '\n' +
            '<tr>\n' +
            '<td class="cell" onclick="chose_codec(this)">' + decode_codec_value(element) +'</td>\n' +
            '</tr>\n'
    });
})
function codec_setting_save(){
    var table = document.querySelector("#Allowed_Codes");
    for (let i = 0, row; row = table.rows[i]; i++){
        console.log(table.rows[i].cells[0].innerText);
        data.codec_setting.allow_codec.push(get_codec_value(table.rows[i].cells[0].innerText))
    }
    table = document.querySelector("#Available_Codes");
    for (let i = 0, row; row = table.rows[i]; i++){
        data.codec_setting.avail_codec.push(get_codec_value(table.rows[i].cells[0].innerText))
    }
    var buttonChange = document.getElementById('buttonChange');
    buttonChange.style.display = 'block';
}
function chose_codec(el){
    el.classList.toggle("active");
}
function select_all_codec(){
    document.querySelector("#Allowed_Codes > tbody").innerHTML = codec_list;
    document.querySelector("#Available_Codes > tbody").innerHTML = '';
}
function remove_all_codec(){
    document.querySelector("#Available_Codes > tbody").innerHTML = codec_list;
    document.querySelector("#Allowed_Codes > tbody").innerHTML = '';
}
function select_codec(){
    var table = document.querySelector("#Available_Codes");
    for (let i = 0, row; row = table.rows[i]; i++){
        if (table.rows[i].cells[0].className.includes('active')){
            console.log(i);
            document.querySelector("#Allowed_Codes > tbody").innerHTML += '\n' +
            '<tr>\n' +
            '<td class="cell" onclick="chose_codec(this)">' + table.rows[i].cells[0].innerText +'</td>\n' +
            '</tr>\n'
            table.deleteRow(i);
            i = -1;
        }
    }
}
function remove_codec(){
    var table = document.querySelector("#Allowed_Codes");
    for (let i = 0, row; row = table.rows[i]; i++){
        if (table.rows[i].cells[0].className.includes('active')){
            console.log(i);
            document.querySelector("#Available_Codes > tbody").innerHTML += '\n' +
            '<tr>\n' +
            '<td class="cell" onclick="chose_codec(this)">' + table.rows[i].cells[0].innerText +'</td>\n' +
            '</tr>\n'
            table.deleteRow(i);
            i = -1;
        }
    }
}
function get_codec_value(codec){
    if (codec.includes('G.711 A-law')) return 'alaw'
    if (codec.includes('G.711 u-law'))return 'ulaw'
    if (codec.includes('G.719')) return 'g719'
    if (codec.includes('G.722')) return 'g722'
    if (codec.includes('G.726')) return 'g726'
    if (codec.includes('G.729A')) return 'g729'
    if (codec.includes('GSM')) return 'gsm'
    if (codec.includes('ILBC')) return 'ilbc'
    if (codec.includes('LPC-10')) return 'lpc10'
    if (codec.includes('Speex')) return 'speex'
    if (codec.includes('Linear PCM')) return 'slin'
    if (codec.includes('VP9')) return 'vp9'
    if (codec.includes('VP8')) return 'vp8'
    if (codec.includes('JPEG')) return 'jpeg'
    if (codec.includes('H.264')) return 'h264'
    if (codec.includes('H.263+')) return 'h263p'
    if (codec.includes('H.263')) return 'h263'
    if (codec.includes('H.261')) return 'h261'
}
function decode_codec_value(codec){
if (codec != null){
    if (codec.includes('alaw')) return 'G.711 A-law'
    if (codec.includes('ulaw'))return 'G.711 u-law'
    if (codec.includes('g719')) return 'G.719'
    if (codec.includes('g722')) return 'G.722'
    if (codec.includes('g726')) return 'G.726'
    if (codec.includes('g729')) return 'G.729A'
    if (codec.includes('gsm')) return 'GSM'
    if (codec.includes('ilbc')) return 'ILBC'
    if (codec.includes('lpc10')) return 'LPC-10'
    if (codec.includes('speex')) return 'Speex'
    if (codec.includes('slin')) return 'Linear PCM'
    if (codec.includes('vp9')) return 'VP9'
    if (codec.includes('vp8')) return 'VP8'
    if (codec.includes('jpeg')) return 'JPEG'
    if (codec.includes('h264')) return 'H.264'
    if (codec.includes('h263p')) return 'H.263+'
    if (codec.includes('h263')) return 'H.263'
    if (codec.includes('h261')) return 'H.261'
}
else return '' 
}
