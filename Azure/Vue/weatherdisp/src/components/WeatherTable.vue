<template>
  <div class="weatherTable" id="weatherTablePane">
      <table>
          <tr>
              <th v-for="item in tableData" :key="item.time">
                  {{getLocalHour(item.time)}}
                    <skycon id="skycon" :condition="item.icon" width="17" height="17"></skycon>
              </th>
          </tr>
          <tr>
              <td v-for="item in tableData" :key="item.time">
                  {{Math.round(item.temperature)}}({{Math.round(item.apparentTemperature)}})
              </td>
          </tr>
          <tr>
              <td v-for="item in tableData" :key="item.time">
                  {{Math.round(item.precipProbability * 10) * 10 }}%
              </td>
          </tr>
          <tr>
              <td v-for="item in tableData" :key="item.time">
                  {{ Math.round(item.windSpeed)}}
              </td>
          </tr>
      </table>
  </div>
</template>

<script lang="ts">
import { Component, Prop, Vue } from "vue-property-decorator";
import { DataBlock, DataPoint } from "darkskyapi-ts";
import DateWithOffset from "date-with-offset";
import * as Util from "../util";

@Component
export default class WeatherTable extends Vue {
  @Prop() private hourlyWeather?: DataBlock;
  get tableData(): DataPoint[] | undefined {
      return this.hourlyWeather?.data.filter((d, i) => i % 2 === 0 && i/2 < 7);
  }
  getLocalHour(tick: number): number {
      return new DateWithOffset(tick*1000, 540).getHours();
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
table {
    border-collapse: collapse;
    border-spacing: 0;
    width: 100%;
}
th, td{
    padding: 0px;
    border: solid 1px black;
}
#skycon {
    margin: -1px 0px -5px 0px;
}
</style>
