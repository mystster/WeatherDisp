<script lang="ts">
  import Weather from "./lib/Weather.svelte";
</script>

<main>
  <div id="app">
    <div id="weatherToday">
      <Weather />
    </div>
    <div id="weatherTommorow">
      <Weather />
    </div>
    <div id="weatherTable">
    </div>
    <div id="weatherNow">
    </div>
  </div></main>

<style lang="scss">
  #app {
    display: grid;
    grid-template-columns: 50% 50%;
    grid-template-rows: 50px 58px auto;
    width: 100vw;
    height: 100vh;
  }

  #weatherToday {
    grid-column: 1;
    grid-row: 1;
  }
  #weatherTommorow {
    grid-column: 2;
    grid-row: 1;
  }
  #weatherTable {
    grid-column: 1 / 3;
    grid-row: 2;
  }
  #weatherNow {
    grid-column: 1 / 3;
    grid-row: 3;
  }
</style>
