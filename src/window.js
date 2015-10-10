Module.parg.canvas = '#canvas3d';

Module.parg.start = function(dims) {
    var $canvas = $(Module.parg.canvas);
    var canvas = $canvas[0];
    $canvas.css({
        width: dims[0] + 'px',
        height: dims[1] + 'px'
    });
    canvas.width = dims[0] * window.devicePixelRatio;
    canvas.height = dims[1] * window.devicePixelRatio;
    var gl = GLctx = window.GLctx = canvas.getContext('webgl', {
        alpha: true,
        antialias: true
    });
    gl.clearColor(0.2, 0.4, 0.8, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
    $canvas.show();

    Module.Window.init();

    var onmouse = function(event) {
        var box = canvas.getBoundingClientRect();
        var x = event.clientX - box.left;
        var y = event.clientY - box.top;
        var etype = event.type;
        if (etype == "mousedown") {
            // Module.Window.input
            window.console.log('down', [x, y]);
        } else if (etype == "mouseup") {
            // Module.Window.input
            window.console.log('up', [x, y]);
        } else if (etype == "mousemove") {
            // Module.Window.input
            window.console.log('move', [x, y]);
        }
    };

    canvas.addEventListener("mousedown", onmouse);
    canvas.addEventListener("mouseup", onmouse);
    canvas.addEventListener("mousemove", onmouse);
    // canvas.addEventListener("mouseenter", onmouse);

    function raf() {
        var milliseconds = window.performance.now();
        Module.Window.tick(milliseconds / 1000.0);
        Module.Window.draw();
        window.requestAnimationFrame(raf);
    };

    window.requestAnimationFrame(raf);
};
