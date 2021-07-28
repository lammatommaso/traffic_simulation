//const { worker } = require("cluster");

const { ENETDOWN } = require("constants");
const { ADDRCONFIG } = require("dns");
const electron = require("electron");
const app = electron.app // require('app');
const BrowserWindow = electron.BrowserWindow // require('browser-window')
const ipcMain = electron.ipcMain //require("ipcMain")
const { shell, dialog } = require("electron")
var path = require('path');

// module.paths.push(path.resolve('node_modules'));
// module.paths.push(path.resolve(__dirname, 'javascript'));
// module.paths.push(path.resolve('javascript'));
// module.paths.push(path.resolve(__dirname, '..', '..', 'electron', 'node_modules'));
// module.paths.push(path.resolve(__dirname, '..', '..', 'electron.asar', 'node_modules'));
// module.paths.push(path.resolve(__dirname, '..', '..', 'app', 'node_modules'));
// module.paths.push(path.resolve(__dirname, '..', 'app.asar', 'javascript'));

// const { app, BrowserWindow, ipcMain } = require("electron")
const fs = require('fs');
const { on } = require("process");

// fs.readdir(__dirname, (err, files) => {
//     files.forEach(file => {
//       console.log(file);
//       fs.readdir(path.join(__dirname, file), (err, files) => {
//         files.forEach(file => {
//           console.log("|--- " + file);
//         });
//       });
//     });
//   });
console.log(app.getAppPath())

fs.readdirSync(__dirname).forEach(file => {
    console.log(file);
    try {
        fs.readdirSync(path.join(__dirname, file)).forEach(file => {
            console.log("|---" + file)
        })
    } catch (e){
        console.log("")
    }
});

var mainWindow = BrowserWindow
var v = ""
var w = ""

function createWindow() {
    mainWindow = new BrowserWindow({
        height: 1024,
        width: 1280,
        webPreferences: {
            nodeIntegration: true
        },
        resizable: false
    })

    mainWindow.loadFile('html/start.html')

    mainWindow.on('closed', () => { 
        // if (v){
            // v.terminate();
            // console.log("termino v...")
        // }
        // if (w){
            // console.log("termino w...")
            // w.terminate();
        // }
        // app.quit()
    })

}

app.whenReady().then( () => {
    createWindow()
})


const { Worker, workerData, parentPort } = require('worker_threads')

var batch_init = true

ipcMain.on("batch_init", (event, base_dir_s, simulation_type, 
        p, cols, rows, min_car, max_car, increment, 
        car_number, gaussian_mean, gaussian_sigma, min_road_length, 
        max_road_length) => {
    if (batch_init){
        const prom = new Promise((resolve, reject) => {
            console.log("Attivo serviceBatch")


            v = new Worker(path.join(__dirname, "javascript", "serviceBatch.js"), {workerData: {"base_dir": base_dir_s, "simulation_type": simulation_type, 
            "p": p, "cols": cols, "rows": rows, "min_car": min_car, "max_car": max_car, "increment": increment, 
            "n_cars": car_number, "gaussian_mean": gaussian_mean, "gaussian_sigma": gaussian_sigma, "min_road_l": min_road_length, 
            "max_road_l": max_road_length, "str_len": base_dir_s.length + 1}} );
            
            console.log("Worker serviceBatch triggerato")

            v.on('error', reject);
            v.on('exit', (code) => {
            if (code !== 0)
                reject(new Error(`Worker stopped with exit code ${code}`));
            })
            
            console.log("Thread creato")
            
            v.addListener("message", (data)=>{
                console.log("Ricevuto messaggio da thread simulazione")
                console.log(data)
                event.reply("sim_update", data)
                //event.reply(data["name"], data["data"])
            })
            
        });
        
        

        prom.then(() => {
            console.log("finito")
        })
        batch_init = false
    } else {
        v.postMessage({"base_dir": base_dir_s, "simulation_type": simulation_type, 
        "p": p, "cols": cols, "rows": rows, "min_car": min_car, "max_car": max_car, "increment": increment, 
        "n_cars": car_number, "gaussian_mean": gaussian_mean, "gaussian_sigma": gaussian_sigma, "min_road_l": min_road_length, 
        "max_road_l": max_road_length, "str_len": base_dir_s.length + 1})
    }

})
 
ipcMain.on("init", (event, increment, n_cars,  rows, columns, gaussian_mean, gaussian_sigma, min_road_l, max_road_l, lanes) => {
    
    const prom = new Promise((resolve, reject) => {
        w = new Worker(path.join(__dirname, 'javascript', 'service.js'), {workerData: {"increment": increment, "n_cars": n_cars, "rows": rows, 
        "columns": columns, "gaussian_mean": gaussian_mean, "gaussian_sigma": gaussian_sigma, 
            "min_road_l": min_road_l, "max_road_l": max_road_l, "lanes": lanes}} );
        //w.on('message', resolve);
        w.on('error', reject);
        w.on('exit', (code) => {
        if (code !== 0)
            reject(new Error(`Worker stopped with exit code ${code}`));
        })
        
        console.log("Thread creato")
        
        w.addListener("message", (data)=>{
            console.log("Ricevuto messaggio da thread simulazione")
            
            event.reply(data["name"], data["data"])
        })

    });

    prom.then(() => {
        console.log("finito")
    })
    
})

ipcMain.on("next", (event) => {
    w.postMessage({"name": "next"})
})

ipcMain.on("create_path", (event, s1, s2) => {
    //s1 è il primo insieme => insieme dei nodi di partenza
    //s2 è il secondo insieme => insieme dei nodi di arrivo
    w.postMessage({"name": "create_path", "data": {"l1": s1.length, "l2": s2.length, "s1": s1, "s2": s2}})
})

ipcMain.on("start_simulation", (event) => {
    w.postMessage({"name": "start_simulation"})
})

ipcMain.on("select_folder", async (event) => {

    const result = await dialog.showOpenDialog(mainWindow, {
      properties: ['openDirectory']
    })

    event.reply("folder_selected", result.filePaths)
})

ipcMain.on("open_folder", (event, folder) => {
    shell.openPath(folder) // Open the given file in the desktop's default manner.
})