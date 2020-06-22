<template>
  <div class="weatherNow" id="weatherNowPane">
    <div id="dateAndIcon">{{ localTimeString }}現在</div>
    <div id="temp">外:{{ Math.round(currentlyWeather.temperature) }}℃</div>
    <div id="humidity">
      / {{ Math.round(currentlyWeather.humidity * 100) }}%
    </div>
  </div>
</template>

<script lang="ts">
import { Component, Prop, Vue } from 'vue-property-decorator';
import { DataPoint } from 'darkskyapi-ts';
import DateWithOffset from 'date-with-offset';

@Component
export default class WeatherNow extends Vue {
  @Prop() private currentlyWeather?: DataPoint;
  get localTimeString(): string {
    const date = new DateWithOffset(
      (this.currentlyWeather?.time ?? 0) * 1000,
      540
    );
    return date.localDate().toLocaleTimeString('ja-JP', {
      hour: '2-digit',
      minute: '2-digit'
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
