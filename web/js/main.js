'using strict';

var par = CreatePar();

$('#menu-toggle').click(function() {
    $('nav').toggle();
    GIZA.refreshSize();
});

$(function() {
    console.log('Hello world.');
});
