
export const HexLightCheck = (hexColor:string)=>{
    const hex = hexColor.replace(/^#/, '');
    const bigint = parseInt(hex, 16);
    const r = (bigint >> 16) & 255;
    const g = (bigint >> 8) & 255;
    const b = bigint & 255;
    const light = 0.299 * r + 0.587 * g + 0.114 * b;
    return light > 128;
}