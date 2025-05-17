<template>
    <div class="tank">
      <div class="water" :style="{ height: waterHeight + '%' }">
        <div class="percentage">{{ waterHeight }}%</div>
      </div>
    </div>
  </template>
  
  <script>




  export default {
    props: {
      waterHeight: Number,
    },
  };
  </script>
  
  <style scoped>
  .tank {
    width: 100px;
    height: 200px;
    border: 3px solid #333;
    position: relative;
    border-radius: 50px;
    overflow: hidden;
    display: flex;
    justify-content: center;
    align-items: flex-end;
    background: linear-gradient(135deg, #ffffff, #ebeff5);
  }
  
  .water {
    width: 100%;
    background: linear-gradient(135deg, #4f9cff, #0073e6);
    position: absolute;
    bottom: 0;
    transition: height 0.5s ease-in-out;
    display: flex;
    justify-content: center;
    align-items: center;
  }
  
  .percentage {
    color: white;
    font-weight: bold;
    font-size: 20px;
    text-shadow: 1px 1px 3px rgba(0,0,0,0.5);
  }
  </style>
  