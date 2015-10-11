'using strict';

var parg = {};

parg.module = null;

parg.init = function() {

    // Execute main() in its entirety:
    parg.module = CreateParg({parg: parg});

    // Make HTTP requests for all assets:
    parg.request_assets();

};

parg.nrequests = 0;

parg.requests = [];

parg.asset_preload = function(id) {
    var url = 'parg/' + id;
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'arraybuffer';
    var onloadFunc = function() {
        if (xhr.response) {
            var ptr = parg.module.Asset.alloc(id, xhr.response.byteLength);
            var u8buffer = new Uint8Array(xhr.response);
            parg.module.HEAPU8.set(u8buffer, ptr);
            parg.module.Asset.commit(id);
            if (--parg.nrequests == 0) {

                // Start the game loop only after every asset is fetched:
                parg.start();
            }
        }
    };
    var errorFunc = function() {
        window.console.error('Unable to download ' + url);
    };
    xhr.onload = onloadFunc;
    xhr.onerror = errorFunc;
    parg.requests[parg.nrequests++] = xhr;
};

parg.request_assets = function() {
    for (var i = 0, len = parg.requests.length; i < len; i++) {
        parg.requests[i].send(null);
    }
};

parg.canvas = '#canvas3d';

parg.start = function() {

    var dims = parg.module.par_window_dims;
    var $canvas = $(parg.canvas);
    var canvas = $canvas[0];
    $canvas.css({
        width: dims[0] + 'px',
        height: dims[1] + 'px'
    });
    canvas.width = dims[0] * window.devicePixelRatio;
    canvas.height = dims[1] * window.devicePixelRatio;

    var GLctx = parg.module.createContext(canvas, 1, 1, {
        alpha: true,
        antialias: true
    })
    GLctx.clearColor(0.2, 0.4, 0.8, 1.0);
    GLctx.clear(GLctx.COLOR_BUFFER_BIT);
    $canvas.show();

    parg.module.Window.init();

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
        parg.module.Window.tick(milliseconds / 1000.0);
        parg.module.Window.draw();
        window.requestAnimationFrame(raf);
    };

    window.requestAnimationFrame(raf);
};
