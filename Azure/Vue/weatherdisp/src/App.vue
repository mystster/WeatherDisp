<template>
  <div id="app" v-if="result">
    <div id="weatherToday">
      <Weather :dailyWeather="result.daily.data[0]" />
      <!-- <HelloWorld msg="Welcome to Your Vue.js + TypeScript App" /> -->
    </div>
    <div id="weatherTommorow">
      <Weather :dailyWeather="result.daily.data[1]" />
    </div>
    <div id="weatherTable">
      <Weather-Table :hourlyWeather="result.hourly" />
    </div>
  </div>
</template>

<script lang="ts">
import { Component, Vue } from "vue-property-decorator";
// import HelloWorld from "./components/HelloWorld.vue";
import Weather from "./components/Weather.vue";
import WeatherTable from "./components/WeatherTable.vue";
import {
  DarkSkyOptions,
  Units,
  Language,
  DarkSky,
  Forecast
} from "darkskyapi-ts";

@Component({
  components: {
    Weather,
    WeatherTable
  }
})
export default class App extends Vue {
  result: Forecast | false = false;
  async created() {
    const KEY = "xxxxxxxx";
    const lat = 111;
    const lng = 222;
    // https://api.darksky.net/forecast/xxxxxxxx/1110,222
    const options: DarkSkyOptions = {
      units: Units.SI,
      lang: Language.JAPANESE
    };
    const client = new DarkSky(KEY, options);

    this.result = await client
      .chain(lat, lng)
      .excludeFlags()
      .excludeAlerts()
      .excludeMinutely()
      .execute();

    console.log(this.result);
  }
}
</script>

<style>
body,
html {
  width: 100vw;
  height: 100vh;
}
body {
  margin: 0;
  background: pink;
  font-size: 12px;
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
  grid-template-columns: 50% 50%;
  grid-template-rows: 60% auto;
  width: 100vw;
  height: 100vh;
}
#app > * {
  border: 2px;
}
#weatherToday {
  grid-column: 1;
  grid-row: 1;
  background: orange;
}
#weatherTommorow {
  grid-column: 2;
  grid-row: 1;
  background: skyblue;
}
#weatherTable {
  grid-column: 1 / 3;
  grid-row: 2;
  background: violet;
}

</style>
