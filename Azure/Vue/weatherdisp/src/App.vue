<template>
  <div id="app" v-if="result">
    <div id="weatherToday">
      <Weather :dailyWeather="result.daily.data[0]"/>
      <!-- <HelloWorld msg="Welcome to Your Vue.js + TypeScript App" /> -->
    </div>
    <div id="weatherTommorow">
      <Weather :dailyWeather="result.daily.data[1]"/>
    </div>
    <div id="tempToday">
      ccccc
    </div>
    <div id="rainyToday">
      ddddddeee
    </div>
  </div>
</template>

<script lang="ts">
import { Component, Vue } from "vue-property-decorator";
// import HelloWorld from "./components/HelloWorld.vue";
import Weather from './components/Weather.vue';
import {DarkSkyClient, DarkSkyOptions, Units, Language, DarkSky, Forecast} from "darkskyapi-ts";

@Component({
  components: {
    Weather
  }
})
export default class App extends Vue {
  result:Forecast | false = false;
  async created(){
    const KEY = 'xxxxxxxx';
    const lat = 1110;
    const lng = 222;
    // https://api.darksky.net/forecast/xxxxxxxx/1110,222
    const options : DarkSkyOptions = {
      units: Units.SI,
      lang: Language.JAPANESE
    }
    const client = new DarkSky(KEY, options);

    this.result = await client.chain(lat, lng)
    .excludeFlags()
    .excludeAlerts()
    .excludeMinutely()
    .execute()
    
    console.log(this.result);

  }
}
</script>

<style>
body, html{
  width: 100vw;
  height: 100vh;
}
body{
  margin: 0;
  background: pink;
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
  grid-template-rows: 70% auto;
  width: 100vw;
  height: 100vh;
}
#app > * {
  border: 2px;
}
#weatherToday{
  grid-column: 1 / 2;
  grid-row: 1;
  background: orange;
}
#weatherTommorow{
  grid-column: 2 / 4;
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
