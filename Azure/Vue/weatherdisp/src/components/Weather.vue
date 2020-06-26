<template>
  <div class="weather" id="weatherPane">
    <div id="dateAndIcon">
      {{ getLocalDateString(dailyWeather.time) }}<br />
      <skycon :condition="dailyWeather.icon"></skycon>
    </div>
    <div id="temp">
      {{ Math.round(dailyWeather.temperatureMin) }}/{{
        Math.round(dailyWeather.temperatureMax)
      }}℃
    </div>
    <div id="rainy">
      {{ hourlyPrecipProbability }}
    </div>
    <div id="wind">
      {{ direction }} {{ Math.round(dailyWeather.windSpeed) }}m/s
    </div>
  </div>
</template>

<script lang="ts">
import { Component, Prop, Vue } from 'vue-property-decorator';
import { DataPoint, DataBlock } from 'darkskyapi-ts';
import * as Util from '../util';
import DateWithOffset from 'date-with-offset';

type HourlyPrecipProbability = {
  hour: number;
  precip: number;
};

@Component
export default class Weather extends Vue {
  @Prop() private dailyWeather?: DataPoint;
  @Prop() private hourlyWeather?: DataBlock;
  get direction(): string {
    return Util.derectionFromDegree(this.dailyWeather?.windBearing ?? 0);
  }
  getLocalDateString(tick: number): string {
    const date = new DateWithOffset(tick * 1000, 540);
    return date.localDate().toLocaleDateString('ja-JP', {
      month: 'numeric',
      day: 'numeric'
    });
  }
  get hourlyPrecipProbability(): string {
    if (!this.hourlyWeather || !this.dailyWeather) {
      return '';
    }
    const todayHourlyPrecipProbability: HourlyPrecipProbability[] = this.hourlyWeather.data
      .filter(
        x =>
          x.time >= (this.dailyWeather?.time ?? 0) &&
          x.time < (this.dailyWeather?.time ?? 0) + 24 * 60 * 60
      )
      .map(x => {
        const d = new DateWithOffset(x.time * 1000, 540);
        return {
          hour: d.localDate().getHours(),
          precip: Math.round((x?.precipProbability ?? 0) * 10) * 10
        };
      });

    const maxPrecipProbability = (
      startHour: number,
      endHour: number
    ): string | number =>
      todayHourlyPrecipProbability
        .filter(x => x.hour >= startHour && x.hour < endHour)
        .reduce((prev: string | number, cur: HourlyPrecipProbability):
          | string
          | number => {
          if (typeof prev === 'string') {
            return cur.precip;
          } else {
            return Math.max(prev, cur.precip);
          }
        }, '-');
    console.dir(todayHourlyPrecipProbability);
    return `${maxPrecipProbability(7, 12)}/${maxPrecipProbability(
      12,
      17
    )}/${maxPrecipProbability(17, 21)}`;
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
  canvas {
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
