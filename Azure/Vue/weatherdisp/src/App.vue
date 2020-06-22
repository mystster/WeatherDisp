<template>
  <div id="app" v-if="result">
    <div id="weatherToday">
      <Weather :dailyWeather="result.daily.data[0]" />
    </div>
    <div id="weatherTommorow">
      <Weather :dailyWeather="result.daily.data[1]" />
    </div>
    <div id="weatherTable">
      <Weather-Table :hourlyWeather="result.hourly" />
    </div>
    <div id="weatherNow">
      <Weather-Now :currentlyWeather="result.currently"></Weather-Now>
    </div>
  </div>
</template>

<script lang="ts">
import { Component, Vue } from 'vue-property-decorator';
import Weather from './components/Weather.vue';
import WeatherTable from './components/WeatherTable.vue';
import WeatherNow from './components/WeatherNow.vue';
import {
  DarkSkyOptions,
  Units,
  Language,
  DarkSky,
  Forecast
} from 'darkskyapi-ts';

@Component({
  components: {
    Weather,
    WeatherTable,
    WeatherNow
  }
})
export default class App extends Vue {
  result: Forecast | false = false;
  async created() {
    const KEY = process?.env?.VUE_APP_DARKSKY_KEY ?? '';
    const lat = process?.env?.VUE_APP_LAT ?? 0;
    const lng = process?.env?.VUE_APP_LAG ?? 0;
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
@import url('http://mplus-webfonts.sourceforge.jp/mplus_webfonts.css');
body,
html {
  width: 100vw;
  height: 100vh;
}
body {
  margin: 0;
  font-size: 12px;
  font-family: 'mplus-1p-bold', sans-serif;
}
::-webkit-scrollbar {
  display: none;
}
</style>
<style scoped>
#app {
  display: grid;
  grid-template-columns: 50% 50%;
  grid-template-rows: 50px 58px auto;
  width: 100vw;
  height: 100vh;
}

#weatherToday {
  grid-column: 1;
  grid-row: 1;
}
#weatherTommorow {
  grid-column: 2;
  grid-row: 1;
}
#weatherTable {
  grid-column: 1 / 3;
  grid-row: 2;
}
#weatherNow {
  grid-column: 1 / 3;
  grid-row: 3;
}
</style>
