#include "HttpMessage.h"

HttpMessage::HttpMessage(){}
HttpMessage::~HttpMessage(){}

int 
HttpMessage::onMessageBegin(){
    return 0;
}

int 
HttpMessage::onUrl(){
    return 0;
}

int 
HttpMessage::onStatus(){
    return 0;
}

int 
HttpMessage::onHeaderField(){
    return 0;
}

int 
HttpMessage::onHeaderValue(){
    return 0;
}

int 
HttpMessage::onHeadersComplete(){
    return 0;
}

int 
HttpMessage::onBodycb(){
    return 0;
}

int 
HttpMessage::onMessageCcompletecb(){
    return 0;
}

void 
HttpMessage::HttpProcess(){
    
    switch(_stage){
          HTTP_ON_MESSAGE_BEGIN,
    HTTP_ON_URL,
    HTTP_ON_STATUS,
    HTTP_ON_HEADER_FIELD, 
    HTTP_ON_HEADER_VALUE,
    HTTP_ON_HEADERS_COMPLETE,
    HTTP_ON_BODY,
    HTTP_ON_MESSAGE_COMPLETE
    }
}