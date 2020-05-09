<template>
  <div class="weather" id="weatherPane">
    <div id="dateAndIcon">
      {{getLocalDateString(dailyWeather.time)}}<br />
      <skycon :condition="dailyWeather.icon"></skycon>
    </div>
    <div id="temp">{{ Math.round(dailyWeather.temperatureMin) }}/{{ Math.round(dailyWeather.temperatureMax) }}℃</div>
    <div id="rainy">
      {{ Math.round(dailyWeather.precipProbability * 10) * 10 }}%
    </div>
    <div id="wind">
      {{direction}} {{ Math.round(dailyWeather.windSpeed) }}m/s
    </div>
  </div>
</template>

<script lang="ts">
import { Component, Prop, Vue } from "vue-property-decorator";
import { DataPoint } from "darkskyapi-ts";
import * as Util from "../util";
import DateWithOffset from "date-with-offset"

@Component
export default class Weather extends Vue {
  @Prop() private dailyWeather?: DataPoint;
  get direction(): string {
    return Util.derectionFromDegree(this.dailyWeather?.windBearing ?? 0);
  }
  getLocalDateString(tick: number): string {
    const date = new DateWithOffset(tick*1000, 540);
    return date.localDate().toLocaleDateString("ja-JP", {
      month: "numeric",
      day: "numeric"
    });
  }

}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style lang="scss" scoped>
$iconsize: 40px;
$fontsize: 18px;
$marginsize: $fontsize * -0.3;
#weatherPane {
  display: grid;
  grid-template-columns: $iconsize * 1.05 auto;
  grid-template-rows: 1fr 1fr 1fr;
  height: 100%;
  margin: 0;
}
.weather {
  div {
    margin-top: $marginsize;
    margin-bottom: $marginsize;
  }
}

#dateAndIcon {
  grid-column: 1;
  grid-row: 1 / 3;
  margin-top: -3px;
  canvas{
    width: $iconsize;
    height: $iconsize;
  }
}

#temp {
  grid-column: 2;
  grid-row: 1;
  font-size: $fontsize;
}
#rainy {
  grid-column: 2;
  grid-row: 2;
  font-size: $fontsize;
}
#wind {
  grid-column: 2;
  grid-row: 3;
  font-size: $fontsize;
}
</style>
