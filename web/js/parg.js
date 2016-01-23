'using strict';

var PargApp = function(canvas, args, baseurl, block_interaction, attribs) {
    this.attribs = attribs || {
        alpha: true,
        antialias: true
    };
    this.canvas = canvas;
    this.GLctx = null;
    this.args = args;
    this.baseurl = baseurl || 'parg/';
    this.nrequests = 0;
    this.requests = [];
    this.linked_module = null;
    this.block_interaction = block_interaction;

    // First, execute the user-defined main() function in its entirety:
    this.module = CreateParg({parg: this});

    // Now, make HTTP requests for all assets:
    this.request_assets();

    // After receiving responses from all HTTP requests, parg will
    // automatically call the user-defined init() function.
};

PargApp.prototype.image_preload = function(id) {
    var url = this.baseurl + id;
    var img = new Image();
    var onloadFunc = function() {
        this.onimage(id, img);
    }.bind(this);
    var errorFunc = function() {
        window.console.error('Unable to download ' + url);
    };
    img.onload = onloadFunc;
    img.onerror = errorFunc;
    this.requests[this.nrequests++] = img;
    img.src = url;
};

PargApp.prototype.asset_preload = function(id) {
    if (id.endsWith('.png')) {
        this.image_preload(id);
        return;
    }
    var url = this.baseurl + id;
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'arraybuffer';
    var onloadFunc = function() {
        if (xhr.response) {
            this.onasset(id, xhr.response);
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
        if (this.requests[i].send) {
            this.requests[i].send(null);
        }
    }
};

PargApp.prototype.onasset = function(id, arraybuffer) {
    var ptr = this.module.Asset.alloc(id, arraybuffer.byteLength);
    var u8buffer = new Uint8Array(arraybuffer);
    this.module.HEAPU8.set(u8buffer, ptr);
    this.module.Asset.commit(id);
    if (--this.nrequests === 0) {
        this.start();
    }
};

PargApp.prototype.onimage = function(id, img) {
    var canvas = document.createElement("canvas");
    var w = canvas.width = img.width;
    var h = canvas.height = img.height;
    var ctx = canvas.getContext("2d");
    ctx.drawImage(img, 0, 0);
    var pixelview = ctx.getImageData(0, 0, w, h).data;
    var pixelbuf = pixelview.buffer;
    var annotated = new Uint8Array(pixelbuf.byteLength + 12);
    var metadata = new Uint32Array(annotated.buffer);
    metadata[0] = w;
    metadata[1] = h;
    metadata[2] = 4;
    annotated.subarray(12).set(pixelview);
    this.onasset(id, annotated);
};

PargApp.prototype.start = function() {

    var cevents = {
        PAR_EVENT_DOWN: 0,
        PAR_EVENT_UP: 1,
        PAR_EVENT_MOVE: 2
    };

    var dims = this.module.par_window_dims || this.module.parg_window_dims;
    var $canvas = $(this.canvas);
    var canvas = $canvas[0];
    $canvas.css({
        width: dims[0] + 'px',
        height: dims[1] + 'px'
    });
    canvas.width = dims[0] * window.devicePixelRatio;
    canvas.height = dims[1] * window.devicePixelRatio;

    var GLctx = this.module.createContext(canvas, 1, 1, this.attribs);
    this.GLctx = GLctx;
    GLctx.clearColor(0.2, 0.4, 0.8, 1.0);
    GLctx.clear(GLctx.COLOR_BUFFER_BIT);
    GLctx.getExtension('OES_element_index_uint');
    $canvas.show();

    this.module.Window.init(this.args);

    var clientWidth = canvas.clientWidth;
    var clientHeight = canvas.clientHeight;
    var clientMaxY = clientHeight - 1;
    var app = this;

    var onmousecore = function(event) {
        if (app.block_interaction) {
            return;
        }
        var box = canvas.getBoundingClientRect();
        var x = (event.clientX - box.left) / clientWidth;
        var y = (clientMaxY - event.clientY + box.top) / clientHeight;
        var etype = event.type;
        var delta;
        if (etype == "mousedown") {
            this.Window.input(cevents.PAR_EVENT_DOWN, x, y, 0);
        } else if (etype == "mouseup") {
            this.Window.input(cevents.PAR_EVENT_UP, x, y, 0);
        } else if (etype == "mousemove") {
            this.Window.input(cevents.PAR_EVENT_MOVE, x, y, 0);
        } else if (etype == "mousewheel") {
            event.preventDefault();
            delta = event.wheelDelta / 10.0;
            this.Window.input(cevents.PAR_EVENT_MOVE, x, y, delta);
        } else if (etype == "DOMMouseScroll") {
            event.preventDefault();
            delta = -event.detail * 2.0;
            this.Window.input(cevents.PAR_EVENT_MOVE, x, y, delta);
        } else if (etype == "wheel") {
            event.preventDefault();
            delta = -event.deltaY / 2.0;
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
    canvas.addEventListener("wheel", onmouse);

    var raf = function() {
        var milliseconds = window.performance.now();
        var needs_draw = this.module.Window.tick(milliseconds / 1000.0, window.devicePixelRatio);
        if (needs_draw) {
            this.module.Window.draw();
        }
        window.requestAnimationFrame(raf);
    }.bind(this);

    window.requestAnimationFrame(raf);
};
