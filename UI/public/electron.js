import { app, BrowserWindow } from 'electron';
import { fileURLToPath } from 'url';
import path from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

let mainWindow;

async function createWindow() {
  try { 
    mainWindow = new BrowserWindow({ width: 900, height: 680});
    const isDev = (await import('electron-is-dev')).default; // Use dynamic import
    const startUrl = isDev ? 'http://localhost:3000' : `${path.join(__dirname, '../build/index.html')}`;
    mainWindow.loadURL(startUrl);
    // Remove the menu bar
    // mainWindow.setMenu(null);
    mainWindow.on('closed', () => (mainWindow = null));
  } catch (error) {
    console.error('Error creating window:', error);
  }
}


app.on('ready', createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});
