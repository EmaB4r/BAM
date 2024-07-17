#include "Errors.h"



int Option_isSome(Option_t self){
    return (self.type == Some_opt) ? 1 : 0;
}

void* Option_unwrap(Option_t self){
    return self.content;
}

/*
    wraps inside an option of type Some or None a pointer to a content
*/
Option_t Option_wrap(void* content, int type){
    Option_t option;
    option.content=content;
    option.type=type;

    option.fn_isSome=Option_isSome;
    option.fn_unwrap=Option_unwrap;
    
    return option;
}




int Result_isOk(Result_t self){
    return (self.type == Ok_res) ? 1 : 0;
}

void* Result_unwrap(Result_t self){
    return self.content;
}


/*
    wraps inside an option of type Ok or Err a pointer to a content
*/
Result_t Result_wrap(void* content, int type){
    Result_t result;
    result.content=content;
    result.type=type;

    result.fn_isOk=Result_isOk;
    result.fn_unwrap=Result_unwrap;
    

    return result;
}



