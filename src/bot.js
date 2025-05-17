const TelegramBot = require("node-telegram-bot-api");
const { initializeApp } = require("firebase/app");
const { getDatabase, ref, get } = require("firebase/database");

// Konfigurasi Firebase
const firebaseConfig = {
    apiKey: "AIzaSyCztyzTjcDiOZucr8elpd5rY4vodp52r1Y",
    authDomain: "kost-oren-kpri-sekawan.firebaseapp.com",
    databaseURL: "https://kost-oren-kpri-sekawan-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "kost-oren-kpri-sekawan",
    storageBucket: "kost-oren-kpri-sekawan.appspot.com",
    messagingSenderId: "874812411732",
    appId: "1:874812411732:web:888372a7035e1ed02ac2c1",
    measurementId: "G-TVS8VBFQV2"
};

const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

// Token Bot Telegram
const botToken = "7696439214:AAEd-Xmd8wKGTe0F3NXl2Bx-5OhRFHJcHM0";
const bot = new TelegramBot(botToken, { polling: true });

bot.onText(/\/GetData/, async (msg) => {
  const chatId = msg.chat.id;

  try {
    // Ambil data dari Firebase
    const snapshot = await get(ref(database, "realtime/dist"));
    if (!snapshot.exists()) {
      bot.sendMessage(chatId, "Tidak ada data tersedia.");
      return;
    }

    const Htank = 26.0; // cm
    const radius = 13.5; // cm
    const pi = 3.1416;

    const H_water = Htank - snapshot.val();
    const C_water = (H_water / Htank) * 100;
    const V_water = (pi * Math.pow(radius, 2) * H_water) / 1000;

    // Format pesan
    const message = `
📊 **Data Air Saat Ini**
💧 *Water Level:* ${H_water.toFixed(2)} cm
📏 *Capacity:* ${C_water.toFixed(2)} %
📦 *Volume:* ${V_water.toFixed(2)} L
    `;

    bot.sendMessage(chatId, message, { parse_mode: "Markdown" });
  } catch (error) {
    console.error("Error fetching data:", error);
    bot.sendMessage(chatId, "Terjadi kesalahan saat mengambil data.");
  }
});

console.log("Bot is running...");
