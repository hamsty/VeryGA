        // mount and sync persistent storage to browser
        Module["prepareFS"] = function () {
            if (!FS.analyzePath('/home/assets').exists)
                FS.mkdir('/home/assets')

            // FS.createLazyFile("/home/assets", "tile.mem", "/assets", true, true);
            // FS.createLazyFile("/home/assets", "labirinto.mem", "/assets", true, true);

            console.log(FS.readdir("/home/assets"))

            FS.chdir("/home")
 
            FS.syncfs(true, function (err) {
                if (err) {
                    Module.print(err);
                }
                removeRunDependency("prepareFS");
            });
        }
 
        Module.preRun = Module.preRun || [];
 
        Module.preRun.push(function(){
            addRunDependency("prepareFS");
            Module.prepareFS();
        })
 
      // sync browser filesystem to persistent storage and unmount
         globalThis.addEventListener('beforeunload', function(event) {
 
            FS.syncfs(false, function(err) {
                if (err) {
                    Module.print(err);
                } else {
                 console.log("success sync")
                }
                FS.unmount('/home/assets');
            });
        }, false);