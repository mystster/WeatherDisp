<template>
  <div class="weather" id="weatherPane">
    <div id="date">
      {{getLocalDateString(dailyWeather.time)}}
    </div>
    <div id="icon">
      <skycon :condition="dailyWeather.icon"></skycon>
    </div>
    <div id="maxTempLabel">高</div>
    <div id="minTempLabel">低</div>
    <div id="rainyLabel">雨</div>
    <div id="windLabel">風</div>
    <div id="maxTemp">{{ Math.round(dailyWeather.temperatureMax) }}({{Math.round(dailyWeather.apparentTemperatureMax)}})℃</div>
    <div id="minTemp">{{ Math.round(dailyWeather.temperatureMin) }}({{Math.round(dailyWeather.apparentTemperatureMin)}})℃</div>
    <div id="rainy">
      {{ Math.round(dailyWeather.precipProbability * 10) * 10 }}%
    </div>
    <div id="wind">{{direction}}{{ Math.round(dailyWeather.windSpeed) }}m/s</div>
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
#weatherPane {
  display: grid;
  grid-template-columns: 54px 0.3fr 1.3fr;
  grid-template-rows: 1fr 1fr 1fr 1fr;
  height: 100%;
  margin: 0;
}
.weather {
  div {
    margin-top: -3px;
    margin-bottom: -3px;
  }
}

#date {
  grid-column: 1;
  grid-row: 1;
  margin-top: 0px;
}

#icon {
  grid-column: 1;
  grid-row: 2 / 4;
  canvas{
    width: $iconsize;
    height: $iconsize;
  }
}

#maxTempLabel {
  grid-column: 2;
  grid-row: 1;
  margin-top: 0px;
}
#minTempLabel {
  grid-column: 2;
  grid-row: 2;
}
#rainyLabel {
  grid-column: 2;
  grid-row: 3;
}
#windLabel {
  grid-column: 2;
  grid-row: 4;
  margin-bottom: 0px;
}
#maxTemp {
  grid-column: 3;
  grid-row: 1;
  margin-top: 0px;
}
#minTemp {
  grid-column: 3;
  grid-row: 2;
}
#rainy {
  grid-column: 3;
  grid-row: 3;
}
#wind {
  grid-column: 3;
  grid-row: 4;
  margin-bottom: 0px;
}
</style>
