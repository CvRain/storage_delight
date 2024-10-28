import { combineReducers } from 'redux';
import globalReducer from './global';

const rootReducer = combineReducers({
    globalReducer,
});

export default rootReducer;