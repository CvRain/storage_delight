import { combineReducers } from 'redux';
import globalReducer from './global';
import layoutReducer from './layout';

const rootReducer = combineReducers({
    globalReducer,
    layoutReducer,
});

export default rootReducer;