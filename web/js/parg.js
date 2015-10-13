'using strict';

var PargApp = function(canvas, args, baseurl) {
    this.canvas = canvas;
    this.args = args;
    this.baseurl = baseurl || 'parg/';
    this.nrequests = 0;
    this.requests = [];
    this.linked_module = null;

    // First, execute the user-defined main() function in its entirety:
    this.module = CreateParg({parg: this});

    // Now, make HTTP requests for all assets:
    this.request_assets();

    // After receiving responses from all HTTP requests, parg will
    // automatically call the user-defined init() function.
};

PargApp.prototype.asset_preload = function(id) {
    var url = this.baseurl + id;
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'arraybuffer';
    var onloadFunc = function() {
        if (xhr.response) {
            this.onasset(id, xhr.response)
        }
    }.bind(this);
    var errorFunc = function() {
        window.console.error('Unable to download ' + url);
    };
    xhr.onload = onloadFunc;
    xhr.onerror = errorFunc;
    this.requests[this.nrequests++] = xhr;
};

PargApp.prototype.request_assets = function() {
    for (var i = 0, len = this.requests.length; i < len; i++) {
        this.requests[i].send(null);
    }
};

PargApp.prototype.onasset = function(id, arraybuffer) {
    var ptr = this.module.Asset.alloc(id, arraybuffer.byteLength);
    var u8buffer = new Uint8Array(arraybuffer);
    this.module.HEAPU8.set(u8buffer, ptr);
    this.module.Asset.commit(id);
    if (--this.nrequests == 0) {
        this.start();
    }
};

PargApp.prototype.start = function() {

    var cevents = {
        PAR_EVENT_DOWN: 0,
        PAR_EVENT_UP: 1,
        PAR_EVENT_MOVE: 2
    };

    var dims = this.module.par_window_dims;
    var $canvas = $(this.canvas);
    var canvas = $canvas[0];
    $canvas.css({
        width: dims[0] + 'px',
        height: dims[1] + 'px'
    });
    canvas.width = dims[0] * window.devicePixelRatio;
    canvas.height = dims[1] * window.devicePixelRatio;

    var GLctx = this.module.createContext(canvas, 1, 1, {
        alpha: true,
        antialias: true
    })
    GLctx.clearColor(0.2, 0.4, 0.8, 1.0);
    GLctx.clear(GLctx.COLOR_BUFFER_BIT);
    $canvas.show();

    this.module.Window.init(this.args);

    var clientWidth = canvas.clientWidth;
    var clientHeight = canvas.clientHeight;
    var clientMaxY = clientHeight - 1;

    var onmousecore = function(event) {
        var box = canvas.getBoundingClientRect();
        var x = (event.clientX - box.left) / clientWidth;
        var y = (clientMaxY - event.clientY + box.top) / clientHeight;
        var etype = event.type;
        if (etype == "mousedown") {
            this.Window.input(cevents.PAR_EVENT_DOWN, x, y, 0);
        } else if (etype == "mouseup") {
            this.Window.input(cevents.PAR_EVENT_UP, x, y, 0);
        } else if (etype == "mousemove") {
            this.Window.input(cevents.PAR_EVENT_MOVE, x, y, 0);
        } else if (etype == "mousewheel") {
            event.preventDefault();
            var delta = event.wheelDelta / 10.0;
            this.Window.input(cevents.PAR_EVENT_MOVE, x, y, delta);
        } else if (etype == "DOMMouseScroll") {
            event.preventDefault();
            var delta = -event.detail / 2.0;
            this.Window.input(cevents.PAR_EVENT_MOVE, x, y, delta);
        }
    };

    var onmouse = onmousecore.bind(this.module);
    if (this.linked_module) {
        var fn1 = onmouse;
        var fn2 = onmousecore.bind(this.linked_module);
        onmouse = function(event) {
            fn1(event);
            fn2(event);
        };
    }

    canvas.addEventListener("mousedown", onmouse);
    canvas.addEventListener("mouseup", onmouse);
    canvas.addEventListener("mousemove", onmouse);
    canvas.addEventListener("mousewheel", onmouse);
    canvas.addEventListener("DOMMouseScroll", onmouse);

    var raf = function() {
        var milliseconds = window.performance.now();
        this.module.Window.tick(milliseconds / 1000.0);
        this.module.Window.draw();
        window.requestAnimationFrame(raf);
    }.bind(this);

    window.requestAnimationFrame(raf);
};
