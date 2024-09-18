import { App } from 'vue'

export const StringIsNull = (str:string) => {
    if(str.length==0||str==""){
        return true;
    }else{
        return false;
    }
}
export const StringTrim = (str:string) => {
    let nextstr:string = str.trim();
    nextstr = nextstr.replaceAll(';','');
    nextstr = nextstr.replaceAll(';','');
    nextstr = nextstr.replaceAll('"','');
    nextstr = nextstr.replaceAll('/','');
    nextstr = nextstr.replaceAll('\\','');
    nextstr = nextstr.replaceAll('+','');
    nextstr = nextstr.replaceAll('=','');
    nextstr = nextstr.replaceAll('.','');
    nextstr = nextstr.replaceAll('(','');
    nextstr = nextstr.replaceAll(')','');
    nextstr = nextstr.replaceAll('{','');
    nextstr = nextstr.replaceAll('}','');
    nextstr = nextstr.replaceAll('[','');
    nextstr = nextstr.replaceAll(']','');
    nextstr = nextstr.replaceAll('@','');
    nextstr = nextstr.replaceAll('#','');
    nextstr = nextstr.replaceAll('$','');
    nextstr = nextstr.replaceAll('%','');
    nextstr = nextstr.replaceAll('^','');
    nextstr = nextstr.replaceAll('&','');
    nextstr = nextstr.replaceAll('*','');
    nextstr = nextstr.replaceAll('-','');
    nextstr = nextstr.replaceAll('_','');
    nextstr = nextstr.replaceAll('`','');
    nextstr = nextstr.replaceAll('~','');
    nextstr = nextstr.replaceAll('!','');
    nextstr = nextstr.replaceAll('|','');
    nextstr = nextstr.replaceAll(':','');
    nextstr = nextstr.replaceAll('>','');
    nextstr = nextstr.replaceAll('<','');
    nextstr = nextstr.replaceAll(',','');
    nextstr = nextstr.replaceAll('》','');
    nextstr = nextstr.replaceAll('《','');
    nextstr = nextstr.replaceAll('—','');
    nextstr = nextstr.replaceAll('?','');
    nextstr = nextstr.replaceAll('【','');
    nextstr = nextstr.replaceAll('】','');
    nextstr = nextstr.replaceAll('？','');
    nextstr = nextstr.replaceAll('￥','');
    nextstr = nextstr.replaceAll('，','');
    nextstr = nextstr.replaceAll('。','');
    nextstr = nextstr.replaceAll('“','');
    nextstr = nextstr.replaceAll('”','');
    nextstr = nextstr.replaceAll('‘','');
    nextstr = nextstr.replaceAll('’','');
    nextstr = nextstr.replaceAll('、','');
    nextstr = nextstr.replaceAll('·','');
    nextstr = nextstr.replaceAll('）','');
    nextstr = nextstr.replaceAll('（','');
    nextstr = nextstr.replaceAll('…','');
    nextstr = nextstr.replaceAll('；','');
    nextstr = nextstr.replaceAll('：','');
    return nextstr;
}
export const upper_letters:Array<string> = [
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z' 
]
export const lower_letters:Array<string> = [
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z'
]