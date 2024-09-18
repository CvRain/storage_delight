//npm install uuid
//npm install nanoid
import { App } from 'vue'
import {v4 as uuidv4} from 'uuid'
import {nanoid} from 'nanoid'

export const Uuid = () => {
    return uuidv4();
}
export const Nanoid = () => {
    return nanoid();
}