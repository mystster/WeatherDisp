declare module "date-with-offset" {
    export default class DateWithOffset{
        constructor(tick: number, offset: number);
        getHours(): number;
        date(): Date;
        localDate(): Date;
        getTime(): Date;
        toString(): string;
        getYear(): number;
        getDate(): number;
        getDay(): number;
        getFullYear(): number;
        getHours(): number;
        getMilliseconds(): number;
        getMinutes(): number;
        getMonth(): number;
        getSeconds(): number;
    }
}
