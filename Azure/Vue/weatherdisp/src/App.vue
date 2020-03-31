<template>
  <div id="app">
    <div id="weatherToday">
      aaaa
      <!-- <HelloWorld msg="Welcome to Your Vue.js + TypeScript App" /> -->
    </div>
    <div id="weatherTommorow">
      <Weather/>
    </div>
    <div id="tempToday">
      ccccc
    </div>
    <div id="rainyToday">
      dddddd
    </div>
  </div>
</template>

<script lang="ts">
import { Component, Vue } from "vue-property-decorator";
// import HelloWorld from "./components/HelloWorld.vue";
import Weather from './components/Weather.vue';

@Component({
  components: {
    Weather
  }
})
export default class App extends Vue {}
</script>

<style>
body, html{
  width: 100%;
  height: 100%;
}
body{
  margin: 0;
}
</style>
<style scoped>
#app {
  /* font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px; */
  display: grid;
  grid-template-columns: 50% 10% 40%;
  grid-template-rows: 60% 40%;
}
#app > * {
  border: 2px;
}
#weatherToday{
  grid-column: 1 / 3;
  grid-row: 1;
  background: orange;
}
#weatherTommorow{
  grid-column: 3;
  grid-row: 1;
  background: skyblue;
}
#tempToday{
  grid-column: 1;
  grid-row: 2;
  background: violet;
}
#rainyToday{
  grid-column: 2 / 4;
  grid-row: 2;
  background:yellow;
}

</style>
