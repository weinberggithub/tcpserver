#ifndef __HTTP_MESSAGE_H__
#define __HTTP_MESSAGE_H__
#include<string>
#include "HttpHeader.h"
#include "Buffer.h"

enum HttpParseStage{
    HTTP_ON_MESSAGE_BEGIN,
    HTTP_ON_URL,
    HTTP_ON_STATUS,
    HTTP_ON_HEADER_FIELD, 
    HTTP_ON_HEADER_VALUE,
    HTTP_ON_HEADERS_COMPLETE,
    HTTP_ON_BODY,
    HTTP_ON_MESSAGE_COMPLETE
};

class HttpMessage{
public:
    HttpMessage();
    ~HttpMessage();

    void HttpProcess();

private:
    //request parser
    int onMessageBegin();
    int onUrl();
    int onStatus();
    int onHeaderField();
    int onHeaderValue();
    int onHeadersComplete();
    int onBodycb();
    int onMessageCcompletecb();
    //response

private:
    HttpParseStage _stage;
    std::string _url;
    HttpHeader _header;
    std::string url;

    Buffer _buf;
};

#endif //__HTTP_MESSAGE_H__