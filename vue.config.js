const { defineConfig } = require('@vue/cli-service');

module.exports = defineConfig({
  transpileDependencies: true,
  publicPath: "./", // Pastikan path relatif agar Firebase membaca dengan benar
  outputDir: "dist", // Pastikan hasil build ada di folder `dist`
  assetsDir: "static" // Menyimpan aset dalam `static` agar lebih rapi
});
