import { App } from 'vue'
import * as base from 'js-base64'

export const BaseEncode =(str:string)=>{
  return base.encode(str)
}
export const BaseDeCode = (b_str:string)=>{
  return base.decode(b_str)
}
export const BaseEncodeImageCanvas = (src:string):string=>{
  let image:HTMLImageElement = new Image()
  image.crossOrigin = ''
  image.src = src
  let base_result:string = ''

  const canvas:HTMLCanvasElement = document.createElement('canvas');
  canvas.width = image.width
  canvas.height = image.height
  const ctx:CanvasRenderingContext2D | null = canvas.getContext('2d')
  ctx?.drawImage(image,0,0,image.width,image.height)
  let ext:string = image.src.substring(image.src.lastIndexOf('.')+1).toLowerCase();
  let dataURL = canvas.toDataURL('image/'+ext)
  image.onload = ()=>{
    base_result = dataURL
  }
  return base_result
}
export const BaseEncodeImageFile = (file:Blob):Promise<string | ArrayBuffer | null>=>{
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.readAsDataURL(file);
    reader.onload = () => {
      resolve(reader.result);
    };
    reader.onerror = error => {
      reject(error);
    };
  });
}