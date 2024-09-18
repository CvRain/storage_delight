//npm install remixicon --save remix开源图标库
import { App } from 'vue'

export const FileSizeView = (filesize:number) => {

    let filesizeview:string = ''
    if (filesize >= 1073741824) {
        filesizeview = Math.round(filesize / 1073741824 * 100) / 100 +' GB';

    } else if (filesize >= 1048576){
        filesizeview = Math.round(filesize / 1048576 * 100) / 100 +' MB';

    } else if (filesize >= 1024){
        filesizeview = Math.round(filesize / 1024 * 100) / 100 +' KB';
    } else {
        filesizeview = filesize + ' B';

    }

    return {
        'text':filesizeview,
        'size':filesize,
    };
}
export const BlobFile = (file:File):Promise<string | ArrayBuffer | null | Blob>=>{
    return new Promise((resolve, reject) => {
        const reader = new FileReader();
        reader.readAsArrayBuffer(file);
        reader.onload = () => {
            if (reader.result) {
                resolve(new Blob([reader.result], { type: file.type }));
            } else {
                reject(new Error('Failed to read file'));
            }
        };
        reader.onerror = error => {
            reject(error);
        };
    });
}
export const FileExtensionTrim = (filename:string)=>{
    return filename.slice(0, filename.lastIndexOf('.'))
} 

export const FileTypeCheckExcel = (file:File):boolean=>{
    const file_type_accept="application/vnd.ms-excel,application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
    if (!file) {
        return false;
    }

    if (!file_type_accept.includes(file.type)) {
        return false;
    }

    return true;
}

export const FileExtensionIconSet = (extension:string) => {
    extension = extension.trim().toString().toLowerCase();
    let extension_dom = '';
    if(extension=='zip'||
        extension=='rar'||
        extension=='tar'||
        extension=='tar.gz'||
        extension=='7z'){
        extension_dom = `ri-file-zip-line`;
    }else if(
        extension=='mp4'||
        extension=='flv'||
        extension=='avi'||
        extension=='m4v'||
        extension=='mov'||
        extension=='mkv'||
        extension=='dat'||
        extension=='3gp'||
        extension=='vob'||
        extension=='rm'||
        extension=='rmvb'||
        extension=='wmv'||
        extension=='asf'||
        extension=='asx'||
        extension=='flash'

    ){
        extension_dom = `ri-file-music-line`;
    }else if(extension=='iso'){
        extension_dom = `ri-record-circle-line`;
    }else if(
        extension=='mp3'||
        extension=='wav'||
        extension=='cda'||
        extension=='aiff'||
        extension=='mid'||
        extension=='wma'||
        extension=='vqf'||
        extension=='ogg'||
        extension=='aac'||
        extension=='vqf'

    ){
        extension_dom = `ri-file-music-line`;

    }else if(
        extension=='html'||
        extension=='php'||
        extension=='py'||
        extension=='css'||
        extension=='sass'||
        extension=='less'||
        extension=='cpp'||
        extension=='c'||
        extension=='xml'||
        extension=='fxml'||
        extension=='ts'||
        extension=='vue'||
        extension=='java'||
        extension=='jar'||
        extension=='js'
    ){
        extension_dom = `ri-file-code-line`;
    }else if(extension=='jar'){
        extension_dom = `ri-cup-line`;
    }else if(
        extension=='jpg'||
        extension=='ico'||
        extension=='jpeg'||
        extension=='png'||
        extension=='bmp'||
        extension=='gif'||
        extension=='tif'||
        extension=='pcx'||
        extension=='exif'||
        extension=='tga'||
        extension=='svg'||
        extension=='cdr'||
        extension=='pcd'||
        extension=='psd'||
        extension=='dxf'||
        extension=='ufo'||
        extension=='eps'||
        extension=='ai'||
        extension=='webp'||
        extension=='wmf'||
        extension=='raw'||
        extension=='avif'||
        extension=='apng'||
        extension=='pdf'||
        extension=='sai'
    ){
        extension_dom = `ri-image-line`;
    }else if(
        extension=='bat'||
        extension=='sh'||
        extension=='exe'||
        extension=='pak'||
        extension=='msi'
    ){
        extension_dom = `ri-window-fill`;
    }else if(extension=='dll'){
        extension_dom = `ri-code-box-line`;
    }else if(
        extension=='json'||
        extension=='txt'||
        extension=='yml'||
        extension=='pdf'
    ){
        extension_dom = `ri-file-text-line`;
    }else if(extension=='sql'){
        extension_dom = `ri-database-2-line`;
    }else if(extension=='excel'||
        extension=='xlsx'||
        extension=='xls'){
        extension_dom = `ri-file-excel-2-line`;
    }else if(extension=='aff') {
        extension_dom = `ri-server-line`;
    }else if(
        extension=='doc'||
        extension=='docx'
    ){
        extension_dom = `ri-file-word-2-line`;
    }else if(
        extension=='ppt'||
        extension=='pptx'
    ){
        extension_dom = `ri-file-ppt-2-line`;
    }else{
        extension_dom = `ri-file-3-line`;
    }

    return {extension_dom:extension_dom,extension:extension};
}