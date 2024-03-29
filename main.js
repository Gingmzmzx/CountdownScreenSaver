// Modules to control application life and create native browser window
const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('node:path');
const fs = require('fs');

function createWindow () {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    },
	fullscreenable:true,
    fullscreen: true,
    simpleFullscreen:true,
  })

   // 读取配置文件
   const configPath = path.join(__dirname, 'data.json');
   const configData = fs.readFileSync(configPath, 'utf-8');
   const config = JSON.parse(configData);
 
   // 将配置数据传递给渲染进程
  mainWindow.webContents.send('config-data', config);
  
  mainWindow.setSimpleFullScreen(true);
  mainWindow.loadFile('index.html');
  // mainWindow.webContents.openDevTools();
  mainWindow.setMenu(null);

  ipcMain.on("openDevTools", (event, message)=>{
    mainWindow.webContents.openDevTools();
  });
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  createWindow()

  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

ipcMain.on("exit", (event, message)=>{
  app.quit();
});


// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', function () {
  try { globalShortcut.unregisterAll(); } catch(e) {}
  if (process.platform !== 'darwin') app.quit()
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.