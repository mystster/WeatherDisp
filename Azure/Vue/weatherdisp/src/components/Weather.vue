<template>
  <div class="weather">
      <div id="date"></div>
      <div id="icon">
        <skycon condition="rain" width="40" height="40"></skycon>
      </div>
      <div id="maxTempLabel">最高気温</div>
      <div id="minTempLabel">最低気温</div>
      <div id="rainyLabel">降水確率</div>
      <div id="windLabel">風速</div>
      <div id="maxTemp"></div>
      <div id="minTemp"></div>
      <div id="rainy"></div>
      <div id="wind"></div>
  </div>
</template>

<script lang="ts">
import { Component, Prop, Vue } from "vue-property-decorator";
// import {DayForecast} from 'darkskyapi-ts'

@Component
export default class Weather extends Vue {
//  @Prop() private weatherInfo: DayForecast;
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h3 {
  margin: 40px 0 0;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}
a {
  color: #42b983;
}
</style>
