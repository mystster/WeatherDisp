declare module "date-with-offset" {
    export default class DateWithOffset extends Date{
        constructor(tick: number, offset: number);
    }
}
