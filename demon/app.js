'use strict';
const electron = require("electron");
const url = require("url")
const { app } = electron;
const { BrowserWindow } = electron;
let win;
function createWindow() {
    win = new BrowserWindow({ width: 800, height: 600 , frame : true, audoHideMenuBar : true, useContentSize : true});

    //win.loadURL('${__dirname}/index.html');
    win.loadURL(url.format({
        pathname: __dirname + '/index.html', // important
        protocol: 'file:',
        slashes: true,
        // baseUrl: 'dist'
    }));
    win.webContents.openDevTools();
    win.on("closed", () => {
        win = null;
    });
}

app.on("ready", createWindow);
app.on("window-all-closed", () => {
    if (process.platform != 'darwin') {
        app.quit();
    }
});
