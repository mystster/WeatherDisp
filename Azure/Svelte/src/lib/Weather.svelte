<script lang="ts">
  import sveltlogo from "../assets/svelte.svg";
</script>

<main>
  <div class="weather" id="weatherPane">
    <div id="dateAndIcon">
      3/1<br />
      <img src={sveltlogo} class="logo" alt="Vite Logo" />
    </div>
    <div id="temp">
      5/15℃
    </div>
    <div id="rainy">
      10 / 10 / 10
    </div>
    <div id="wind">
      南 5m/s
    </div>
  </div>
</main>

<style lang="scss">
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
  img {
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