$(document).ready(function () {
    socket.on('connect', function () {
        socket.emit('my_event', { data: 'I\'m connected!' });
    });
    socket.on('my_response', function (msg) {
        $('#log').append('<br>' + $('<div/>').text('Received:' + msg.data).html());
    });
    $('form#emit').submit(function (event) {
        socket.emit('my_event', { data: $('#emit_data').val() });
        return false;
    });
    $('form#disconnect').submit(function (event) {
        socket.emit('disconnect_request');
        return false;
    });
});
