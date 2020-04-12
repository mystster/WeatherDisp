<template>
  <div class="weatherTable" id="weatherTablePane">
      <table>
          <tr>
              <th v-for="item in tableData" :key="item.time">
                  {{new Date(item.time*1000).getHours()}}
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
//import * as Util from "../util";

@Component
export default class WeatherTable extends Vue {
  @Prop() private hourlyWeather?: DataBlock;
  get tableData(): DataPoint[] | undefined {
      return this.hourlyWeather?.data.filter((d, i) => i % 2 === 0 && i/2 < 7);
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
table{
    border: solid 1px black;
    border-collapse: collapse;
    border-spacing: 0;
    width: 100%;
}
</style>
