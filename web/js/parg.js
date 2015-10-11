'using strict';

var parg = {};

parg.module = null;

parg.init = function() {

    // First, execute the user-defined main() function in its entirety:
    parg.module = CreateParg({parg: parg});

    // Now, make HTTP requests for all assets:
    parg.request_assets();

    // After receiving the response from all HTTP requests, parg will
    // automatically call the user-defined init() function.

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
            parg.onasset(id, xhr.response)
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

parg.onasset = function(id, arraybuffer) {
    var ptr = parg.module.Asset.alloc(id, arraybuffer.byteLength);
    var u8buffer = new Uint8Array(arraybuffer);
    parg.module.HEAPU8.set(u8buffer, ptr);
    parg.module.Asset.commit(id);
    if (--parg.nrequests == 0) {
        parg.start();
    }
};

parg.canvas = '#canvas3d';

parg.start = function() {

    var cevents = {
        PAR_EVENT_DOWN: 0,
        PAR_EVENT_UP: 1,
        PAR_EVENT_MOVE: 2
    };

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

    var clientWidth = canvas.clientWidth;
    var clientHeight = canvas.clientHeight;
    var clientMaxY = clientHeight - 1;

    var onmouse = function(event) {
        var box = canvas.getBoundingClientRect();
        var x = (event.clientX - box.left) / clientWidth;
        var y = (clientMaxY - event.clientY + box.top) / clientHeight;
        var etype = event.type;
        if (etype == "mousedown") {
            parg.module.Window.input(cevents.PAR_EVENT_DOWN, x, y, 0);
        } else if (etype == "mouseup") {
            parg.module.Window.input(cevents.PAR_EVENT_UP, x, y, 0);
        } else if (etype == "mousemove") {
            parg.module.Window.input(cevents.PAR_EVENT_MOVE, x, y, 0);
        } else if (etype == "mousewheel") {
            event.preventDefault();
            var delta = event.wheelDelta / 10.0;
            parg.module.Window.input(cevents.PAR_EVENT_MOVE, x, y, delta);
        } else if (etype == "DOMMouseScroll") {
            event.preventDefault();
            var delta = -event.detail / 2.0;
            parg.module.Window.input(cevents.PAR_EVENT_MOVE, x, y, delta);
        }
    };

    canvas.addEventListener("mousedown", onmouse);
    canvas.addEventListener("mouseup", onmouse);
    canvas.addEventListener("mousemove", onmouse);
    canvas.addEventListener("mousewheel", onmouse);
    canvas.addEventListener("DOMMouseScroll", onmouse);

    function raf() {
        var milliseconds = window.performance.now();
        parg.module.Window.tick(milliseconds / 1000.0);
        parg.module.Window.draw();
        window.requestAnimationFrame(raf);
    };

    window.requestAnimationFrame(raf);
};
