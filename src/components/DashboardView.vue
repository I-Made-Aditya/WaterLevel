<template>
  <div class="dashboard">
    <div class="card timestamp-card">
      <p>🕒 Timestamp</p>
      <p>{{ formattedTimestamp }}</p>
    </div>

    <div class="card water-card">
      <WaterTank @updateWaterCapacity="checkWaterLevel" />
    </div>

    <div class="card relay-card">
      <label>⚡ Relay Control</label>
      <div class="switch">
        <input type="checkbox" id="relaySwitch" :checked="relayState" @click="toggleRelay">
        <label for="relaySwitch" class="slider"></label>
      </div>
      <p class="status">{{ relayState ? "ON" : "OFF" }}</p>
    </div>
  </div>
</template>

<script>
import { database, ref, onValue, set } from "../firebase";
import WaterTank from "./WaterTank.vue";

export default {
  components: { WaterTank },
  data() {
    return {
      formattedTimestamp: "Loading...",
      relayState: false,
      C_water: 0,
      relayTriggered: false // Flag untuk mencegah pemicu berulang
    };
  },
  created() {
    const relayRef = ref(database, "realtime/relay");

    onValue(relayRef, (snapshot) => {
      const relayValue = snapshot.val();
      console.log("🔥 Relay state dari Firebase:", relayValue);
      if (relayValue !== null) {
        this.relayState = !relayValue; // Membalik nilai relay untuk UI
      }
    });

    // Mengambil waktu dari sistem pengguna setiap detik
    this.updateLocalTimestamp();
  },
  methods: {
    updateLocalTimestamp() {
      setInterval(() => {
        const now = new Date();
        this.formattedTimestamp = this.convertUnixToDate(now.getTime() / 1000);
        console.log("📅 Timestamp lokal diperbarui:", this.formattedTimestamp);

        // Cek apakah waktu sudah pukul 20:00
        if (now.getHours() === 21 && now.getMinutes() === 0 && !this.relayTriggered) {
        console.log("⏰ Waktu sudah 20:00, mengaktifkan relay...");
        this.triggerRelay();
        this.relayTriggered = true;
        }
      }, 1000); // Update setiap 1 detik
    },

    convertUnixToDate(unix) {
      const date = new Date(unix * 1000);
      return date.toLocaleString("en-US", {
        weekday: "short",
        year: "numeric",
        month: "long",
        day: "2-digit",
        hour: "2-digit",
        minute: "2-digit",
        second: "2-digit",
        hour12: false
      });
    },
    toggleRelay() {
      const newState = !this.relayState;
      this.relayState = newState;

      const relayRef = ref(database, "realtime/relay");
      set(relayRef, !newState)
        .then(() => console.log("✅ Relay state updated in Firebase:", !newState))
        .catch((error) => console.error("❌ Error updating relay state:", error));
    },
    triggerRelay() {
      const relayRef = ref(database, "realtime/relay");
      set(relayRef, false)
        .then(() => console.log("✅ Relay diaktifkan otomatis pada pukul 20:00"))
        .catch((error) => console.error("❌ Error updating relay state:", error));

      this.relayState = true;
    },
    checkWaterLevel(C_water) {
      this.C_water = C_water;
      console.log("💧 Kapasitas air:", C_water + "%");

      if (C_water > 90) {
        console.log("🚨 Air terlalu penuh! Mengaktifkan pompa...");
        const relayRef = ref(database, "realtime/relay");
        set(relayRef, true)
          .then(() => console.log("✅ Relay diaktifkan otomatis karena air penuh"))
          .catch((error) => console.error("❌ Error updating relay state:", error));

        this.relayState = false;
      }
    }
  }
};
</script>

<style scoped>
.dashboard {
  display: flex;
  flex-direction: column;
  gap: 20px;
  align-items: center;
}

.card {
  background: linear-gradient(135deg, #ffffff, #dce2ee);
  padding: 15px;
  border-radius: 10px;
  box-shadow: 2px 4px 10px rgba(0, 0, 0, 0.15);
  width: 280px;
  text-align: center;
}

.water-card {
  display: flex;
  flex-direction: column;
  align-items: center;
}

.relay-card {
  display: flex;
  flex-direction: column;
  align-items: center;
}

.timestamp-card {
  font-weight: bold;
  font-size: 18px;
}

.switch {
  position: relative;
  width: 80px;
  height: 40px;
}

.switch input {
  opacity: 0;
  width: 0;
  height: 0;
}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  border-radius: 34px;
  transition: 0.4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 32px;
  width: 32px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  border-radius: 50%;
  transition: 0.4s;
}

input:checked+.slider {
  background-color: #4f9cff;
}

input:checked+.slider:before {
  transform: translateX(40px);
}

.status {
  margin-top: 10px;
  font-size: 16px;
  font-weight: bold;
  color: #333;
}
</style>
