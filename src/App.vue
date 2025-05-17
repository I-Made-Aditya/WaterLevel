<template>
  <div id="app">
    <DashboardView />
  </div>
</template>

<script>
import DashboardView from "./components/DashboardView.vue";

export default {
  components: { DashboardView }
};
</script>

<style>
#app {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 100dvh;
  background-color: #f4f4f4;
}
</style>
 