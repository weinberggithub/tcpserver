#ifndef __HTTP_HEADER_H__
#define __HTTP_HEADER_H__
#include<unordered_map>

enum HttpMethod {
    HTTP_METHOD_DELETE      =   0,
    HTTP_METHOD_GET         =   1,
    HTTP_METHOD_HEAD        =   2,
    HTTP_METHOD_POST        =   3,
    HTTP_METHOD_PUT         =   4,
    HTTP_METHOD_CONNECT     =   5,
    HTTP_METHOD_OPTIONS     =   6,
    HTTP_METHOD_TRACE       =   7,
    HTTP_METHOD_COPY        =   8,
    HTTP_METHOD_LOCK        =   9,
    HTTP_METHOD_MKCOL       =   10,
    HTTP_METHOD_MOVE        =   11,
    HTTP_METHOD_PROPFIND    =   12,
    HTTP_METHOD_PROPPATCH   =   13,
    HTTP_METHOD_SEARCH      =   14,
    HTTP_METHOD_UNLOCK      =   15,
    HTTP_METHOD_REPORT      =   16,
    HTTP_METHOD_MKACTIVITY  =   17,
    HTTP_METHOD_CHECKOUT    =   18,
    HTTP_METHOD_MERGE       =   19,
    HTTP_METHOD_MSEARCH     =   20,
    HTTP_METHOD_NOTIFY      =   21,
    HTTP_METHOD_SUBSCRIBE   =   22,
    HTTP_METHOD_UNSUBSCRIBE =   23,
    HTTP_METHOD_PATCH       =   24,
    HTTP_METHOD_PURGE       =   25,
    HTTP_METHOD_MKCALENDAR  =   26
};

using QueryMap = std::unordered_map<std::string,std::string>;
struct URI{
    int                                     port;
    mutable bool                            queryWasModified;
    mutable bool                            initializedQueryMap;
    std::string                             host;
    std::string                             path;
    std::string                             userInfo;
    std::string                             fragment;
    std::string                             scheme;
    mutable std::string                     query;
    mutable QueryMap                        queryMap;
};

class HttpMessage;
//http protocol header or http message header.
class HttpHeader{
    using HeaderMap=std::unordered_map<std::string,std::string>;
public:
    HttpHeader();
    ~HttpHeader();

    const URI& Uri(){ return _uri;}

private:
friend class HttpMessage;
    HeaderMap   _headers;//ignore case.
    URI         _uri;
    int         _statusCode;
    HttpMethod  _method;
    std::string _contentType;
    std::string _unresolvedPath;
};
#endif //__HTTP_HEADER_H__