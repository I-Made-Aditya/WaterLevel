<template>
    <div class="tank-container">
      <WaterLevel :waterHeight="parseFloat(waterHeight.toFixed(2))" />
      <div class="info">
        <p>💧 Water Level: {{ H_water.toFixed(2) }} cm</p>
        <p>📦 Volume: {{ V_water.toFixed(2) }} liters</p>
      </div>
    </div>
</template>
  
<script>
import { database, ref, onValue } from "../firebase";
// import { sendMessage } from '../telegram';
import WaterLevel from "./WaterLevel.vue";

export default {
  components: { WaterLevel },
  data() {
    return {
      waterHeight: 50, // Default percentage
      H_water: 0,
      V_water: 0,
      Htank: 26.0, // cm
      pi: 3.1416,
      radius: 13.5 // cm
    };
  },
  created() {
    const waterRef = ref(database, "realtime/dist");
    onValue(waterRef, (snapshot) => {
      const data = snapshot.val();
      if (data !== null) {
        this.H_water = this.Htank - data;
        const C_water = (this.H_water / this.Htank) * 100;
        this.V_water = (this.pi * Math.pow(this.radius, 2) * this.H_water) / 1000;
        this.waterHeight = parseFloat(C_water.toFixed(2));
      }
    });
  },
  // testBot() {
  //  const chatId = 1305523182;
  //  sendMessage(chatId, "Tes successfully from Vue.js!")
  //     .catch(err => console.error("Error sending test message:", err));
  // },
};
</script>

<style scoped>
.tank-container {
  display: flex;
  flex-direction: column;
  align-items: center;
}

.info {
  margin-top: 10px;
  text-align: center;
}

p {
  font-size: 16px;
  font-weight: bold;
}
</style>
  