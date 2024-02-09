/**
 * The preload script runs before. It has access to web APIs
 * as well as Electron's renderer process modules and some
 * polyfilled Node.js functions.
 *
 * https://www.electronjs.org/docs/latest/tutorial/sandbox
 */
 
window.addEventListener('DOMContentLoaded', () => {
  const replaceText = (selector, text) => {
    const element = document.getElementById(selector)
    if (element) element.innerText = text
  }

  for (const type of ['chrome', 'node', 'electron']) {
    replaceText(`${type}-version`, process.versions[type])
  }
})

const { contextBridge, ipcRenderer, remote } = require('electron');

ipcRenderer.on('config-data', (event, config) => {
  console.log("Debug 1", config);
  window.configData = config;
});

contextBridge.exposeInMainWorld('electronApi', {
  sendMessage: (name, message=undefined) => {
    ipcRenderer.send(name, message);
  },
  getConfig: () => {
    return window.configData;
  },
  openDevTools: () => {
    remote.getCurrentWebContents().openDevTools();
  }
});