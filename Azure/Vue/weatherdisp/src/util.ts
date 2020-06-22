/**
 * 風の角度から方角を算出する
 * @param degree 風の角度
 */
export function derectionFromDegree(degree: number): string {
  switch (Math.floor(((degree + 45) % 360) / 90)) {
    case 0:
      return '北';
      break;
    case 1:
      return '東';
    case 2:
      return '南';
      break;
    case 3:
      return '西';
      break;
    default:
      return '';
      break;
  }
}
