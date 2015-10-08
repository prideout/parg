
Module.par_startup = function(dims) {
    $('#canvas3d').css({
        width: dims[0] + 'px',
        height: dims[1] + 'px'
    });
    var canvas = $('#canvas3d')[0];
    var gl = GLctx = window.GLctx = canvas.getContext('webgl', {
        alpha: true,
        antialias: true
    });
    gl.clearColor(0.2, 0.4, 0.8, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
    $('#canvas3d').show();

    Module.Window.init();

    // TODO set up mouse handlers here...
    // call Module.Window.input as needed

    function raf() {
        var milliseconds = window.performance.now();
        Module.Window.tick(milliseconds / 1000.0);
        Module.Window.draw();
        window.requestAnimationFrame(raf);
    };

    window.requestAnimationFrame(raf);
};

Module.par_startup(Module.par_window_dims);
