<template>
  <div class="weather" id="weatherPane">
      <div id="date">{{new Date(dailyWeather.time*1000).toLocaleDateString("ja-JP",{month: 'numeric', day: 'numeric' })}}</div>
      <div id="icon">
        <skycon :condition="dailyWeather.icon" width="60" height="60"></skycon>
      </div>
      <div id="maxTempLabel">高</div>
      <div id="minTempLabel">低</div>
      <div id="rainyLabel">雨</div>
      <div id="windLabel">風</div>
      <div id="maxTemp">{{Math.round(dailyWeather.temperatureHigh)}}℃</div>
      <div id="minTemp">{{Math.round(dailyWeather.temperatureLow)}}℃</div>
      <div id="rainy">{{Math.round(dailyWeather.precipProbability*10)*10}}%</div>
      <div id="wind">{{Math.round(dailyWeather.windSpeed)}}m/s</div>
  </div>
</template>

<script lang="ts">
import { Component, Prop, Vue } from "vue-property-decorator";
import {Forecast, DataBlock, } from 'darkskyapi-ts'

@Component
export default class Weather extends Vue {
  @Prop() private dailyWeather?: DataBlock
  
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
#weatherPane {
  display: grid;
  grid-template-columns: 1.5fr 0.5fr 1fr;
  grid-template-rows: 1fr 1fr 1fr 1fr;
  height: 100%;
  margin: 0;
}
#weatherPane > * {
  margin: 0;
}
#date{
  grid-column: 1;
  grid-row: 1;
}
#icon{
  grid-column: 1;
  grid-row: 2 / 4;
}
#maxTempLabel{
  grid-column: 2;
  grid-row: 1;
  font-size-adjust: inherit;
}
#minTempLabel{
  grid-column: 2;
  grid-row: 2;
}
#rainyLabel{
  grid-column: 2;
  grid-row: 3;
}
#windLabel{
  grid-column: 2;
  grid-row: 4;
}
#maxTemp{
  grid-column: 3;
  grid-row: 1;
  font-size:100%;
}
#minTemp{
  grid-column: 3;
  grid-row: 2;
}
#rainy{
  grid-column: 3;
  grid-row: 3;
}
#wind{
  grid-column: 3;
  grid-row: 4;
}
</style>
