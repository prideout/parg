'using strict';

var parg = {};
var emmodule = null;
var nrequests = 0;
var requests = [];

parg.asset_preload = function(id) {
    var url = 'parg/' + id;
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'arraybuffer';
    var onloadFunc = function() {
        if (xhr.response) {
            var ptr = emmodule.Window.alloc(id, xhr.response.byteLength);
            var u8buffer = new Uint8Array(xhr.response);
            emmodule.HEAPU8.set(u8buffer, ptr);
            emmodule.Window.commit(id);
            if (--nrequests == 0) {

                // Start the game loop only after every asset is fetched:
                emmodule.parg.start(emmodule.par_window_dims);
            }
        }
    };
    var errorFunc = function() {
        window.console.error('Unable to download ' + url);
    };
    xhr.onload = onloadFunc;
    xhr.onerror = errorFunc;
    requests[nrequests++] = xhr;
};

// This allows main() to run and exit:
emmodule = CreatePar({parg: parg});

for (var i = 0, len = requests.length; i < len; i++) {
    requests[i].send(null);
}

$('#menu-toggle').click(function() {
    $('nav').toggle();
    GIZA.refreshSize();
});
