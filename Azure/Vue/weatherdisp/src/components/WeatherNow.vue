<template>
  <div class="weatherNow" id="weatherNowPane">
    <div id="dateAndIcon">
      {{localTimeString}}現在
    </div>
    <div id="temp">
      外:{{ Math.round(currentlyWeather.temperature) }}℃
    </div>
    <div id="humidity">
      / {{ Math.round(currentlyWeather.precipProbability * 100)}}%
    </div>
  </div>
</template>

<script lang="ts">
import { Component, Prop, Vue } from "vue-property-decorator";
import { DataPoint } from "darkskyapi-ts";
import * as Util from "../util";

@Component
export default class WeatherNow extends Vue {
  @Prop() private currentlyWeather?: DataPoint;
  get localTimeString(): string {
    const date = new Date((this.currentlyWeather?.time ?? 0) *1000);
    return date.toLocaleTimeString("ja-JP",{
      hour: "numeric",
      minute: "numeric"
    });
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style lang="scss" scoped>
.weatherNow {
  div {
    display: inline;
  }
}
</style>
