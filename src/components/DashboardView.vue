<template>
  <!-- just testing -->
  <!-- <button @click="showNotification"> Test Notif </button>
  <notifications/> -->

  <div class="dashboard">
    <div class="card timestamp-card">
      <p>🕒 Timestamp</p>
      <p>{{ formattedTimestamp }}</p>
    </div>

    <div class="card water-card">
      <WaterTank />
    </div>

    <div class="card relay-card">
      <label>⚡ Relay Control</label>
      <div class="switch">
        <input type="checkbox" id="relaySwitch" v-model="relayState" @change="toggleRelay">
        <label for="relaySwitch" class="slider"></label>
      </div>
      <p class="status">{{ relayState ? "ON" : "OFF" }}</p>
    </div>
  </div>
</template>

<script>
import { database, ref, onValue } from "../firebase";
import WaterTank from "./WaterTank.vue";
// import { useNotification } from '@kyvg/vue3-notification';

export default {
  components: { WaterTank },
  data() {
    return {
      timestamp: null, // Data dari Firebase
      formattedTimestamp: "Loading...",
      relayState: false
    };
  },
  created() {
    // Referensi ke Firebase path "realtime/timestamps"
    const timestampRef = ref(database, "realtime/timestamps");

    onValue(timestampRef, (snapshot) => {
      const unixTimestamp = snapshot.val();
      // console.log("🔥 Unix Timestamp dari Firebase:", unixTimestamp);
      if (unixTimestamp) {
        this.timestamp = unixTimestamp;
        this.formattedTimestamp = this.convertUnixToDate(unixTimestamp);
        // console.log("✅ Timestamp setelah konversi:", this.formattedTimestamp);
      }
    });
  },
  methods: {
    convertUnixToDate(unix) {
      const date = new Date(unix * 1000); // Konversi ke milidetik langsung tanpa manipulasi zona waktu
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
      console.log("Relay state:", this.relayState);
    },
    // showNotification() {
    //   const { notify } = useNotification();
    //   notify({
    //     title: "Important message",
    //     text: "Hello user! This is a notification.",
    //     type: 'success',
    //     duration: 5000,
    //   });
    // }
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
  /* transition: transform 0.3s ease-in-out; */
}

/* .card:hover {
  transform: scale(1.05);
} */

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

/* Switch UI */
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
